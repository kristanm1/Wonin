#ifndef _WONIN_PARSER_
#define _WONIN_PARSER_

#include "types.hpp"
#include "scanner.hpp"
#include "expr.hpp"
#include "stmt.hpp"

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

            DeclIdMiss,

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
        Parser(Scanner const& scanner) : m_scanner(scanner) {}

        bool Parse();

        std::vector<StmtPtr> const& GetStatements() const { return m_stmts; }
        Scanner const& GetScanner() const { return m_scanner; }

    private:
        bool IsAtEnd()
        {
            return m_scanner.IsTokenMatch(Token::Type::EoF, m_position);
        }

        bool IsMatch(Token::Type type)
        {
            return !IsAtEnd() && m_scanner.IsTokenMatch(type, m_position);
        }

        template<unsigned N>
        bool IsMatch(Token::Type const* types);

        void Sync();

        StmtPtr ParseDecl();
        StmtPtr ParseVarDecl();

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

        Scanner const& m_scanner;
        std::vector<Wonin::StmtPtr> m_stmts{};
        unsigned m_position = 0;
        bool m_hasError;
    };

}

#endif
