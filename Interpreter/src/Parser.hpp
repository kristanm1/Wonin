#ifndef _WONIN_PARSER_
#define _WONIN_PARSER_

#include "Types.hpp"
#include "Scanner.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

#include <vector>

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

        ParserError(Type type, Token const& token)
            : m_type(type)
            , m_token(token)
        {}
        
        std::string GetMessage() const override;
    private:
        Token const& m_token;
        Type m_type;
    };

    class Parser
    {
    public:
        Parser(Scanner const& scanner, std::vector<Token> const& tokens);
        std::vector<StmtPtr> Parse();

        bool HasError() { return m_hasError; }

    private:
        bool IsAtEnd()
        {
            return m_tokens[m_position].GetType() == Token::Type::EoF;
        }

        bool IsMatch(Token::Type type)
        {
            return !IsAtEnd() && m_tokens[m_position].GetType() == type;
        }

        template<unsigned N>
        bool IsMatch(Token::Type const* types);

        void Sync();

        StmtPtr ParseStmt();
        StmtPtr ParseStmtExpr();
        StmtPtr ParseStmtPrint();

        ExprPtr ParseExpr();
        ExprPtr ParseExprEquality();
        ExprPtr ParseExprComparison();
        ExprPtr ParseExprTerm();
        ExprPtr ParseExprFactor();
        ExprPtr ParseExprUnary();
        ExprPtr ParseExprPrimary();

        unsigned m_position = 0;
        Scanner const& m_scanner;
        std::vector<Token> const& m_tokens;
        bool m_hasError;
    };

}

#endif
