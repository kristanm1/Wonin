#ifndef _WONIN_TOKEN_
#define _WONIN_TOKEN_

#include "Types.hpp"

namespace Wonin
{

    class Token
    {
    public:
        enum class Type
        {
            Sentinel,

            // Single-character tokens
            LeftParen, RightParen, LeftBrace, RightBrace,
            Semicolon, Comma, Dot, Minus, Plus, Slash, Star,

            // One or two character tokens
            Bang, BangEqual, Equal, EqualEqual,
            Greater, GreaterEqual, Less, LessEqual,

            // Literals
            Identifier, Number, String,

            // Keywords
            And, Or, If, Else, True, False,
            Class, This, Super, Fun, Var,
            For, While, Print, Nil, Return,

            EoF,
        };

        Token(usize position, usize line, usize length, Type type);

        usize GetPositios() const { return m_position; }
        usize GetLine() const { return m_line; }
        usize GetLength() const { return m_length; }
        Type GetType() const { return m_type; }

        char const* ToString() const { return ToString(m_type); }

        static char const* ToString(Token::Type type);
    private:
        usize m_position;
        usize m_line;
        usize m_length;
        Type m_type;
    };

}

#endif
