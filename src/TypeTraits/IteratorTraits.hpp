#pragma once

#include <type_traits>

#include <TypeTraits/DeclVal.hpp>

namespace sb {
    template<class Iter>
    struct IteratorTraits;
    // have
        // Type
    //

    template<class T>
    using IteratorTraitsT = typename IteratorTraits<T>::Type;
// realization
  // Type
    template<class Iter>
    concept _C_ItT =  // concept Iter Type
    requires(Iter iter) {
        {*iter};
    };

    template<class Iter, bool HaveType>
    struct _T_ItT { // traits Iter Type
        using type = void;
    };

    template<class Iter>
    struct _T_ItT<Iter, 1> {
        using type = std::remove_reference_t<decltype(*DeclLVal<Iter>())>;
    };
  // Traits
    template<class Iter>
    struct IteratorTraits {
        using Type = typename _T_ItT<Iter, _C_ItT<Iter>>::type;
    };
} // namespace sb