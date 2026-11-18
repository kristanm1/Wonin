#ifndef _WONIN_VISITOR_
#define _WONIN_VISITOR_

#include "types.hpp"
#include "type_traits.hpp"

#include <string>
#include <variant>
#include <unordered_map>

namespace Wonin
{

    struct ExprLiteral;
    struct ExprGrouping;
    struct ExprUnary;
    struct ExprBinary;
    struct ExprVar;

    struct StmtExpr;
    struct StmtPrint;
    struct StmtVarDecl;

    class ValueError : public RuntimeError
    {
    public:
        enum class Type
        {
            OpNegTypMismatch,
            OpNotTypMismatch,

            OpEqTypEmpty,
            OpNeqTypEmpty,

            OpLesTypNotSup,
            OpLesEqTypNotSup,
            OpGrtTypNotSup,
            OpGrtEqTypNotSup,

            OpPluTypMismatch,
            OpMinTypMismatch,
            OpMulTypMismatch,
            OpDivTypMismatch,
            OpDivByZero,
        };

        ValueError(Type type) : m_type(type) {}
        std::string GetMessage() const override;
    private:
        Type m_type;
    };

    class Value
    {
    public:
        Value() : m_data(std::monostate{}) {}

        template<typename T, EnableIfType<
            !IsSame<
                RemoveCVRefType<T>,
                Value
            >::Value>* = nullptr
        >
        Value(T&& data) : m_data(std::forward<T>(data)) {}

        Value(Value const& other) : m_data(other.m_data) {}
        Value(Value&& other) : m_data(std::move(other.m_data)) {}

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
        Value operator<(Value const& rhs) const;
        Value operator<=(Value const& rhs) const;
        Value operator>(Value const& rhs) const;
        Value operator>=(Value const& rhs) const;

        Value operator+(Value const& rhs) const;
        Value operator-(Value const& rhs) const;
        Value operator*(Value const& rhs) const;
        Value operator/(Value const& rhs) const;

        bool IsEmpty() const;
        bool IsNull() const;
        bool IsBool() const;
        bool IsNumber() const;
        bool IsString() const;

        bool GetBool() const;
        double GetNumber() const;
        std::string const& GetString() const;

    private:
        template<typename T>
        bool IsTypeOf() const { return std::holds_alternative<T>(m_data); }

        template<typename T>
        T const& GetType() const { return std::get<T>(m_data); }
        
        std::variant<std::monostate, std::nullptr_t, bool, double, std::string> m_data;
    };

    struct Visitor
    {
        ~Visitor() = default;

        virtual Value Visit(ExprLiteral const& num) const = 0;
        virtual Value Visit(ExprGrouping const& gro) const = 0;
        virtual Value Visit(ExprUnary const& una) const = 0;
        virtual Value Visit(ExprBinary const& bin) const = 0;
        virtual Value Visit(ExprVar const& var) const = 0;

        virtual void Visit(StmtExpr const& exp) const = 0;
        virtual void Visit(StmtPrint const& exp) const = 0;
        virtual void Visit(StmtVarDecl const& exp) const = 0;
    };

}

#endif
