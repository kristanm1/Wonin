#include "environment.hpp"
#include "log.hpp"

using namespace wonin;

envid Environment::nextId()
{
    static envid id = 0;
    return id++;
}

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

Value& Environment::get(strview key)
{
    auto it = m_data.find(key);
    if (it == m_data.end())
    {
        if (m_parent != nullptr)
            return m_parent->get(key);
        
        throw RuntimeError(log::stringify("Undefined variable ", key));
    }
    
    return it->second;
}

void Environment::capture()
{
    m_captured = true;

    if (m_parent)
        m_parent->capture();
}

EnvironmentManager& EnvironmentManager::instance()
{
    static EnvironmentManager singleton;
    return singleton;
}

Environment& EnvironmentManager::create(Environment* parent)
{
    EnvironmentManager& mng = instance();
    Environment environment(parent);
    envid id = environment.id();
    
    if (mng.m_environments.find(id) != mng.m_environments.end())
        throw RuntimeError(log::stringify("Environment with id: ", id, " already exists"));
    
    mng.m_environments[id] = std::move(environment);
    return mng.m_environments[id];
}

void EnvironmentManager::remove(Environment* envronment)
{
    EnvironmentManager& mng = instance();

    auto it = mng.m_environments.find(envronment->id());
    if (it == mng.m_environments.end())
        throw RuntimeError(log::stringify(
            "Environment with id: ", envronment->id(), " does not exists"
        ));
    
    if (!it->second.m_captured)
        mng.m_environments.erase(it);
}
