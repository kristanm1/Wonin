#ifndef _WONIN_UTILS_
#define _WONIN_UTILS_

#include <string>
#include <string_view>
#include <unordered_map>

namespace wonin::utils
{

    std::string loadText(std::string const& path);

    static bool isDigit(char c) { return c >= '0' && c <= '9'; }
    static bool isLower(char c) { return c >= 'a'  && c <= 'z'; }
    static bool isUpper(char c) { return c >= 'A' && c <= 'Z'; }
    static bool isAlpha(char c) { return isLower(c) || isUpper(c) || c == '_'; }
    static bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

    struct StringViewHash
    {
        using is_transparent = void;
        size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv);}
    };

    template<typename T>
    using unordered_map = std::unordered_map<std::string, T, StringViewHash, std::equal_to<>>;

}

#endif
