#pragma once

#include <type_traits>
#include <utility>

namespace sb {
    template<class T, size_t Size>
    struct get_bindings;

    template<class T, class... Args>
    struct check_bindings;
    
    template<class T, class... Args>
    constexpr bool check_bindings_v = check_bindings<T, Args...>::value;

/// Realization

    template<class T>
    auto check2_1(T in) {
        auto [v1, v2] = in;
        return v1;
    }

    template<class T>
    auto check2_2(T in) {
        auto [v1, v2] = in;
        return v2;
    }

    template<class T>
    struct get_bindings<T, 2> {
        using T1 = decltype(check2_1<T>(std::declval<T>()));
        using T2 = decltype(check2_2<T>(std::declval<T>()));

        using constT1 = decltype(check2_1<const T>(std::declval<const T>()));
        using constT2 = decltype(check2_2<const T>(std::declval<const T>()));

        static constexpr bool value = true;
    };
    
    template<class T, class U1, class U2>
    struct check_bindings<T, U1, U2> : public get_bindings<T, 2> {
        using typename get_bindings<T, 2>::T1;
        using typename get_bindings<T, 2>::T2;

        static constexpr bool value = std::is_same_v<U1, T1> && std::is_same_v<U2, T2>;
    };
} // namespace sb