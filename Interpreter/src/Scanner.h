#ifndef _WONIN_SCANNER_
#define _WONIN_SCANNER_

#include <string>
#include <vector>

namespace Wonin
{

    class Token
    {
        friend class Scanner;
        friend class Parser;
    public:
        enum class Type
        {
            // Single-character tokens.
            LeftParenthesis, RightParenthesis,
            LeftCurlyBracket, RightCurlyBracket,
            Minus, Plus, Slash, Star,

            // One or two character tokens.
            Negation,
            Assignment,
            Equal, NotEqual,
            Greater, GreaterOrEqual,
            Less, LessOrEqual,

            // Literals.
            Identifier, NumberLiteral, StringLiteral,

            // Keywords.
            If, Else,
            True, False,
            And, Or,

            EoF,
        };

        Token(Type type, unsigned position, unsigned length, unsigned line)
            : m_type(type)
            , m_position(position)
            , m_length(length)
            , m_line(line)
        {}

        char const* ToString() const
        {
            switch (m_type)
            {
                case Type::LeftParenthesis:   return "left parenthesis";
                case Type::RightParenthesis:  return "right parenthesis";
                case Type::LeftCurlyBracket:  return "left curly bracket";
                case Type::RightCurlyBracket: return "right curly bracket";
                case Type::Minus:             return "minus";
                case Type::Plus:              return "plus";
                case Type::Slash:             return "slash";
                case Type::Star:              return "star";
                case Type::Negation:          return "negation";
                case Type::Assignment:        return "assignment";
                case Type::Equal:             return "equal";
                case Type::NotEqual:          return "not equal";
                case Type::Greater:           return "greater";
                case Type::GreaterOrEqual:    return "greater or equal";
                case Type::Less:              return "less";
                case Type::LessOrEqual:       return "less or equal";
                case Type::Identifier:        return "identifier";
                case Type::NumberLiteral:     return "number literal";
                case Type::StringLiteral:     return "string literal";
                case Type::If:                return "if";
                case Type::Else:              return "else";
                case Type::True:              return "true";
                case Type::False:             return "false";
                case Type::And:               return "and";
                case Type::Or:                return "or";
                case Type::EoF:               return "eof";
                default:                      return "unknown";
            }
        }
    
    private:
        Type m_type;
        unsigned m_position;
        unsigned m_length;
        unsigned m_line;
    };

    class Scanner
    {
        friend class Parser;
    public:
        bool LoadText(std::string const& path);
        bool ScanTokens();
        bool HasErrors() { return m_errors > 0; }
        void Display();
    private:
        bool IsDigit(char c) { return c >= '0' && c <= '9'; }
        bool IsLower(char c) { return 'a' >= c && c <= 'z'; }
        bool IsUpper(char c) { return 'A' >= c && c <= 'Z'; }
        bool IsAlpha(char c) { return IsLower(c) || isupper(c) || c == '_'; }
        bool IsAlphaNumeric(char c) { return IsAlpha(c) || IsDigit(c); }

        bool IsAtEnd(unsigned offset) { return m_position + offset >= m_src.size(); }
        bool IsMatchAt(char c, unsigned offset) { return !IsAtEnd(offset) && c == m_src[m_position + offset]; }
        bool IsDigitAt(unsigned offset) { return IsDigit(m_src[m_position + offset]); }

        void AddToken(Wonin::Token::Type type, unsigned length);
        unsigned ScanToken();

        std::string m_src{};
        std::vector<Token> m_tokens{};
        unsigned m_position = 0;
        unsigned m_line = 1;

        unsigned m_errors = 0;
    };

}

#endif
