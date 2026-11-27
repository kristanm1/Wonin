#ifndef _WONIN_VALUE_
#define _WONIN_VALUE_

#include "types.hpp"

#include <variant>
#include <memory>

namespace wonin
{

    class Callable;

    class Value
    {
    public:
        Value()                                 : m_data(std::monostate{})        {}
        Value(Value const& other)               : m_data(other.m_data)            {}
        Value(Value&& other)                    : m_data(std::move(other.m_data)) {}
        Value(std::nullptr_t data)              : m_data(data)                    {}
        Value(bool data)                        : m_data(data)                    {}
        Value(double data)                      : m_data(data)                    {}
        Value(str data)                         : m_data(data)                    {}
        Value(std::shared_ptr<Callable>&& data) : m_data(std::move(data))         {}

        Value& operator=(Value const& other)
        {
            m_data = other.m_data;
            return *this;
        }

        Value& operator=(Value&& other)
        {
            if (this == &other)
                return *this;
            
            m_data = std::move(other.m_data);
            return *this;
        }

        Value operator-() const;
        Value operator!() const;

        Value operator==(Value const& rhs) const;
        Value operator!=(Value const& rhs) const;
        Value operator<(Value const& rhs)  const;
        Value operator<=(Value const& rhs) const;
        Value operator>(Value const& rhs)  const;
        Value operator>=(Value const& rhs) const;

        Value operator+(Value const& rhs) const;
        Value operator-(Value const& rhs) const;
        Value operator*(Value const& rhs) const;
        Value operator/(Value const& rhs) const;

        bool isEmpty()    const { return isTypeOf<std::monostate>(); }
        bool isNull()     const { return isTypeOf<std::nullptr_t>(); }
        bool isBool()     const { return isTypeOf<bool>(); }
        bool isNumber()   const { return isTypeOf<double>(); }
        bool isString()   const { return isTypeOf<str>(); }
        bool isCallable() const { return isTypeOf<std::shared_ptr<Callable>>(); }

        bool       getBool()     const;
        double     getNumber()   const { return getType<double>(); }
        str const& getString()   const { return getType<str>(); }
        Callable&  getCallable() const { return *getType<std::shared_ptr<Callable>>().get(); }

    private:
        template<typename T>
        bool isTypeOf() const { return std::holds_alternative<T>(m_data); }

        template<typename T>
        T const& getType() const { return std::get<T>(m_data); }
        
        std::variant<
            std::monostate,
            std::nullptr_t,
            bool,
            double,
            str,
            std::shared_ptr<Callable>
        > m_data;
    };

}

#endif
