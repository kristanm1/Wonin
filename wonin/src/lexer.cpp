#include "lexer.hpp"
#include "log.hpp"
#include "utils.hpp"

#include <sstream>
#include <string_view>
#include <unordered_map>

using namespace wonin;

unordered_map<Token> Lexer::s_keywords =
{
    { "and",    Token::And },
    { "or" ,    Token::Or },
    { "if" ,    Token::If },
    { "else",   Token::Else },
    { "true",   Token::True },
    { "false",  Token::False },
    { "class",  Token::Class }, 
    { "this",   Token::This },
    { "super",  Token::Super },
    { "fun",    Token::Fun },
    { "var",    Token::Var },
    { "for",    Token::For },
    { "while",  Token::While },
    { "print",  Token::Print },
    { "nil",    Token::Nil },
    { "return", Token::Return },
};

bool Lexer::scan(std::string const& source)
{
    m_lexemes.clear();
    m_hasError = false;
    m_position = 0;
    m_source = &source;
    while (m_position < (*m_source).size())
        m_position += matchToken();
    
    m_lexemes.emplace_back(m_position, 0, Token::EoF);
    
    return !m_hasError;
}

void Lexer::print()
{
    for (usize i = 0; i < m_lexemes.size(); i++)
        printLexeme(i);
}

void Lexer::printLexeme(usize position) const
{
    auto const& lexeme = m_lexemes[position];
    if (lexeme.length() == 0)
        log::info(
            "i: ", position, ", ",
            "token: ", lexeme.cstrToken(), ", "
            "line: ", lineNumber(lexeme)
        );
    else
        log::info(
            "i: ", position, ", ",
            "token: ", lexeme.cstrToken(), ", ",
            "lexeme: ", lexemeView(lexeme), ", ",
            "line: ", lineNumber(lexeme)
        );
}

usize Lexer::matchToken()
{
    switch ((*m_source)[m_position])
    {
    case '(':
        return addLexeme(m_position, 1, Token::LeftParen);

    case ')':
        return addLexeme(m_position, 1, Token::RightParen);

    case '{':
        return addLexeme(m_position, 1, Token::LeftBrace);

    case '}':
        return addLexeme(m_position, 1, Token::RightBrace);

    case ';':
        return addLexeme(m_position, 1, Token::Semicolon);

    case '+':
        return addLexeme(m_position, 1, Token::Plus);

    case '-':
        return addLexeme(m_position, 1, Token::Minus);

    case '*':
        return addLexeme(m_position, 1, Token::Star);
    
    case ',':
        return addLexeme(m_position, 1, Token::Comma);
    
    case '.':
        return addLexeme(m_position, 1, Token::Dot);

    case '/':
        if (!isEnd(1) && isMatch('/', 1))
            return matchLineComment();
        return addLexeme(m_position, 1, Token::Slash);

    case '!':
        if (!isEnd(1) && isMatch('=', 1))
            return addLexeme(m_position, 2, Token::BangEqual);
        return addLexeme(m_position, 1, Token::Bang);

    case '=':
        if (!isEnd(1) && isMatch('=', 1))
            return addLexeme(m_position, 2, Token::EqualEqual);
        return addLexeme(m_position, 1, Token::Equal);

    case '>':
        if (!isEnd(1) && isMatch('=', 1))
            return addLexeme(m_position, 2, Token::GreaterEqual);
        return addLexeme(m_position, 1, Token::Greater);

    case '<':
        if (!isEnd(1) && isMatch('=', 1))
            return addLexeme(m_position, 2, Token::LessEqual);
        return addLexeme(m_position, 1, Token::Less);

    case '"':
        return matchString();

    case ' ':
    case '\t':
    case '\r':
    case '\n':
        return 1;

    default:
        if (utils::isDigit((*m_source)[m_position]))
            return matchNumber();
        else if (utils::isAlpha((*m_source)[m_position]))
            return matchIdentifier();
        
        m_hasError = true;
        log::lexerError(
            log::stringify("Unexpected character ", (*m_source)[m_position]),
            lineNumber(m_position)
        );
        
        return 1;
    }

}

strview Lexer::lexemeView(Lexeme const& lexeme) const
{
    return std::string_view(&(*m_source)[lexeme.position()], lexeme.length());
}

str Lexer::lexemeString(Lexeme const& lexeme) const
{
    return std::string(lexemeView(lexeme));
}

double Lexer::lexemeNumber(Lexeme const& lexeme) const
{
    char const* begin = &(*m_source)[lexeme.position()];
    char* end = const_cast<char*>(begin) + lexeme.length();
    return std::strtod(begin, &end);
}

usize Lexer::lineNumber(usize position) const
{
    usize line = 1;
    for (usize i = 0; i < position; i++)
        if ((*m_source)[i] == '\n')
            line++;

    return line;
}

usize Lexer::addLexeme(usize position, usize length, Token token)
{
    m_lexemes.emplace_back(position, length, token);
    return length;
}

usize Lexer::matchLineComment()
{
    usize offset = 2;
    while (!isEnd(offset))
    {
        if (isMatch('\n', offset))
            break;
        
        offset++;
    }

    return offset;
}

usize Lexer::matchString()
{
    usize offset = 1;
    while (!isEnd(offset))
    {
        if (isMatch('\n', offset))
            break;

        if (isMatch('"', offset++))
            return addLexeme(m_position + 1, offset - 2, Token::String) + 2;
    }

    m_hasError = true;
    log::lexerError("Unterminated string literal", lineNumber(m_position));

    return offset;
}

usize Lexer::matchNumber()
{
    usize offset = 1;
    while (!isEnd(offset))
    {
        if (!isDigit(offset))
            break;
        
        offset++;
    }

    if (!isEnd(offset) && isMatch('.', offset))
    {
        offset++;
        while (!isEnd(offset))
        {
            if (!isDigit(offset))
                break;
            
            offset++;
        }
    }
    
    return addLexeme(m_position, offset, Token::Number);
}

usize Lexer::matchIdentifier()
{
    usize offset = 1;
    while (!isEnd(offset))
    {
        if (!isAlphaNumeric(offset))
            break;

        offset++;
    }

    auto it = s_keywords.find(std::string_view(&(*m_source)[m_position], offset));
    if (it != s_keywords.end())
        return addLexeme(m_position, offset, it->second);
    return addLexeme(m_position, offset, Token::Identifier);
}
