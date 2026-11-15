#ifndef _WONIN_PARSER_
#define _WONIN_PARSER_

#include "Statement.hpp"
#include "Types.hpp"
#include "Scanner.hpp"
#include "Expr.hpp"

namespace Wonin
{

    class ParserError : public RuntimeError
    {
    public:
        enum class Type
        {
            PrimNoMatch,
            PrimMissRightParen,
            MissSemicolon,
            OnlyEofToken,
        };

        ParserError(Type type) : m_type(type) {}
        char const* GetMessage() const override;
    private:
        Type m_type;
    };

    class Parser
    {
    public:
        Parser(Scanner const& scanner, std::vector<Token> const& tokens);
        ExprPtr Parse();

        bool HasError() { return m_hasError; }

    private:
        bool IsAtEnd() { return m_tokens[m_position].m_type == Token::Type::EoF; }
        bool IsMatch(Token::Type type) { return !IsAtEnd() && m_tokens[m_position].m_type == type; }

        template<unsigned N>
        bool IsMatch(Token::Type const* types);

        void Sync();

        //Statement* PrintStatement();
        //Statement* ExpressionStatement();
        //Statement* Statement();

        ExprPtr ParseExpr();
        ExprPtr ParseEquality();
        ExprPtr ParseComparison();
        ExprPtr ParseTerm();
        ExprPtr ParseFactor();
        ExprPtr ParseUnary();
        ExprPtr ParsePrimary();

        unsigned m_position = 0;
        Scanner const& m_scanner;
        std::vector<Token> const& m_tokens;
        bool m_hasError;
    };

}

#endif
