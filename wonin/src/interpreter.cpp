#include "interpreter.hpp"
#include "scanner.hpp"
#include "log.hpp"
#include "expr.hpp"
#include "stmt.hpp"

using namespace wonin;

void Interpreter::interpret(Environment& environment) const
{
    m_environment = &environment;

    for (auto const& stmt : m_parser.m_stmts)
    {
        try
        {
            stmt->accept(*this);
        }
        catch (RuntimeError const& err)
        {
            log::error("Interpreter error: ", err.what());
            return;
        }
    }
}

Value Interpreter::visit(ExprLiteral const& expr) const
{
    return expr.m_data;
}

Value Interpreter::visit(ExprGrouping const& expr) const
{
    return expr.m_expr->accept(*this);
}

Value Interpreter::visit(ExprVar const& expr) const
{
    auto var = m_parser.m_scanner.lexemeView((expr.m_lexeme));
    try
    {
        return (*m_environment)[var];
    }
    catch(RuntimeError const& err)
    {
        throw RuntimeError(log::stringify(
            err.what(), " at line ",
            m_parser.m_scanner.lineNumber(expr.m_lexeme)
        ));
    }
}

Value Interpreter::visit(ExprUnary const& expr) const
{
    Value value = expr.m_expr->accept(*this);
    if (expr.m_lexeme.token() == Token::Minus)
        return -value;

    else
        throw RuntimeError(log::stringify(
            "Invalid unary operator ", expr.m_lexeme.cstrToken(), " at line ",
            m_parser.m_scanner.lineNumber(expr.m_lexeme)
        ));
}

Value Interpreter::visit(ExprBinary const& expr) const
{
    Value left = expr.m_left->accept(*this);
    Value right = expr.m_right->accept(*this);
    if (expr.m_lexeme.token() == Token::Plus)
        return left + right;

    else if (expr.m_lexeme.token() == Token::Minus)
        return left - right;

    else if (expr.m_lexeme.token() == Token::Star)
        return left * right;

    else if (expr.m_lexeme.token() == Token::Slash)
        return left / right;
    
    throw RuntimeError(log::stringify(
        "Invalid binary operator ", expr.m_lexeme.cstrToken(), " at line ",
            m_parser.m_scanner.lineNumber(expr.m_lexeme)
    ));
}

Value Interpreter::visit(ExprAsgn const& expr) const
{
    try
    {
        auto var = m_parser.m_scanner.lexemeView(expr.m_lexeme);
        m_environment->assign(var, expr.m_expr->accept(*this));
        return (*m_environment)[var];
    }
    catch (RuntimeError const& err)
    {
        throw RuntimeError(log::stringify(
            err.what(), " at line ",
            m_parser.m_scanner.lineNumber(expr.m_lexeme)
        ));
    }
}

void Interpreter::visit(StmtExpr const& stmt) const
{
    stmt.m_expr->accept(*this);
}

void Interpreter::visit(StmtPrint const& stmt) const
{
    Value value = stmt.m_expr->accept(*this);
    if (value.isNumber())
        log::info(value.getNumber());

    else if (value.isString())
        log::info(value.getString());

    else if (value.isBool())
        log::info(value.getBool() ? "true" : "false");

    else if (value.isNull())
        log::info("null");

    else if (value.isEmpty())
        log::info("void");
}

void Interpreter::visit(StmtVarDecl const& stmt) const
{
    if (stmt.m_expr.get())
        m_environment->define(
            m_parser.m_scanner.lexemeString(stmt.m_lexeme),
            stmt.m_expr->accept(*this)
        );
    else
        m_environment->define(
            m_parser.m_scanner.lexemeString(stmt.m_lexeme),
            Value(std::nullptr_t{})
        );
}

void Interpreter::visit(StmtBlock const& stmt) const
{
    Environment wrapper(m_environment);
    executeBlock(stmt.m_stmts, wrapper);
}

void Interpreter::executeBlock(std::vector<StmtPtr> const& stmts, Environment& environment) const
{
    Environment *previousEnvironment = m_environment;
    m_environment = &environment;

    try
    {
        for (auto const& stmt : stmts)
            stmt->accept(*this);
    }
    catch (RuntimeError const& err)
    {
        m_environment = previousEnvironment;

        throw RuntimeError(log::stringify(
            err.what() + " todo: more info.."
        ));
    }

    m_environment = previousEnvironment;
}
