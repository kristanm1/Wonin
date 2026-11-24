#include "environment.hpp"
#include "log.hpp"

using namespace wonin;

void Environment::define(strview key, Value&& value)
{
    auto it = m_data.find(key);
    if (it != m_data.end())
        throw RuntimeError(log::stringify("Variable ", key, " already exists"));
    
    m_data[std::string(key)] = std::move(value);
}

void Environment::assign(strview key, Value&& value)
{
    auto it = m_data.find(key);
    if (it == m_data.end())
    {
        if (m_parent == nullptr)
            throw RuntimeError(log::stringify("Undefined variable ", key));
            
        m_parent->assign(key, std::move(value));
        return;
    }
    
    it->second = std::move(value);
}

Value& Environment::operator[](strview key)
{
    auto it = m_data.find(key);
    if (it == m_data.end())
    {
        if (m_parent != nullptr)
            return m_parent->operator[](key);
        
        throw RuntimeError(log::stringify("Undefined variable ", key));
    }
    
    return it->second;
}
