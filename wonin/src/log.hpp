#ifndef _WONIN_LOG_
#define _WONIN_LOG_

#include <sstream>

#ifdef WONGO_PLATFORM_WINDOWS
    #define DBREAK() __debugbreak();
#else
    #define DBREAK()
#endif

#define ASSERT(x) if (!(x)) DBREAK()

namespace Wonin
{
    struct Log
    {
        static void InfoImpl(std::string const& s);
        static void ErrorImpl(std::string const& s);

        template<typename... Args>
        static std::string Stringify(Args&&... args)
        {
            std::stringstream ss;
            (ss << ... << std::forward<Args>(args));
            return ss.str();
        }

        template<typename... Args>
        static void Info(Args&&... args)
        {
            Log::InfoImpl(Stringify(std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Error(Args&&... args)
        {
            Log::ErrorImpl(Stringify(std::forward<Args>(args)...));
            DBREAK();
        }
    };

}

#endif
