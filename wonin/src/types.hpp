#ifndef _WONIN_USER_TYPES_
#define _WONIN_USER_TYPES_

#include <string>
#include <cstdint>

namespace wonin
{

    using chr   = char;
    using usize = std::uint32_t;

    using str     = std::basic_string<chr, std::char_traits<chr>, std::allocator<chr>>;
    using strview = std::basic_string_view<chr>;
    using cstr    = chr const*;

}

#endif
