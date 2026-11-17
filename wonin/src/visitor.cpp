#include "visitor.hpp"

using namespace Wonin;

std::string ValueError::GetMessage() const
{
    switch (m_type)
    {
        case Type::OpNegTypMismatch: return "Value operator negation type mismatch";
        case Type::OpNotTypMismatch: return "Value operator not type mismatch";

        case Type::OpEqTypEmpty:     return "Value operator equal type is empty";
        case Type::OpNeqTypEmpty:    return "Value operator not equal type is empty";
        case Type::OpLesEmpty:       return "Value operator equal type is empty";
        case Type::OpLesEqTypEmpty:  return "Value operator not equal type is empty";
        case Type::OpGrtTypEmpty:    return "Value operator equal type is empty";
        case Type::OpGrtEqTypEmpty:  return "Value operator not equal type is empty";

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

Value Value::operator!() const
{
    if (IsBool())
        return Value(!GetBool());

    throw ValueError(ValueError::Type::OpNotTypMismatch);
}

Value Value::operator==(Value const& rhs) const
{
    if (IsEmpty())
        throw ValueError(ValueError::Type::OpEqTypEmpty);
    
    return m_data == rhs.m_data;
}

Value Value::operator!=(Value const& rhs) const
{
    if (IsEmpty())
        throw ValueError(ValueError::Type::OpNeqTypEmpty);
    
    return m_data != rhs.m_data;
}

Value Value::operator<(Value const& rhs) const
{
    return {};
}

Value Value::operator<=(Value const& rhs) const
{
    return {};
}

Value Value::operator>(Value const& rhs) const
{
    return {};
}

Value Value::operator>=(Value const& rhs) const
{
    return {};
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
    {
        if (rhs.GetNumber() == 0.0)
            throw ValueError(ValueError::Type::OpDivByZero);
        else
            return Value(GetNumber() / rhs.GetNumber());
    }
    
    throw ValueError(ValueError::Type::OpDivTypMismatch);
}

bool Value::IsEmpty() const
{
    return IsTypeOf<std::monostate>();
}

bool Value::IsBool() const
{
    return IsTypeOf<bool>();
}

bool Value::IsNumber() const
{
    return IsTypeOf<double>();
}

bool Value::IsString() const
{
    return IsTypeOf<std::string>();
}

bool Value::GetBool() const
{
    return GetType<bool>();
}

double Value::GetNumber() const
{
    return GetType<double>();
}

std::string const& Value::GetString() const
{
    return GetType<std::string>();
}
