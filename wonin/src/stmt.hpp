#ifndef _WONIN_STATEMENT_
#define _WONIN_STATEMENT_

#include "visitor.hpp"

#include <memory>

namespace Wonin
{
    struct Token;

    struct Expr;

    struct Stmt
    {
        virtual ~Stmt() = default;
        virtual void Accept(Visitor const& visitor) const = 0;
    };

    using StmtPtr = std::unique_ptr<Stmt>;

    struct StmtExpr : public Stmt
    {
        StmtExpr(ExprPtr&& expr) : m_expr(std::move(expr)) {}
        void Accept(Visitor const& visitor) const override
        {
            visitor.Visit(*this);
        }
        ExprPtr m_expr;
    };

    struct StmtPrint : public Stmt
    {
        StmtPrint(ExprPtr&& expr) : m_expr(std::move(expr)) {}
        void Accept(Visitor const& visitor) const override
        {
            visitor.Visit(*this);
        }
        ExprPtr m_expr;
    };

    struct StmtVarDecl : public Stmt
    {
        StmtVarDecl(Token const& identifier, ExprPtr&& expr)
            : m_identifier(identifier), m_expr(std::move(expr)) {}
        void Accept(Visitor const& visitor) const override
        {
            visitor.Visit(*this);
        }
        Token const& m_identifier;
        ExprPtr m_expr;
    };

}

#endif
