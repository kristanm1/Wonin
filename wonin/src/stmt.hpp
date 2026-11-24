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

    struct StmtExpr : Stmt
    {
        StmtExpr(ExprPtr&& expr) : m_expr(std::move(expr)) {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        ExprPtr m_expr;
    };

    struct StmtPrint : Stmt
    {
        StmtPrint(ExprPtr&& expr) : m_expr(std::move(expr)) {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        ExprPtr m_expr;
    };

    struct StmtVarDecl : Stmt
    {
        StmtVarDecl(Lexeme const& variable, ExprPtr&& initializer)
            : m_variable(variable), m_initializer(std::move(initializer))
        {}
        
        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        Lexeme const& m_variable;
        ExprPtr m_initializer;
    };

    struct StmtBlock : Stmt
    {
        StmtBlock(std::vector<StmtPtr>&& stmts) : m_stmts(std::move(stmts)) {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        std::vector<StmtPtr> m_stmts;
    };

    struct StmtIf : Stmt
    {
        StmtIf(ExprPtr&& condition, StmtPtr&& stmtThen, StmtPtr&& stmtElse)
            : m_condition(std::move(condition))
            , m_stmtThen(std::move(stmtThen))
            , m_stmtElse(std::move(stmtElse))
        {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        ExprPtr m_condition;
        StmtPtr m_stmtThen;
        StmtPtr m_stmtElse;
    };

    struct StmtWhile : Stmt
    {
        StmtWhile(ExprPtr&& condition, StmtPtr&& body)
            : m_condition(std::move(condition))
            , m_body(std::move(body))
        {}

        void accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        ExprPtr m_condition;
        StmtPtr m_body;
    };
}

#endif
