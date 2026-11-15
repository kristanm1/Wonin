#ifndef _WONIN_VISITOR_
#define _WONIN_VISITOR_

#include "Types.hpp"

#include <string>
#include <variant>

namespace Wonin
{

    struct ExprNumber;
    struct ExprGrouping;
    struct ExprUnary;
    struct ExprBinary;
    struct StmtExpression;
    struct StmtPrint;

    class ValueError : public RuntimeError
    {
    public:
        enum class Type
        {
            OpNegTypMismatch,

            OpPluTypMismatch,
            OpMinTypMismatch,
            OpMulTypMismatch,
            OpDivTypMismatch,
        };

        ValueError(Type type) : m_type(type) {}
        char const* GetMessage() const override;
    private:
        Type m_type;
    };

    class Value
    {
    public:
        Value() : m_data(std::monostate{}) {}
        Value(double d) : m_data(d) {}
        Value(std::string s) : m_data(std::move(s)) {}

        Value operator-() const;

        Value operator+(Value const& rhs) const;
        Value operator-(Value const& rhs) const;
        Value operator*(Value const& rhs) const;
        Value operator/(Value const& rhs) const;

        bool IsEmpty() const;
        bool IsNumber() const;
        bool IsString() const;

        double const& GetNumber() const;
        std::string const& GetString() const;

    private:
        template<typename T>
        bool TypeOf() const { return std::holds_alternative<T>(m_data); }

        template<typename T>
        T const& GetType() const { return std::get<T>(m_data); }
        
        std::variant<std::monostate, double, std::string> m_data;
    };

    struct ExpressionVisitor
    {
        ~ExpressionVisitor() = default;

        virtual Value Visit(ExprNumber const& num) = 0;
        virtual Value Visit(ExprGrouping const& gro) = 0;
        virtual Value Visit(ExprUnary const& una) = 0;
        virtual Value Visit(ExprBinary const& bin) = 0;
    };

    struct StatementVisitor
    {
        ~StatementVisitor() = default;

        virtual void Visit(StmtExpression const& exp) = 0;
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
        char const* GetMessage() const;
    private:
        Type m_type;
    };

    struct Interpreter : ExpressionVisitor
    {
        Value Visit(ExprNumber const& num) override;
        Value Visit(ExprGrouping const& gro) override;
        Value Visit(ExprUnary const& una) override;
        Value Visit(ExprBinary const& bin) override;
    };

}

#endif
