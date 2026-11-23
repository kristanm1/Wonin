#ifndef _WONIN_PARSER_
#define _WONIN_PARSER_

#include "types.hpp"
#include "scanner.hpp"
#include "expr.hpp"
#include "stmt.hpp"

#include <vector>

namespace wonin
{

    class Parser
    {
        friend class Interpreter;
    public:
        Parser(Scanner const& scanner) : m_scanner(scanner) {}

        bool parse();

    private:
        bool isToken(Token token) const { return m_scanner.m_lexemes[m_position].token() == token; }
        bool isEnd()              const { return isToken(Token::EoF); }

        template<unsigned N>
        bool isToken(Token const* tokens);

        void sync();

        StmtPtr parseDecl();
        StmtPtr parseVarDecl();

        StmtPtr parseStmt();
        StmtPtr parseStmtExpr();
        StmtPtr parseStmtPrint();
        StmtPtr parseStmtBlock();

        ExprPtr parseExpr();
        ExprPtr parseExprAsgn();
        ExprPtr parseExprEquality();
        ExprPtr parseExprComparison();
        ExprPtr parseExprTerm();
        ExprPtr parseExprFactor();
        ExprPtr parseExprUnary();
        ExprPtr parseExprPrimary();

    private:
        Scanner const& m_scanner;
        std::vector<StmtPtr> m_stmts{};
        usize m_position = 0;
        bool m_hasError = false;
    };

}

#endif
