#ifndef _WONIN_VISITOR_
#define _WONIN_VISITOR_

#include "types.hpp"
#include "type_traits.hpp"

#include <string>
#include <variant>
#include <unordered_map>

namespace wonin
{

    struct ExprLiteral;
    struct ExprGrouping;
    struct ExprVar;
    struct ExprUnary;
    struct ExprBinary;
    struct ExprAsgn;

    struct StmtExpr;
    struct StmtPrint;
    struct StmtVarDecl;
    struct StmtBlock;

    class Value
    {
    public:
        Value() : m_data(std::monostate{}) {}

        template<typename T, typename = EnableIfType<
            !IsSame<
                RemoveCVRefType<T>,
                Value
            >::Value>
        >
        Value(T&& data) : m_data(std::forward<T>(data)) {}

        Value(Value const& other) : m_data(other.m_data)            {}
        Value(Value&& other)      : m_data(std::move(other.m_data)) {}

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

        bool isEmpty()  const { return isTypeOf<std::monostate>(); }
        bool isNull()   const { return isTypeOf<std::nullptr_t>(); }
        bool isBool()   const { return isTypeOf<bool>(); }
        bool isNumber() const { return isTypeOf<double>(); }
        bool isString() const { return isTypeOf<str>(); }

        bool       getBool()   const { return getType<bool>(); }
        double     getNumber() const { return getType<double>(); }
        str const& getString() const { return getType<str>(); }

    private:
        template<typename T>
        bool isTypeOf() const { return std::holds_alternative<T>(m_data); }

        template<typename T>
        T const& getType() const { return std::get<T>(m_data); }
        
        std::variant<std::monostate, std::nullptr_t, bool, double, str> m_data;
    };

    struct Visitor
    {
        ~Visitor() = default;

        virtual Value visit(ExprLiteral const& num)  const = 0;
        virtual Value visit(ExprGrouping const& gro) const = 0;
        virtual Value visit(ExprVar const& var)      const = 0;
        virtual Value visit(ExprUnary const& una)    const = 0;
        virtual Value visit(ExprBinary const& bin)   const = 0;
        virtual Value visit(ExprAsgn const& var)     const = 0;

        virtual void visit(StmtExpr const& exp)    const = 0;
        virtual void visit(StmtPrint const& exp)   const = 0;
        virtual void visit(StmtVarDecl const& exp) const = 0;
        virtual void visit(StmtBlock const& exp)   const = 0;
    };

}

#endif
