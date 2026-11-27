#ifndef _WONIN_CALLABLE_
#define _WONIN_CALLABLE_

#include "types.hpp"
#include "value.hpp"
#include "stmt.hpp"
#include "environment.hpp"

#include <chrono>
#include <vector>

namespace wonin
{

    class Interpreter;

    class Callable
    {
        friend class Interpreter;
    public:
        Callable(usize arity) : m_arity(arity) {}
        virtual ~Callable() = default;

        virtual Value operator()(std::vector<Value>& arguments) = 0;

        virtual strview name() const = 0;

        usize arity() const { return m_arity; }
        
    private:
        usize m_arity;
    };

    class CallableClock : public Callable
    {
    public:
        CallableClock()
            : Callable(0)
            , m_creationPoint(std::chrono::steady_clock::now())
        {}

        Value operator()(std::vector<Value>& arguments ) override;

        strview name() const override { return "clock"; }
    
    private:
        std::chrono::steady_clock::time_point m_creationPoint;
    };

    class CallableFunction : public Callable
    {
        friend class Interpreter;
    public:
        CallableFunction(StmtFunDecl stmtFunDecl, Environment& captured);
        ~CallableFunction();

        Value operator()(std::vector<Value>& arguments) override;

        strview name() const override;
    
    private:
        StmtFunDecl m_stmtFunDecl;
        Environment& m_captured;
    };

}

#endif
