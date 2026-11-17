#ifndef _WONIN_USER_TYPES_
#define _WONIN_USER_TYPES_

#include <string>
#include <cstdint>

namespace Wonin
{
    using usize = std::uint32_t;

    class RuntimeError
    {
    public:
        ~RuntimeError() = default;
        virtual std::string GetMessage() const = 0;
    };
}

#endif
