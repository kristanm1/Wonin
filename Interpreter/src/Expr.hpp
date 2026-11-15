#ifndef _WONIN_EXPR_
#define _WONIN_EXPR_

#include "Scanner.hpp"
#include "Visitor.hpp"
#include "Log.hpp"

#include <memory>
#include <variant>

namespace Wonin
{

    struct Expr
    {
        virtual ~Expr() = default;
        virtual Value Accept(ExpressionVisitor& visitor) const = 0;
    };

    using ExprPtr = std::unique_ptr<Expr>;

    struct ExprNumber : public Expr
    {
        ExprNumber(double num) : m_num(num) {}
        Value Accept(ExpressionVisitor& visitor) const override
        {
            ASSERT(&visitor);
            return visitor.Visit(*this);
        }
        double m_num;
    };

    struct ExprGrouping : public Expr
    {
        ExprGrouping(ExprPtr&& expr) : m_expr(std::move(expr)) {}
        Value Accept(ExpressionVisitor& visitor) const override
        {
            ASSERT(&visitor);
            return visitor.Visit(*this);
        }
        ExprPtr m_expr;
    };

    struct ExprUnary : public Expr
    {
        ExprUnary(Token::Type op, ExprPtr&& expr) : m_op(op) , m_expr(std::move(expr)) {}
        Value Accept(ExpressionVisitor& visitor) const override
        {
            ASSERT(&visitor);
            return visitor.Visit(*this);
        }
        Token::Type m_op;
        ExprPtr m_expr;
    };

    struct ExprBinary : public Expr
    {
        ExprBinary(ExprPtr&& left, Token::Type op, ExprPtr&& right)
            : m_op(op) , m_left(std::move(left)) , m_right(std::move(right)) {}
        Value Accept(ExpressionVisitor& visitor) const override
        {
            ASSERT(&visitor);
            return visitor.Visit(*this);
        }
        Token::Type m_op;
        ExprPtr m_left;
        ExprPtr m_right;
    };

}

#endif
