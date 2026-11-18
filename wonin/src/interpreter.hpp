#ifndef _WONIN_INTERPRETER_
#define _WONIN_INTERPRETER_

#include "visitor.hpp"
#include "parser.hpp"

namespace Wonin
{

    struct Parser;

        class InterpreterError : RuntimeError
    {
    public:
        enum class Type
        {
            UnkUnaOp,
            UnkBinOp,
            UnkId,
        };

        InterpreterError(Type type) : m_type(type) {}
        std::string GetMessage() const;
    private:
        Type m_type;
    };

    class Interpreter : public Visitor
    {
    public:
        Interpreter(Parser const& parser) : m_parser(parser) {}
        void Interpret() const;

    private:
        Value Visit(ExprLiteral const& expr) const override;
        Value Visit(ExprGrouping const& expr) const override;
        Value Visit(ExprUnary const& expr) const override;
        Value Visit(ExprBinary const& expr) const override;
        Value Visit(ExprVar const& expr) const override;

        void Visit(StmtExpr const& stmt) const override;
        void Visit(StmtPrint const& stmt) const override;
        void Visit(StmtVarDecl const& stmt) const override;

        Parser const& m_parser;
        mutable std::unordered_map<std::string, Value> m_environment{};
    };

}

#endif
