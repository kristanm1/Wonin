#ifndef _WONIN_VISITOR_
#define _WONIN_VISITOR_

namespace Wonin
{

    struct Number;
    struct Grouping;
    struct Unary;
    struct Binary;

    struct ExpressionVisitor
    {
        ~ExpressionVisitor() = default;

        virtual void VisitNumber(Number const& num) = 0;
        virtual void VisitGrouping(Grouping const& gro) = 0;
        virtual void VisitUnary(Unary const& una) = 0;
        virtual void VisitBinary(Binary const& bin) = 0;
    };

    struct Interpreter : ExpressionVisitor
    {
        double result;

        void VisitNumber(Number const& num) override;
        void VisitGrouping(Grouping const& gro) override;
        void VisitUnary(Unary const& una) override;
        void VisitBinary(Binary const& bin) override;
    };

}

#endif
