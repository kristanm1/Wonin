#ifndef _WONIN_EXPR_
#define _WONIN_EXPR_

#include "lexeme.hpp"
#include "visitor.hpp"

#include <memory>
#include <vector>
#include <variant>

namespace wonin
{

    struct Lexeme;
    struct Expr
    {
        Expr(usize lineNumber) : m_lineNumber(lineNumber) {}

        virtual       ~Expr() = default;
        virtual Value accept(Visitor const& visitor) const = 0;

        usize lineNumber() const { return m_lineNumber; }

    private:
        usize m_lineNumber;
    };

    using exprptr = std::shared_ptr<Expr>;
    using exprlist = std::vector<exprptr>;

    struct ExprLiteral : Expr
    {
        template<typename T>
        ExprLiteral(T&& data, usize lineNumber)
            : Expr(lineNumber)
            , m_data(std::forward<T>(data))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Value m_data;
    };

    struct ExprGrouping : Expr
    {
        ExprGrouping(exprptr&& expr, usize lineNumber)
            : Expr(lineNumber)
            , m_expr(std::move(expr))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }
        
        exprptr m_expr;
    };

    struct ExprUnary : Expr
    {
        ExprUnary(Token op, exprptr&& expr, usize lineNumber)
            : Expr(lineNumber)
            , m_op(op)
            , m_expr(std::move(expr))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Token   m_op;
        exprptr m_expr;
    };

    struct ExprBinary : Expr
    {
        ExprBinary(Token op, exprptr&& left, exprptr&& right, usize lineNumber)
            : Expr(lineNumber)
            , m_op(op)
            , m_left(std::move(left))
            , m_right(std::move(right))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Token   m_op;
        exprptr m_left;
        exprptr m_right;
    };

    struct ExprVariable : Expr
    {
        ExprVariable(str&& name, usize lineNumber)
            : Expr(lineNumber)
            , m_name(std::move(name))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        str m_name;
    };

    struct ExprAsgn : Expr
    {
        ExprAsgn(str&& name, exprptr&& expr, usize lineNumber)
            : Expr(lineNumber)
            , m_name(std::move(name))
            , m_expr(std::move(expr))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        str     m_name;
        exprptr m_expr;
    };

    struct ExprLogical : Expr
    {
        ExprLogical(Token op, exprptr&& left, exprptr&& right, usize lineNumber)
            : Expr(lineNumber)
            , m_op(op)
            , m_left(std::move(left))
            , m_right(std::move(right))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        Token   m_op;
        exprptr m_left;
        exprptr m_right;
    };

    struct ExprCall : Expr
    {
        ExprCall(exprptr&& callee, std::vector<exprptr>&& arguments, usize lineNumber)
            : Expr(lineNumber)
            , m_callee(std::move(callee))
            , m_arguments(std::move(arguments))
        {}

        Value accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        exprptr  m_callee;
        exprlist m_arguments;
    };

}

#endif
