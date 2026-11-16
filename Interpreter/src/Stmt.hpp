#ifndef _WONIN_STATEMENT_
#define _WONIN_STATEMENT_

#include "Visitor.hpp"

#include <memory>

namespace Wonin
{

    struct Expr;

    struct Stmt
    {
        virtual ~Stmt() = default;
        virtual void Accept(Visitor& visitor) const = 0;
    };

    using StmtPtr = std::unique_ptr<Stmt>;

    struct StmtExpr : public Stmt
    {
        StmtExpr(ExprPtr&& expr) : m_expr(std::move(expr)) {}
        void Accept(Visitor& visitor) const override
        {
            visitor.Visit(*this);
        }
        ExprPtr m_expr;
    };

    struct StmtPrint : public Stmt
    {
        StmtPrint(ExprPtr&& expr) : m_expr(std::move(expr)) {}
        void Accept(Visitor& visitor) const override
        {
            visitor.Visit(*this);
        }
        ExprPtr m_expr;
    };

}

#endif
