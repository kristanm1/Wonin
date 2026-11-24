#ifndef _WONIN_INTERPRETER_
#define _WONIN_INTERPRETER_

#include "visitor.hpp"
#include "parser.hpp"
#include "environment.hpp"

#include <vector>

namespace wonin
{

    struct Parser;

    class Interpreter : public Visitor
    {
    public:
        Interpreter(Parser const& parser) : m_parser(parser) {}

        bool interpret(Environment& environment) const;

    private:
        Value visit(ExprLiteral const& expr)  const override;
        Value visit(ExprGrouping const& expr) const override;
        Value visit(ExprVar const& expr)      const override;
        Value visit(ExprLogical const& expr)  const override;
        Value visit(ExprUnary const& expr)    const override;
        Value visit(ExprBinary const& expr)   const override;
        Value visit(ExprAsgn const& expr)     const override;

        void visit(StmtExpr const& stmt)    const override;
        void visit(StmtPrint const& stmt)   const override;
        void visit(StmtVarDecl const& stmt) const override;
        void visit(StmtBlock const& stmt)   const override;
        void visit(StmtIf const& stmt)      const override;
        void visit(StmtWhile const& stmt)   const override;

        void executeBlock(std::vector<StmtPtr> const& stmts, Environment& environment) const;

        Parser const& m_parser;
        mutable Environment* m_environment = nullptr;
    };

}

#endif
