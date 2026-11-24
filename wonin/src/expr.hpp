#ifndef _WONIN_EXPR_
#define _WONIN_EXPR_

#include "visitor.hpp"

#include <memory>
#include <variant>

namespace wonin
{

    struct Expr
    {
        virtual       ~Expr() = default;
        virtual Value accept(Visitor const& visitor) const = 0;
    };

    using ExprPtr = std::unique_ptr<Expr>;

    struct ExprLiteral : Expr
    {
        template<typename T>
        ExprLiteral(T&& data) : m_data(std::forward<T>(data)) {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Value m_data;
    };

    struct ExprGrouping : Expr
    {
        ExprGrouping(ExprPtr&& expr) : m_expr(std::move(expr)) {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }
        
        ExprPtr m_expr;
    };

    struct ExprUnary : Expr
    {
        ExprUnary(Lexeme const& op, ExprPtr&& expr)
            : m_op(op)
            , m_expr(std::move(expr))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Lexeme const& m_op;
        ExprPtr m_expr;
    };

    struct ExprBinary : Expr
    {
        ExprBinary(Lexeme const& op, ExprPtr&& left, ExprPtr&& right)
            : m_op(op)
            , m_left(std::move(left))
            , m_right(std::move(right))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Lexeme const& m_op;
        ExprPtr m_left;
        ExprPtr m_right;
    };

    struct ExprVar : Expr
    {
        ExprVar(Lexeme const& lexeme) : m_lexeme(lexeme) {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Lexeme const& m_lexeme;
    };

    struct ExprAsgn : Expr
    {
        ExprAsgn(Lexeme lexeme, ExprPtr&& expr)
            : m_lexeme(lexeme)
            , m_expr(std::move(expr))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Lexeme m_lexeme;
        ExprPtr m_expr;
    };

    struct ExprLogical : Expr
    {
        ExprLogical(Lexeme const& op, ExprPtr&& left, ExprPtr&& right)
            : m_op(op)
            , m_left(std::move(left))
            , m_right(std::move(right))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Lexeme const& m_op;
        ExprPtr       m_left;
        ExprPtr       m_right;
    };
}

#endif
