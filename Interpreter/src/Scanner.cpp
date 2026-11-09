#include "Scanner.h"
#include "Log.h"
#include <fstream>

bool Wonin::Scanner::LoadText(std::string const& path)
{
    std::fstream s(path, std::ifstream::in | std::ifstream::binary);
    if (!s.is_open())
    {
        Wonin::Error("File ", path, " does not exist!");
        return false;
    }

    std::streampos pos = s.seekg(0, std::ios::end).tellg();
    if (std::streampos(-1) == pos)
    {
        Wonin::Error("End of file ", path ," could not be found!");
        return false;
    }

    s.seekg(0, std::ios::beg);
    size_t size = static_cast<size_t>(pos);

    m_src = std::move(std::string(size, '\0'));
    s.read(&m_src[0], size);

    return true;
}

void Wonin::Scanner::AddToken(Wonin::Token::Type type, unsigned length)
{
    m_tokens.emplace_back(type, m_position, length, m_line);
}

unsigned Wonin::Scanner::ScanToken()
{
    using Type = Wonin::Token::Type;

    char c = m_src[m_position];
    switch (c)
    {
        case '(':
            AddToken(Type::LeftParenthesis, 1);
            return 1;
        case ')':
            AddToken(Type::RightParenthesis, 1);
            return 1;
        case '{':
            AddToken(Type::LeftCurlyBracket, 1);
            return 1;
        case '}':
            AddToken(Type::RightCurlyBracket, 1);
            return 1;
        case '+':
            AddToken(Type::Plus, 1);
            return 1;
        case '-':
            AddToken(Type::Minus, 1);
            return 1;
        case '*':
            AddToken(Type::Star, 1);
            return 1;
        case '|':
            if (IsMatchAt('|', 1))
            {
                AddToken(Type::Or, 2);
                return 2;
            }

            m_errors++;
            Wonin::Error("Unknown token | at line ", m_line);
            return 1;
        case '&':
            if (IsMatchAt('&', 1))
            {
                AddToken(Type::And, 2);
                return 2;
            }

            m_errors++;
            Wonin::Error("Unknown token & at line ", m_line);
            return 1;
        case '!':
            if (IsMatchAt('=', 1))
            {
                AddToken(Type::NotEqual, 2);
                return 2;
            }

            AddToken(Type::Negation, 1);
            return 1;
        case '=':
            if (IsMatchAt('=', 1))
            {
                AddToken(Type::Equal, 2);
                return 2;
            }

            AddToken(Type::Assignment, 1);
            return 1;
        case '<':
            if (IsMatchAt('=', 1))
            {
                AddToken(Type::LessOrEqual, 2);
                return 2;
            }

            AddToken(Type::Less, 1);
            return 1;
        case '>':
            if (IsMatchAt('=', 1))
            {
                AddToken(Type::GreaterOrEqual, 2);
                return 2;
            }

            AddToken(Type::Greater, 1);
            return 1;
        case '/':
            if (IsMatchAt('/', 1))
            {
                int offset = 2;
                while (!IsAtEnd(offset))
                {
                    if (IsMatchAt('\n', offset))
                    {
                        break;
                    }
                    offset++;
                }
                return offset;
            }
            
            AddToken(Type::Slash, 1);
            return 1;
        case '"':
        {
            int offset = 1;
            for (;;offset++)
            {
                if (IsAtEnd(offset) || IsMatchAt('\n', offset))
                {
                    m_errors++;
                    Wonin::Error("Unterminated string literal at line ", m_line);
                    break;
                }

                if (IsMatchAt('"', offset))
                {
                    AddToken(Type::StringLiteral, ++offset);
                    break;
                }
            }
            return offset;
        }
        case ' ':
        case '\t':
        case '\r':
            return 1;
        case '\n':
            m_line++;
            return 1;
        default:
            if (IsDigit(c))
            {
                unsigned offset = 1;
                for (;!IsAtEnd(offset) && IsDigitAt(offset); offset++)
                {}

                if (IsMatchAt('.', offset))
                {
                    for (offset++; !IsAtEnd(offset) && IsDigitAt(offset); offset++)
                    {}
                }

                AddToken(Type::NumberLiteral, offset);
                return offset;
            }

            m_errors++;
            Wonin::Error("Unknown token: ", c, " in line: ", m_line);
            return 1;
    }

}

bool Wonin::Scanner::ScanTokens()
{
    m_errors = 0;
    m_line = 1;
    for (m_position = 0; m_position < m_src.size(); m_position += ScanToken())
    {}
    
    AddToken(Wonin::Token::Type::EoF, 1);
    
    return m_errors == 0;
}

void Wonin::Scanner::Display()
{
    for (auto const& t : m_tokens)
    {
        unsigned end = t.m_position + t.m_length;
        std::stringstream ss;
        for (unsigned i = t.m_position; i < end; i++)
        {
            ss << m_src[i];
        }
        Wonin::Info(t.ToString(), " ", ss.str(), " at line ", t.m_line);
    }
}
