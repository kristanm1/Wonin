#ifndef _WONIN_UTILS_
#define _WONIN_UTILS_

#include <string>

namespace Wonin
{

    struct Utils
    {
        static std::string LoadText(std::string const& path);

        static bool IsDigit(char c) { return c >= '0' && c <= '9'; }
        static bool IsLower(char c) { return c >= 'a'  && c <= 'z'; }
        static bool IsUpper(char c) { return c >= 'A' && c <= 'Z'; }
        static bool IsAlpha(char c) { return IsLower(c) || IsUpper(c) || c == '_'; }
        static bool IsAlphaNumeric(char c) { return IsAlpha(c) || IsDigit(c); }
    };

}

#endif
