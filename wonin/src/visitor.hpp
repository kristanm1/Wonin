#ifndef _WONIN_VISITOR_
#define _WONIN_VISITOR_

#include "value.hpp"

namespace wonin
{

    struct ExprLiteral;
    struct ExprGrouping;
    struct ExprVar;
    struct ExprLogical;
    struct ExprUnary;
    struct ExprBinary;
    struct ExprAsgn;

    struct StmtExpr;
    struct StmtPrint;
    struct StmtVarDecl;
    struct StmtBlock;
    struct StmtIf;
    struct StmtWhile;

    struct Visitor
    {
        ~Visitor() = default;

        virtual Value visit(ExprLiteral const& num)  const = 0;
        virtual Value visit(ExprGrouping const& gro) const = 0;
        virtual Value visit(ExprVar const& var)      const = 0;
        virtual Value visit(ExprLogical const& var)  const = 0;
        virtual Value visit(ExprUnary const& una)    const = 0;
        virtual Value visit(ExprBinary const& bin)   const = 0;
        virtual Value visit(ExprAsgn const& var)     const = 0;

        virtual void visit(StmtExpr const& exp)    const = 0;
        virtual void visit(StmtPrint const& exp)   const = 0;
        virtual void visit(StmtVarDecl const& exp) const = 0;
        virtual void visit(StmtBlock const& exp)   const = 0;
        virtual void visit(StmtIf const& exp)      const = 0;
        virtual void visit(StmtWhile const& exp)   const = 0;
    };

}

#endif
