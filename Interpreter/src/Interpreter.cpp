#include "Visitor.hpp"
#include "Scanner.hpp"
#include "Log.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

using namespace Wonin;

std::string InterpreterError::GetMessage() const
{
    switch (m_type)
    {
        case Type::UnkUnaOp: return "Unknown unary operator";
        case Type::UnkBinOp: return "Unknown binary operator";
        default: return "Unknown interpreter error";
    }
}

Value Interpreter::Visit(ExprLiteral const& expr) const
{
    return expr.m_data;
}

Value Interpreter::Visit(ExprGrouping const& expr) const
{
    return expr.m_expr->Accept(*this);
}

Value Interpreter::Visit(ExprUnary const& expr) const
{
    Value value = expr.m_expr->Accept(*this);
    if (expr.m_token.GetType() == Token::Type::Minus)
        return -value;
    else
        throw InterpreterError(InterpreterError::Type::UnkUnaOp);
}

Value Interpreter::Visit(ExprBinary const& expr) const
{
    Value left = expr.m_left->Accept(*this);
    Value right = expr.m_right->Accept(*this);
    if (expr.m_token.GetType() == Token::Type::Plus)
        return left + right;
    else if (expr.m_token.GetType() == Token::Type::Minus)
        return left - right;
    else if (expr.m_token.GetType() == Token::Type::Star)
        return left * right;
    else if (expr.m_token.GetType() == Token::Type::Slash)
        return left / right;
    
    throw Wonin::InterpreterError(Wonin::InterpreterError::Type::UnkBinOp);
}

void Interpreter::Visit(StmtExpr const& stmt) const
{
    stmt.m_expr->Accept(*this);
}

void Interpreter::Visit(StmtPrint const& stmt) const
{
    Value value = stmt.m_expr->Accept(*this);
    if (value.IsNumber())
    {
        Log::Info(value.GetNumber());
    }
}