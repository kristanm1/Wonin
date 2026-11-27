#ifndef _WONIN_ENVIRONMENT_
#define _WONIN_ENVIRONMENT_

#include "utils.hpp"
#include "visitor.hpp"

namespace wonin
{

    class Callable;
    class Environment;

    using envid = unsigned long long;

    class Environment
    {
        friend class EnvironmentManager;
    public:
        static envid nextId();

        Environment(Environment* parent = nullptr)
            : m_id(nextId())
            , m_parent(parent)
        {}

        void   define(strview key, Value&& value);
        void   assign(strview key, Value&& value);
        Value& get(strview key);

        void capture();

        envid id() const { return m_id; }

    private:
        envid                m_id;
        Environment*         m_parent;
        unordered_map<Value> m_data{};
        bool                 m_captured = false;
    };

    class EnvironmentManager
    {
    public:
        static Environment& create(Environment* parent = nullptr);
        static void         remove(Environment* environment);

    private:
        EnvironmentManager() = default;

        static EnvironmentManager& instance();

        std::unordered_map<envid, Environment> m_environments{};
    };

}

#endif
