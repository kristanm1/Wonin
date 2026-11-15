#ifndef _WONIN_STATEMENT_
#define _WONIN_STATEMENT_

#include "Visitor.hpp"

namespace Wonin
{

    struct Expression;

    struct Statement
    {
        virtual ~Statement() = default;
        virtual void Accept(StatementVisitor& visitor) const = 0;
    };

    struct StmtExpression : public Statement
    {
        StmtExpression(Expression *expr) : m_expr(expr) {}
        void Accept(StatementVisitor& visitor) const override { visitor.Visit(*this); }
        Expression *m_expr;
    };

    struct StmtPrint : public Statement
    {
        StmtPrint(Expression *expr) : m_expr(expr) {}
        void Accept(StatementVisitor& visitor) const override { visitor.Visit(*this); }
        Expression *m_expr;
    };

}

#endif
