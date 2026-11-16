#ifndef _WONIN_EXPR_
#define _WONIN_EXPR_

#include "Visitor.hpp"

#include <memory>
#include <variant>

namespace Wonin
{

    struct Expr
    {
        virtual ~Expr() = default;
        virtual Value Accept(Visitor const& visitor) const = 0;
    };

    using ExprPtr = std::unique_ptr<Expr>;

    struct ExprLiteral : public Expr
    {
        template<typename T>
        ExprLiteral(T&& data)
            : m_data(std::forward<T>(data))
        {}

        Value Accept(Visitor const& visitor) const override
        {
            return visitor.Visit(*this);
        }

        Value m_data;
    };

    struct ExprGrouping : public Expr
    {
        ExprGrouping(ExprPtr&& expr)
            : m_expr(std::move(expr))
        {}

        Value Accept(Visitor const& visitor) const override
        {
            return visitor.Visit(*this);
        }
        ExprPtr m_expr;
    };

    struct ExprUnary : public Expr
    {
        ExprUnary(Token const& token, ExprPtr&& expr)
            : m_token(token)
            , m_expr(std::move(expr))
        {}

        Value Accept(Visitor const& visitor) const override
        {
            return visitor.Visit(*this);
        }

        Token const& m_token;
        ExprPtr m_expr;
    };

    struct ExprBinary : public Expr
    {
        ExprBinary(ExprPtr&& left, Token const& token, ExprPtr&& right)
            : m_token(token)
            , m_left(std::move(left))
            , m_right(std::move(right))
        {}

        Value Accept(Visitor const& visitor) const override
        {
            return visitor.Visit(*this);
        }

        Token const& m_token;
        ExprPtr m_left;
        ExprPtr m_right;
    };

}

#endif
