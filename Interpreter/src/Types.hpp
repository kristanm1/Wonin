#ifndef _WONIN_USER_TYPES_
#define _WONIN_USER_TYPES_

#include <cstdint>

namespace Wonin
{
    using usize = std::uint32_t;

    class RuntimeError
    {
    public:
        ~RuntimeError() = default;
        virtual char const* GetMessage() const = 0;
    };
}

#endif
