#ifndef _WONIN_UTILS_
#define _WONIN_UTILS_

#include "types.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

namespace wonin::utils
{

    struct ViewHash
    {
        using is_transparent = void;
        size_t operator()(strview sv) const noexcept { return std::hash<strview>{}(sv);}
    };

    std::string loadText(str const& path);

    static bool isDigit(char c) { return c >= '0' && c <= '9'; }
    static bool isLower(char c) { return c >= 'a'  && c <= 'z'; }
    static bool isUpper(char c) { return c >= 'A' && c <= 'Z'; }
    static bool isAlpha(char c) { return isLower(c) || isUpper(c) || c == '_'; }
    static bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

}

namespace wonin
{

    class RuntimeError
    {
    public:
        RuntimeError(str message) : m_message(message) {}

        str const& what() const { return m_message; }
    private:
        str m_message;
    };

    template<typename T>
    using unordered_map = std::unordered_map<
        std::string,
        T,
        utils::ViewHash,
        std::equal_to<>
    >;

}

#endif
