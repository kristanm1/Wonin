#include "Visitor.hpp"
#include "Scanner.hpp"
#include "Expr.hpp"

using namespace Wonin;

char const* InterpreterError::GetMessage() const
{
    switch (m_type)
    {
        case Type::UnkUnaOp: return "Unknown unary operator";
        case Type::UnkBinOp: return "Unknown binary operator";
        default: return "Unknown interpreter error";
    }
}

Value Interpreter::Visit(ExprNumber const& expr)
{
    ASSERT(&expr);
    return Value(expr.m_num);
}

Value Interpreter::Visit(ExprGrouping const& expr)
{
    ASSERT(&expr);
    return expr.m_expr->Accept(*this);
}

Value Interpreter::Visit(ExprUnary const& expr)
{
    ASSERT(&expr);
    Value value = expr.m_expr->Accept(*this);
    if (expr.m_op == Token::Type::Minus)
        return -value;
    else
        throw InterpreterError(InterpreterError::Type::UnkUnaOp);
}

Value Interpreter::Visit(ExprBinary const& expr)
{
    ASSERT(&expr);
    Value left = expr.m_left->Accept(*this);
    Value right = expr.m_right->Accept(*this);
    if (expr.m_op == Token::Type::Plus)
        return left + right;
    else if (expr.m_op == Token::Type::Minus)
        return left - right;
    else if (expr.m_op == Token::Type::Star)
        return left * right;
    else if (expr.m_op == Token::Type::Slash)
        return left / right;
    
    throw Wonin::InterpreterError(Wonin::InterpreterError::Type::UnkBinOp);
}
