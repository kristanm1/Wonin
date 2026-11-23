#ifndef _WONIN_TOKEN_
#define _WONIN_TOKEN_

#include "types.hpp"

namespace wonin
{

    enum class Token
    {
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

    class Lexeme
    {
    public:
        Lexeme(usize position, usize length, Token token)
            : m_position(position)
            , m_length(length)
            , m_token(token)
        {}

        usize      position() const { return m_position; }
        usize      length()   const { return m_length; }
        Token      token()    const { return m_token; }

        cstr cstrToken() const { return cstrToken(m_token); }

    private:
        static cstr cstrToken(Token token);

        usize m_position;
        usize m_length;
        Token m_token;
    };

}

#endif
