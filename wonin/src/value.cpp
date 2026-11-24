#include "value.hpp"
#include "utils.hpp"

using namespace wonin;

Value Value::operator-() const
{
    if (isNumber())
        return Value(-getNumber());
    
    throw RuntimeError("Unary- used on invalid type");
}

Value Value::operator!() const
{
    if (isBool())
        return Value(!getBool());

    throw RuntimeError("Unary! used on invalid type");
}

Value Value::operator==(Value const& rhs) const
{
    if (isEmpty() || rhs.isEmpty())
        throw RuntimeError("Binary== used on void type");
    
    return m_data == rhs.m_data;
}

Value Value::operator!=(Value const& rhs) const
{
    if (isEmpty() || rhs.isEmpty())
        throw RuntimeError("Binary!= used on void type");
    
    return m_data != rhs.m_data;
}

Value Value::operator<(Value const& rhs) const
{
    if (isNumber() && rhs.isNumber())
        return Value(getNumber() < rhs.getNumber());
    
    throw RuntimeError("Binary< used on invalid type");
}

Value Value::operator<=(Value const& rhs) const
{
    if (isNumber() && rhs.isNumber())
        return Value(getNumber() <= rhs.getNumber());
    
    throw RuntimeError("Binary<= used on invalid type");
}

Value Value::operator>(Value const& rhs) const
{
    if (isNumber() && rhs.isNumber())
        return Value(getNumber() > rhs.getNumber());
    
    throw RuntimeError("Binary> used on invalid type");
}

Value Value::operator>=(Value const& rhs) const
{
    if (isNumber() && rhs.isNumber())
        return Value(getNumber() >= rhs.getNumber());
    
    throw RuntimeError("Binary>= used on invalid type");
}

Value Value::operator+(Value const& rhs) const
{
    if (isNumber() && rhs.isNumber())
        return Value(getNumber() + rhs.getNumber());
    
    if (isString() && rhs.isString())
        return Value(getString() + rhs.getString());
    
    throw RuntimeError("Binary+ used on invalid type");
}

Value Value::operator-(Value const& rhs) const
{
    if (isNumber() && rhs.isNumber())
        return Value(getNumber() - rhs.getNumber());
    
    throw RuntimeError("Binary- used on invalid type");
}

Value Value::operator*(Value const& rhs) const
{
    if (isNumber() && rhs.isNumber())
        return Value(getNumber() * rhs.getNumber());
    
    throw RuntimeError("Binary* used on invalid type");
}

Value Value::operator/(Value const& rhs) const
{
    if (isNumber() && rhs.isNumber())
    {
        if (rhs.getNumber() == 0.0)
            throw RuntimeError("Division by zero");
        else
            return Value(getNumber() / rhs.getNumber());
    }
    
    throw RuntimeError("Binary/ used on invalid type");
}

bool Value::getBool() const
{
    if (isBool())
        return getType<bool>();

    return false;    
}
