#include "interpreter.hpp"
#include "lexer.hpp"
#include "log.hpp"
#include "expr.hpp"
#include "stmt.hpp"

using namespace wonin;

bool Interpreter::interpret(Environment& environment) const
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
            return false;
        }
    }

    return true;
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
    auto var = m_parser.m_lexer.lexemeView((expr.m_lexeme));
    try
    {
        return (*m_environment)[var];
    }
    catch(RuntimeError const& err)
    {
        throw RuntimeError(log::stringify(
            err.what(), " at line ",
            m_parser.m_lexer.lineNumber(expr.m_lexeme)
        ));
    }
}

Value Interpreter::visit(ExprLogical const& expr)  const
{
    Value left = expr.m_left->accept(*this);
    if (expr.m_op.token() == Token::Or)
        if (left.getBool())
            return left;
    
    if (!left.getBool())
        return left;
    
    return expr.m_right->accept(*this);
}

Value Interpreter::visit(ExprUnary const& expr) const
{
    Value value = expr.m_expr->accept(*this);

    switch (expr.m_op.token())
    {
    case Token::Minus:
        return -value;
    
    case Token::Bang:
        return !value.getBool();

    default:
        throw RuntimeError(log::stringify(
            "Invalid unary operator ", expr.m_op.cstrToken(), " at line ",
            m_parser.m_lexer.lineNumber(expr.m_op)
        ));   
    }
}

Value Interpreter::visit(ExprBinary const& expr) const
{
    Value left = expr.m_left->accept(*this);
    Value right = expr.m_right->accept(*this);
    switch (expr.m_op.token())
    {
    case Token::Plus:
        return left + right;
    
    case Token::Minus:
        return left - right;
    
    case Token::Star:
        return left * right;
    
    case Token::Slash:
        return left / right;
    
    case Token::EqualEqual:
        return left == right;
    
    case Token::BangEqual:
        return left != right;
    
    case Token::Less:
        return left < right;
    
    case Token::LessEqual:
        return left <= right;
    
    case Token::Greater:
        return left > right;
    
    case Token::GreaterEqual:
        return left >= right;
    
    default:
        throw RuntimeError(log::stringify(
            "Invalid binary operator ", expr.m_op.cstrToken(), " at line ",
                m_parser.m_lexer.lineNumber(expr.m_op)
        ));
    }    
}

Value Interpreter::visit(ExprAsgn const& expr) const
{
    try
    {
        auto var = m_parser.m_lexer.lexemeView(expr.m_lexeme);
        m_environment->assign(var, expr.m_expr->accept(*this));
        return (*m_environment)[var];
    }
    catch (RuntimeError const& err)
    {
        throw RuntimeError(log::stringify(
            err.what(), " at line ",
            m_parser.m_lexer.lineNumber(expr.m_lexeme)
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
    if (stmt.m_initializer.get())
        m_environment->define(
            m_parser.m_lexer.lexemeString(stmt.m_variable),
            stmt.m_initializer->accept(*this)
        );
    else
        m_environment->define(
            m_parser.m_lexer.lexemeString(stmt.m_variable),
            Value(std::nullptr_t{})
        );
}

void Interpreter::visit(StmtBlock const& stmt) const
{
    Environment wrapper(m_environment);
    executeBlock(stmt.m_stmts, wrapper);
}

void Interpreter::executeBlock(
    std::vector<StmtPtr> const& stmts,
    Environment& environment
) const
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

void Interpreter::visit(StmtIf const& stmt) const
{
    Value condition = stmt.m_condition->accept(*this);
    if (condition.getBool())
        stmt.m_stmtThen->accept(*this);
    else if (stmt.m_stmtElse != nullptr)
        stmt.m_stmtElse->accept(*this);
}

void Interpreter::visit(StmtWhile const& stmt) const
{
    while (stmt.m_condition->accept(*this).getBool())
        stmt.m_body->accept(*this);
}
