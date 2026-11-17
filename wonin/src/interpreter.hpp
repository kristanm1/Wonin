#ifndef _WONIN_INTERPRETER_
#define _WONIN_INTERPRETER_

#include "visitor.hpp"

#include <vector>
#include <memory>

namespace Wonin
{

    struct Stmt;
    using StmtPtr = std::unique_ptr<Stmt>;

    class Interpreter : public Visitor
    {
    public:
        void Interpret(std::vector<StmtPtr> const& stmts) const;

    private:
        Value Visit(ExprLiteral const& expr) const override;
        Value Visit(ExprGrouping const& expr) const override;
        Value Visit(ExprUnary const& expr) const override;
        Value Visit(ExprBinary const& expr) const override;

        void Visit(StmtExpr const& stmt) const override;
        void Visit(StmtPrint const& stmt) const override;
    };

}

#endif
