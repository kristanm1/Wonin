#ifndef _WONIN_STATEMENT_
#define _WONIN_STATEMENT_

#include "types.hpp"
#include "expr.hpp"
#include "visitor.hpp"

#include <memory>
#include <vector>

namespace wonin
{

    struct Lexeme;
    struct Stmt
    {
        Stmt(usize lineNumber) : m_lineNumber(lineNumber) {}

        virtual      ~Stmt()                              = default;
        virtual void accept(Visitor const& visitor) const = 0;

        usize lineNumber() { return m_lineNumber; }

    private:
        usize m_lineNumber;
    };

    using stmtptr  = std::shared_ptr<Stmt>;
    using stmtlist = std::vector<stmtptr>;
    using strlist  = std::vector<str>;

    struct StmtExpr : Stmt
    {
        StmtExpr(exprptr&& expr, usize lineNumber)
            : Stmt(lineNumber)
            , m_expr(std::move(expr))
        {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        exprptr m_expr;
    };

    struct StmtPrint : Stmt
    {
        StmtPrint(exprptr&& expr, usize lineNumber)
            : Stmt(lineNumber)
            , m_expr(std::move(expr))
        {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        exprptr m_expr;
    };

    struct StmtReturn : Stmt
    {
        StmtReturn(exprptr&& expr, usize lineNumber)
            : Stmt(lineNumber)
            , m_expr(std::move(expr))
        {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        exprptr m_expr;
    };

    struct StmtVarDecl : Stmt
    {
        StmtVarDecl(str&& name, exprptr&& initializer, usize lineNumber)
            : Stmt(lineNumber)
            , m_name(name)
            , m_initializer(std::move(initializer))
        {}
        
        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        str     m_name;
        exprptr m_initializer;
    };

    struct StmtFunDecl : Stmt
    {
        StmtFunDecl(str&& name, strlist&& parameters, stmtlist&& body, usize lineNumber)
            : Stmt(lineNumber)
            , m_name(std::move(name))
            , m_parameters(std::move(parameters))
            , m_body(std::move(body))
        {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }
        
        str      m_name;
        strlist  m_parameters;
        stmtlist m_body;
    };

    struct StmtBlock : Stmt
    {
        StmtBlock(stmtlist&& stmts, usize lineNumber)
            : Stmt(lineNumber)
            , m_stmts(std::move(stmts))
        {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        stmtlist m_stmts;
    };

    struct StmtIf : Stmt
    {
        StmtIf(exprptr&& condition, stmtptr&& stmtThen, stmtptr&& stmtElse, usize lineNumber)
            : Stmt(lineNumber)
            , m_condition(std::move(condition))
            , m_stmtThen(std::move(stmtThen))
            , m_stmtElse(std::move(stmtElse))
        {}

        void accept(Visitor const& visitor) const override { visitor.visit(*this); }

        exprptr m_condition;
        stmtptr m_stmtThen;
        stmtptr m_stmtElse;
    };

    struct StmtWhile : Stmt
    {
        StmtWhile(exprptr&& condition, stmtptr&& body, usize lineNumber)
            : Stmt(lineNumber)
            , m_condition(std::move(condition))
            , m_body(std::move(body))
        {}

        void accept(Visitor const& visitor) const override { return visitor.visit(*this); }

        exprptr m_condition;
        stmtptr m_body;
    };
}

#endif
