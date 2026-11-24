#ifndef _WONIN_ENVIRONMENT_
#define _WONIN_ENVIRONMENT_

#include "utils.hpp"
#include "visitor.hpp"

namespace wonin
{

    class Environment
    {
    public:
        Environment(Environment* environment = nullptr) : m_parent(environment) {}

        void define(strview key, Value&& value);
        void assign(strview key, Value&& value);

        Value& operator[](strview key);

    private:
        Environment* m_parent;
        unordered_map<Value> m_data{};
    };

}

#endif
