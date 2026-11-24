#ifndef _WONIN_PARSER_
#define _WONIN_PARSER_

#include "types.hpp"
#include "lexer.hpp"
#include "expr.hpp"
#include "stmt.hpp"

#include <vector>

namespace wonin
{

    class Parser
    {
        friend class Interpreter;
    public:
        Parser(Lexer const& lexer) : m_lexer(lexer) {}

        bool parse();

    private:
        bool isToken(Token token) const { return m_lexer.m_lexemes[m_position].token() == token; }
        bool isEnd()              const { return isToken(Token::EoF); }
        
        template<unsigned N>
        bool isToken(Token const* tokens) const;
        bool ifTokenConsumeIt(Token token);

        void sync();

        StmtPtr parseDecl();
        StmtPtr parseVarDecl();

        StmtPtr parseStmt();
        StmtPtr parseStmtExpr();
        StmtPtr parseStmtPrint();
        StmtPtr parseStmtBlock();
        StmtPtr parseStmtIf();
        StmtPtr parseStmtWhile();
        StmtPtr parseStmtFor();

        ExprPtr parseExpr();
        ExprPtr parseExprAsgn();
        ExprPtr parseExprOr();
        ExprPtr parseExprAnd();
        ExprPtr parseExprEquality();
        ExprPtr parseExprComparison();
        ExprPtr parseExprTerm();
        ExprPtr parseExprFactor();
        ExprPtr parseExprUnary();
        ExprPtr parseExprPrimary();

    private:
        ExprPtr parseParenthesizedExpr(char const* where);

    private:
        Lexer const& m_lexer;
        std::vector<StmtPtr> m_stmts{};
        usize m_position = 0;
        bool m_hasError = false;
    };

}

#endif
