#ifndef _WONIN_TYPE_TRAITS_
#define _WONIN_TYPE_TRAITS_

namespace Wonin
{

    // IntegralType
    template<typename T, T Value>
    struct IntegralType
    {
        static constexpr T value = Value;
    }; 

    template<bool B>
    struct BoolType : IntegralType<bool, B> {};

    using FalseType = BoolType<false>;
    using TrueType = BoolType<true>;

    // IsSame
    template<typename T, typename U>
    struct IsSame : FalseType {};

    template<typename T>
    struct IsSame<T, T> : TrueType {};

    // EnableIf
    template<bool B, class T = void>
    struct EnableIf{};

    template<class T>
    struct EnableIf<true, T>
    {
        typedef T type;
    };

    template<bool B, typename T = void>
    using EnableIfType = typename EnableIf<B, T>::type;

}

#endif 
