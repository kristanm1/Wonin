#include "callable.hpp"
#include "types.hpp"
#include "interpreter.hpp"

using namespace wonin;

Value CallableClock::operator()(std::vector<Value>& arguments)
{
    return Value(std::chrono::duration<double>(
        std::chrono::steady_clock::now() - m_creationPoint
    ).count());
}

CallableFunction::CallableFunction(StmtFunDecl stmtFunDecl, Environment& captured)
    : Callable(stmtFunDecl.m_parameters.size())
    , m_stmtFunDecl(std::move(stmtFunDecl))
    , m_captured(captured)
{
    captured.capture();
}

CallableFunction::~CallableFunction()
{
    // todo: uncapture environment??
    // count howmany times any environment was captured??
}

Value CallableFunction::operator()(std::vector<Value>& arguments)
{
    Interpreter interpreter(&m_captured);
    for (usize i = 0; i < m_stmtFunDecl.m_parameters.size(); i++)
        interpreter.m_environment.define(
            m_stmtFunDecl.m_parameters[i],
            std::move(arguments[i])
        );

    try
    {
        interpreter.executeBlock(m_stmtFunDecl.m_body);
    }
    catch(Return& ret)
    {
        return ret.consume();
    }

    return {};
}

strview CallableFunction::name() const
{
    return m_stmtFunDecl.m_name;
}
