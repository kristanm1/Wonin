#ifndef _WONIN_PARSER_
#define _WONIN_PARSER_

#include "Expression.h"
#include "Scanner.h"

namespace Wonin
{

    class Parser
    {
    public:
        Parser(Scanner const& scanner);
        Expression* Parse() { return Expr(); }
    private:
        template<unsigned Count>
        bool IsMatch(Token::Type types[Count]);

        bool IsMatch(Token::Type type)
        {
            return !IsAtEnd() && m_scanner.m_tokens[m_position].m_type == type;
        }

        bool IsAtEnd()
        {
            return m_position >= m_scanner.m_tokens.size()
                || m_scanner.m_tokens[m_position].m_type == Token::Type::EoF;
        }

        Expression* Expr();
        Expression* Equality();
        Expression* Comparison();
        Expression* Term();
        Expression* Factor();
        Expression* Unary();
        Expression* Primary();

        unsigned m_position = 0;
        Scanner const& m_scanner;
    };

}

#endif
