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
        friend class CallableFunction;
    public:
        Interpreter();
        Interpreter(Environment* parent);

        ~Interpreter();
        
        bool interpret(Parser const& parser) const;

        Value visit(ExprLiteral const& literal)   const override;
        Value visit(ExprGrouping const& group)    const override;
        Value visit(ExprVariable const& variable) const override;
        Value visit(ExprLogical const& logical)   const override;
        Value visit(ExprUnary const& unary)       const override;
        Value visit(ExprBinary const& binary)     const override;
        Value visit(ExprAsgn const& assign)       const override;
        Value visit(ExprCall const& call)         const override;

        void visit(StmtExpr const& stmt)       const override;
        void visit(StmtPrint const& stmt)      const override;
        void visit(StmtReturn const& stmt)     const override;
        void visit(StmtVarDecl const& stmt)    const override;
        void visit(StmtFunDecl const& funtion) const override;
        void visit(StmtBlock const& stmt)      const override;
        void visit(StmtIf const& stmt)         const override;
        void visit(StmtWhile const& stmt)      const override;

        void executeBlock(std::vector<stmtptr> const& stmts) const;

        Environment& m_environment;
    };

}

#endif
