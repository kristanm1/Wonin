#ifndef _WONIN_PARSER_
#define _WONIN_PARSER_

#include "types.hpp"
#include "lexer.hpp"

#include <vector>

namespace wonin
{

    struct Expr;
    struct Stmt;

    using exprptr  = std::shared_ptr<Expr>;
    using stmtptr  = std::shared_ptr<Stmt>;
    using stmtlist = std::vector<stmtptr>;

    class Parser
    {
        friend class Interpreter;
    public:
        Parser(Lexer const& lexer) : m_lexer(lexer) {}

        bool parse();

    private:
        bool isToken(Token token) const { return m_lexer.m_lexemes[m_position].token() == token; }
        bool isEnd()              const { return m_position >= m_lexer.m_lexemes.size() || isToken(Token::EoF); }
        
        template<unsigned N>
        bool ifTokenConsumeIt(Token const* tokens);
        bool ifTokenConsumeIt(Token token);

        void   sync();
        Lexeme const& current()  { return m_lexer.m_lexemes[m_position]; }
        Lexeme const& previous() { return m_lexer.m_lexemes[m_position - 1]; }

        stmtptr parseDecl();
        stmtptr parseVarDecl();
        stmtptr parseFunDecl();

        stmtptr parseStmt();
        stmtptr parseStmtExpr();
        stmtptr parseStmtPrint();
        stmtptr parseStmtReturn();
        stmtptr parseStmtBlock();
        stmtptr parseStmtIf();
        stmtptr parseStmtWhile();
        stmtptr parseStmtFor();

        exprptr parseExpr();
        exprptr parseExprAsgn();
        exprptr parseExprOr();
        exprptr parseExprAnd();
        exprptr parseExprEquality();
        exprptr parseExprComparison();
        exprptr parseExprTerm();
        exprptr parseExprFactor();
        exprptr parseExprUnary();
        exprptr parseExprCall();
        exprptr parseExprPrimary();

    private:
        exprptr parseParenthesizedExpr(char const* where);
        exprptr parseCallArguments(exprptr&& expr);

    private:
        Lexer const& m_lexer;
        stmtlist     m_stmts{};
        usize        m_position = 0;
        bool         m_hasError = false;
    };

}

#endif
