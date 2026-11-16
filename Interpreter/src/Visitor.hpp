#ifndef _WONIN_VISITOR_
#define _WONIN_VISITOR_

#include "Types.hpp"

#include <string>
#include <variant>

namespace Wonin
{

    struct ExprLiteral;
    struct ExprGrouping;
    struct ExprUnary;
    struct ExprBinary;
    struct StmtExpr;
    struct StmtPrint;

    class ValueError : public RuntimeError
    {
    public:
        enum class Type
        {
            OpNegTypMismatch,
            OpNotTypMismatch,

            OpEqTypEmpty,
            OpNeqTypEmpty,
            OpLesEmpty,
            OpLesEqTypEmpty,
            OpGrtTypEmpty,
            OpGrtEqTypEmpty,

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

        template<typename T>
        Value(T&& data) : m_data(std::forward<T>(data)) {}

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

        virtual Value Visit(ExprLiteral const& num) = 0;
        virtual Value Visit(ExprGrouping const& gro) = 0;
        virtual Value Visit(ExprUnary const& una) = 0;
        virtual Value Visit(ExprBinary const& bin) = 0;

        virtual void Visit(StmtExpr const& exp) = 0;
        virtual void Visit(StmtPrint const& exp) = 0;
    };

    class InterpreterError : RuntimeError
    {
    public:
        enum class Type
        {
            UnkUnaOp,
            UnkBinOp,
        };

        InterpreterError(Type type) : m_type(type) {}
        std::string GetMessage() const;
    private:
        Type m_type;
    };

    struct Interpreter : Visitor
    {
        Value Visit(ExprLiteral const& expr) override;
        Value Visit(ExprGrouping const& expr) override;
        Value Visit(ExprUnary const& expr) override;
        Value Visit(ExprBinary const& expr) override;

        void Visit(StmtExpr const& stmt) override;
        void Visit(StmtPrint const& stmt) override;
    };

}

#endif
