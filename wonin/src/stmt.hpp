#ifndef _WONIN_STATEMENT_
#define _WONIN_STATEMENT_

#include "visitor.hpp"

#include <memory>

namespace wonin
{
    struct Lexeme;
    struct Expr;
    struct Stmt
    {
        virtual      ~Stmt()                              = default;
        virtual void accept(Visitor const& visitor) const = 0;
    };

    using StmtPtr = std::unique_ptr<Stmt>;

    struct StmtExpr : public Stmt
    {
        StmtExpr(ExprPtr&& expr) : m_expr(std::move(expr)) {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        ExprPtr m_expr;
    };

    struct StmtPrint : public Stmt
    {
        StmtPrint(ExprPtr&& expr) : m_expr(std::move(expr)) {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        ExprPtr m_expr;
    };

    struct StmtVarDecl : public Stmt
    {
        StmtVarDecl(Lexeme const& lexeme, ExprPtr&& expr)
            : m_lexeme(lexeme), m_expr(std::move(expr))
        {}
        
        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        Lexeme const& m_lexeme;
        ExprPtr m_expr;
    };

    struct StmtBlock : public Stmt
    {
        StmtBlock(std::vector<StmtPtr>&& stmts) : m_stmts(std::move(stmts)) {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        std::vector<StmtPtr> m_stmts;
    };

}

#endif
