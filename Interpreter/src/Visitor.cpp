#include "Visitor.hpp"

using namespace Wonin;

char const* ValueError::GetMessage() const
{
    switch (m_type)
    {
        case Type::OpNegTypMismatch: return "Value operator negation type mismatch";
        case Type::OpPluTypMismatch: return "Value operator plus type mismatch";
        case Type::OpMinTypMismatch: return "Value operator minus type mismatch";
        case Type::OpMulTypMismatch: return "Value operator multiply type mismatch";
        case Type::OpDivTypMismatch: return "Value operator divide type mismatch";
        default: return "Unknown value error";
    }
}

Value Value::operator-() const
{
    if (IsNumber())
        return Value(-GetNumber());
    
    throw ValueError(ValueError::Type::OpNegTypMismatch);
}

Value Value::operator+(Value const& rhs) const
{
    if (IsNumber() && rhs.IsNumber())
        return Value(GetNumber() + rhs.GetNumber());
    
    throw ValueError(ValueError::Type::OpPluTypMismatch);
}

Value Value::operator-(Value const& rhs) const
{
    if (IsNumber() && rhs.IsNumber())
        return Value(GetNumber() - rhs.GetNumber());
    
    throw ValueError(ValueError::Type::OpMinTypMismatch);
}

Value Value::operator*(Value const& rhs) const
{
    if (IsNumber() && rhs.IsNumber())
        return Value(GetNumber() * rhs.GetNumber());
    
    throw ValueError(ValueError::Type::OpMulTypMismatch);
}

Value Value::operator/(Value const& rhs) const
{
    if (IsNumber() && rhs.IsNumber())
        return Value(GetNumber() / rhs.GetNumber());
    
    throw ValueError(ValueError::Type::OpDivTypMismatch);
}

bool Value::IsEmpty() const
{
    return TypeOf<std::monostate>();
}

bool Value::IsNumber() const
{
    return TypeOf<double>();
}

bool Value::IsString() const
{
    return TypeOf<std::string>();
}

double const& Value::GetNumber() const
{
    return GetType<double>();
}

std::string const& Value::GetString() const
{
    return GetType<std::string>();
}
