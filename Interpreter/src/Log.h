#ifndef _WONIN_LOG_
#define _WONIN_LOG_

#include <sstream>

#define DBREAK() __debugbreak();

namespace Wonin
{
    void LogImpl(std::string const& s);

    template<typename... Args>
    void Info(Args&&... args)
    {
        std::stringstream ss;
        (ss << ... << std::forward<Args>(args));
        LogImpl(ss.str());
    }

    template<typename... Args>
    void Error(Args&&... args)
    {
        Wonin::Info(std::forward<Args>(args)...);
        DBREAK();
    }

}

#endif
