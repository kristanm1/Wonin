#include "interpreter.hpp"
#include "lexer.hpp"
#include "log.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "callable.hpp"

using namespace wonin;

Interpreter::Interpreter()
    : m_environment(EnvironmentManager::create())
{
    m_environment.define("clock", Value(std::make_shared<CallableClock>()));
}

Interpreter::Interpreter(Environment* parent)
    : m_environment(EnvironmentManager::create(parent))
{}

Interpreter::~Interpreter()
{
    EnvironmentManager::remove(&m_environment);
}

bool Interpreter::interpret(Parser const& parser) const
{
    for (auto const& stmt : parser.m_stmts)
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

Value Interpreter::visit(ExprLiteral const& literal) const
{
    return literal.m_data;
}

Value Interpreter::visit(ExprGrouping const& group) const
{
    return group.m_expr->accept(*this);
}

Value Interpreter::visit(ExprVariable const& variable) const
{
    try
    {
        return m_environment.get(variable.m_name);
    }
    catch(RuntimeError const& err)
    {
        throw RuntimeError(log::stringify(
            err.what(), " at line ", variable.lineNumber()
        ));
    }
}

Value Interpreter::visit(ExprLogical const& logical)  const
{
    Value left = logical.m_left->accept(*this);
    if (logical.m_op == Token::Or)
        if (left.getBool())
            return left;
    
    if (!left.getBool())
        return left;
    
    return logical.m_right->accept(*this);
}

Value Interpreter::visit(ExprUnary const& unary) const
{
    Value value = unary.m_expr->accept(*this);

    switch (unary.m_op)
    {
    case Token::Minus:
        return -value;
    
    case Token::Bang:
        return !value.getBool();

    default:
        throw RuntimeError(log::stringify(
            "Invalid unary operator ", Lexeme::cstrToken(unary.m_op),
            " at line ", unary.lineNumber()
        ));   
    }
}

Value Interpreter::visit(ExprBinary const& binary) const
{
    Value left = binary.m_left->accept(*this);
    Value right = binary.m_right->accept(*this);
    switch (binary.m_op)
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
            "Invalid binary operator ", Lexeme::cstrToken(binary.m_op),
            " at line ", binary.lineNumber()
        ));
    }    
}

Value Interpreter::visit(ExprAsgn const& assign) const
{
    m_environment.assign(assign.m_name, assign.m_expr->accept(*this));
    return m_environment.get(assign.m_name);
}

Value Interpreter::visit(ExprCall const& call) const
{
    Value callee = call.m_callee->accept(*this);
    if (!callee.isCallable())
        throw RuntimeError(log::stringify(
            "Invalid call invocation at line ", call.lineNumber()
        ));

    std::vector<Value> arguments{};
    for (exprptr const& arg : call.m_arguments)
        arguments.push_back(arg->accept(*this));
    
    Callable& callable = callee.getCallable();
    if (callable.arity() != arguments.size())
        throw RuntimeError(log::stringify(
            callable.name(), " expected ", callable.arity(), " arguments but got ",
            arguments.size(), " at line ", call.lineNumber()
        ));

    return callable(arguments);
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

    else if (value.isCallable())
        log::info("<fun ", value.getCallable().name(), ">");

    else if (value.isNull())
        log::info("null");

    else if (value.isEmpty())
        log::info("void");
}

void Interpreter::visit(StmtReturn const& stmt) const
{
    Value value = nullptr;
    if (stmt.m_expr != nullptr)
        value = stmt.m_expr->accept(*this);

    throw Return(std::move(value));
}

void Interpreter::visit(StmtVarDecl const& stmt) const
{
    m_environment.define(
        stmt.m_name,
        stmt.m_initializer.get()
            ? stmt.m_initializer->accept(*this)
            : Value(std::nullptr_t{})
    );
}

void Interpreter::visit(StmtFunDecl const& function) const
{
    m_environment.define(
        function.m_name,
        Value(std::make_shared<CallableFunction>(function, m_environment))
    );
}

void Interpreter::visit(StmtBlock const& stmt) const
{
    Interpreter(&m_environment).executeBlock(stmt.m_stmts);
}

void Interpreter::executeBlock(std::vector<stmtptr> const& stmts) const
{
    for (auto const& stmt : stmts)
        stmt->accept(*this);
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
