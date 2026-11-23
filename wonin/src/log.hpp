#ifndef _WONIN_LOG_
#define _WONIN_LOG_

#include "types.hpp"

#include <sstream>

#ifdef WONIN_PLATFORM_WINDOWS
    #if NDEBUG
        #define DBREAK()
    #else
        #define DBREAK() __debugbreak();
    #endif
#else
    #define DBREAK()
#endif

#define ASSERT(x) if (!(x)) DBREAK()

namespace wonin::log
{
    void infoImpl(std::string const& s);
    void errorImpl(std::string const& s);

    template<typename... Args>
    std::string stringify(Args&&... args)
    {
        return (std::stringstream{} << ... << std::forward<Args>(args)).str();
    }

    template<typename... Args>
    void info(Args&&... args)
    {
        infoImpl(stringify(std::forward<Args>(args)...));
    }

    template<typename... Args>
    void error(Args&&... args)
    {
        errorImpl(stringify(std::forward<Args>(args)...));
        DBREAK();
    }

    void scannerError(std::string message, usize lineNumber);

}

#endif
