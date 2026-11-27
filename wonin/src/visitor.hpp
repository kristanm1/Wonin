#ifndef _WONIN_VISITOR_
#define _WONIN_VISITOR_

#include "value.hpp"

namespace wonin
{

    struct ExprLiteral;
    struct ExprGrouping;
    struct ExprVariable;
    struct ExprLogical;
    struct ExprUnary;
    struct ExprBinary;
    struct ExprAsgn;
    struct ExprCall;

    struct StmtExpr;
    struct StmtPrint;
    struct StmtReturn;
    struct StmtVarDecl;
    struct StmtFunDecl;
    struct StmtBlock;
    struct StmtIf;
    struct StmtWhile;

    struct Visitor
    {
        virtual ~Visitor() = default;

        virtual Value visit(ExprLiteral const& literal)   const = 0;
        virtual Value visit(ExprGrouping const& group)    const = 0;
        virtual Value visit(ExprVariable const& variable) const = 0;
        virtual Value visit(ExprLogical const& logical)   const = 0;
        virtual Value visit(ExprUnary const& unary)       const = 0;
        virtual Value visit(ExprBinary const& binary)     const = 0;
        virtual Value visit(ExprAsgn const& assign)       const = 0;
        virtual Value visit(ExprCall const& call)         const = 0;

        virtual void visit(StmtExpr const& exp)        const = 0;
        virtual void visit(StmtPrint const& exp)       const = 0;
        virtual void visit(StmtReturn const& exp)      const = 0;
        virtual void visit(StmtVarDecl const& exp)     const = 0;
        virtual void visit(StmtFunDecl const& funtion) const = 0;
        virtual void visit(StmtBlock const& exp)       const = 0;
        virtual void visit(StmtIf const& exp)          const = 0;
        virtual void visit(StmtWhile const& exp)       const = 0;
    };

}

#endif
