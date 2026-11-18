#ifndef _WONIN_TYPE_TRAITS_
#define _WONIN_TYPE_TRAITS_

namespace Wonin
{
    //RemoveReference
    template<typename T>
    struct RemoveRef
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveRef<T&>
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveRef<T&&>
    {
        using Type = T;
    };

    // RemoveConst
    template<typename T>
    struct RemoveConst
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveConst<const T>
    {
        using Type = T;
    };

    // Remove volatile
    template<typename T>
    struct RemoveVolatile
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveVolatile<volatile T>
    {
        using Type = T;
    };

    // Remove const volatile
    template<typename T>
    struct RemoveCV
    {
        using Type = typename RemoveConst<
            typename RemoveVolatile<T>::Type
        >::Type;
    };

    // Remove const volatile reference
    template<typename T>
    struct RemoveCVRef
    {
        using Type = typename RemoveCV<
            typename RemoveRef<T>::Type
        >::Type;
    };

    template<typename T>
    using RemoveCVRefType = RemoveCVRef<T>::Type;

    // IntegralType
    template<typename T, T V>
    struct IntegralType
    {
        static constexpr T Value = V;
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
        using Type = T;
    };

    template<bool B, typename T = void>
    using EnableIfType = typename EnableIf<B, T>::Type;

}

#endif 
