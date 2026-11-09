#include "Visitor.h"
#include "Expression.h"
#include "Log.h"

void Wonin::Interpreter::VisitNumber(Number const& num)
{
    result = num.m_num;
}

void Wonin::Interpreter::VisitGrouping(Grouping const& gro)
{
    gro.m_expr->Accept(*this);
}

void Wonin::Interpreter::VisitUnary(Unary const& una)
{
    una.m_expr->Accept(*this);

    double value = result;
    switch (una.m_op)
    {
        case Token::Type::Minus:
            result = -value;
            break;
        default:
            Error("Unknown unary operator!");
    }
}

void Wonin::Interpreter::VisitBinary(Binary const& bin)
{
    bin.m_left->Accept(*this);
    double left = result;

    bin.m_right->Accept(*this);
    double right = result;

    switch (bin.m_op)
    {
        case Token::Type::Plus:
            result = left + right;
            break;
        case Token::Type::Minus:
            result = left - right;
            break;
        case Token::Type::Star:
            result = left * right;
            break;
        case Token::Type::Slash:
            result = left / right;
            break;
    }
}
