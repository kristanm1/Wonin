#include "scanner.hpp"
#include "log.hpp"

#include <sstream>
#include <string_view>
#include <unordered_map>

using namespace Wonin;
using Type = Token::Type;

namespace
{
    struct MapHash
    {
        using is_transparent = void;
        size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv);}
    };

    std::unordered_map<std::string, Type, MapHash, std::equal_to<>> s_keywords =
    {
        { "and",    Type::And },
        { "or" ,    Type::Or },
        { "if" ,    Type::If },
        { "else",   Type::Else },
        { "true",   Type::True },
        { "false",  Type::False },
        { "class",  Type::Class }, 
        { "this",   Type::This },
        { "super",  Type::Super },
        { "fun",    Type::Fun },
        { "var",    Type::Var },
        { "for",    Type::For },
        { "while",  Type::While },
        { "print",  Type::Print },
        { "nil",    Type::Nil },
        { "return", Type::Return },
    };
}

void Scanner::PrintTokens()
{
    for (usize i = 0; i < m_tokens.size(); i++)
    {
        auto const& token = m_tokens[i];
        usize end = token.GetPositios() + token.GetLength();
        std::stringstream ss;
        for (usize j = token.GetPositios(); j < end; j++)
        {
            ss << m_source[j];
        }
        Log::Info(i, " ", token.ToString(), " ", ss.str(), " at line ", token.GetLine());
    }
}

void Scanner::ScannerError(std::string message)
{
    m_hasError = true;
    Log::Error("Scanner error at line ", m_line, ": " , message);
}

void Scanner::ScannerError(char character)
{
    ScannerError((std::stringstream{} << "Unexpected character " << std::string(1, character)).str());
}

Token Scanner::MatchLineComment()
{
    usize offset = 2;
    while (!IsAtEnd(offset))
    {
        if (IsMatchAt('\n', offset))
        {
            break;
        }
        offset++;
    }
    return Token(m_position, m_line, offset, Type::Sentinel);
}

Token Scanner::MatchString()
{
    int offset = 1;
    while (!IsMatchAt(offset, '\n'))
        if (m_source[m_position + offset++] == '"')
            return Token(m_position, m_line, offset, Type::String);

    m_hasError = true;
    ScannerError("Unterminated string literal");
    return Token(m_position, m_line, offset, Type::Sentinel);
}

Token Scanner::MatchDigit()
{
    usize offset = 1;
    for (;!IsAtEnd(offset) && IsDigitAt(offset); offset++)
    {}

    if (IsMatchAt('.', offset))
        for (offset++; !IsAtEnd(offset) && IsDigitAt(offset); offset++)
        {}

    return Token(m_position, m_line, offset, Type::Number);
}

Token Scanner::MatchIdentifier()
{
    usize offset = 1;
    while (!IsAtEnd(offset) && Utils::IsAlphaNumeric(m_source[m_position + offset]))
        offset++;

    auto it = s_keywords.find(std::string_view(&m_source[m_position], offset));
    if (it != s_keywords.end())
        return Token(m_position, m_line, offset, it->second);    

    return Token(m_position, m_line, offset, Type::Identifier);
}

Token Scanner::MatchKeyword()
{
    return Token(0, 0, 0, Type::Sentinel);
}

Token Scanner::GetNextToken()
{
    switch (m_source[m_position])
    {
        case '(': return Token(m_position, m_line, 1, Type::LeftParen);
        case ')': return Token(m_position, m_line, 1, Type::RightParen);
        case '{': return Token(m_position, m_line, 1, Type::LeftBrace);
        case '}': return Token(m_position, m_line, 1, Type::RightBrace);
        case ';': return Token(m_position, m_line, 1, Type::Semicolon);
        case '+': return Token(m_position, m_line, 1, Type::Plus);
        case '-': return Token(m_position, m_line, 1, Type::Minus);
        case '*': return Token(m_position, m_line, 1, Type::Star);
        case '/': return IsMatchAt('/', 1)
            ? MatchLineComment()
            : Token(m_position, m_line, 1, Type::Slash);

        case '!': return IsMatchAt('=', 1)
            ? Token(m_position, m_line, 2, Type::BangEqual)
            : Token(m_position, m_line, 1, Type::Bang);
        case '=': return IsMatchAt('=', 1)
            ? Token(m_position, m_line, 2, Type::EqualEqual)
            : Token(m_position, m_line, 1, Type::Equal);
        case '>': return IsMatchAt('=', 1)
            ? Token(m_position, m_line, 2, Type::GreaterEqual)
            : Token(m_position, m_line, 2, Type::Greater);
        case '<': return IsMatchAt('=', 1)
            ? Token(m_position, m_line, 2, Type::LessEqual)
            : Token(m_position, m_line, 1, Type::Less);

        case '"': return MatchString();
        case ' ':
        case '\t':
        case '\r': return Token(m_position, m_line, 1, Type::Sentinel);
        case '\n': return Token(m_position, m_line + 1, 1, Type::Sentinel);
        default:
            if (Utils::IsDigit(m_source[m_position]))
                return MatchDigit();
            else if(Utils::IsAlpha(m_source[m_position]))
                return MatchIdentifier();

            ScannerError(m_source[m_position]);
            return Token(m_position, m_line, 1, Type::Sentinel);
    }

}

bool Scanner::ScanTokens()
{
    m_position = 0;
    m_line = 1;
    m_hasError = false;

    while (m_position < m_source.size())
    {
        Token token = GetNextToken();
        if (token.GetType() != Type::Sentinel)
        {
            m_tokens.push_back(token);
        }

        m_position += token.GetLength();
        m_line = token.GetLine();
    }
    
    m_tokens.emplace_back(m_position, m_line, 0, Type::EoF);
    
    return !m_hasError;
}
