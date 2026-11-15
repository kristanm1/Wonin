#ifndef _WONIN_LOG_
#define _WONIN_LOG_

#include <sstream>

#define DBREAK() __debugbreak();
#define ASSERT(x) if (!(x)) DBREAK()

namespace Wonin
{
    struct Log
    {
        static void LogImpl(std::string const& s);

        template<typename... Args>
        static void Info(Args&&... args)
        {
            std::stringstream ss;
            (ss << ... << std::forward<Args>(args));
            Log::LogImpl(ss.str());
        }

        template<typename... Args>
        static void Error(Args&&... args)
        {
            Info(std::forward<Args>(args)...);
            DBREAK();
        }
    };

}

#endif
