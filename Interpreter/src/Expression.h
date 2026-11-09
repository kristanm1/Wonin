#ifndef _WONIN_EXPRESSION_
#define _WONIN_EXPRESSION_

#include "Scanner.h"
#include "Visitor.h"

namespace Wonin
{

    struct Expression
    {
        virtual ~Expression() = default;
        virtual void Accept(ExpressionVisitor& visitor) const = 0;
    };

    struct Number : public Expression
    {
        Number(double num)
            : m_num(num) {}
        
        void Accept(ExpressionVisitor& visitor) const override
        {
            visitor.VisitNumber(*this);
        }

        double m_num;
    };

    struct Grouping : public Expression
    {
        Grouping(Expression *expr)
            : m_expr(expr)
        {}

        ~Grouping()
        {
            delete m_expr;
        }

        void Accept(ExpressionVisitor& visitor) const override
        {
            visitor.VisitGrouping(*this);
        }
        
        Expression *m_expr;
    };

    struct Unary : public Expression
    {
        Unary(Token::Type op, Expression *expr)
            : m_op(op)
            , m_expr(expr)
        {}

        ~Unary()
        {
            delete m_expr;
        }

        void Accept(ExpressionVisitor& visitor) const override
        {
            visitor.VisitUnary(*this);
        }

        Token::Type m_op;
        Expression *m_expr;
    };

    struct Binary : public Expression
    {
        Binary(Expression *left, Token::Type op, Expression *right)
            : m_op(op)
            , m_left(left)
            , m_right(right)
        {}

        ~Binary()
        {
            delete m_left;
            delete m_right;
        }

        void Accept(ExpressionVisitor& visitor) const override
        {
            visitor.VisitBinary(*this);
        }

        Token::Type m_op;
        Expression *m_left;
        Expression *m_right;
    };

}

#endif
