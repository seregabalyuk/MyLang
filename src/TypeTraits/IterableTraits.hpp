#pragma once

#include <TypeTraits/DeclVal.hpp>

#include "IteratorTraits.hpp"

namespace sb {
    template<class T>
    struct IterableTraits;
    // have
        // Iterator
        // ConstIterator
        // Type
    //

    template<class T>
    using IterableTraitsT = typename IterableTraits<T>::Type;
    template<class T>
    using IterableTraitsIt = typename IterableTraits<T>::Iterator;
    template<class T>
    using IterableTraitsCIt = typename IterableTraits<T>::ConstIterator;
// realization
  // Iterator and ConstIterator
    template<class T>
    concept _C_ItIt =  // concept Iterable Iteartor
    requires(T t, const T ct) {
        {t.begin()};
        {ct.begin()};
    };

    template<class T, bool HaveIter>
    struct _T_ItIt { // traits Iterable Iteartor
        using type = void;
        using const_type = void;
    };

    template<class T>
    struct _T_ItIt<T, 1> {
        using type = decltype(DeclLVal<T>().begin());
        using const_type = decltype(DeclLVal<const T>().begin());
    };
  // Traits
    template<class T>
    struct IterableTraits {
        using Iterator = typename _T_ItIt<T, _C_ItIt<T>>::type;
        using ConstIterator = typename _T_ItIt<T, _C_ItIt<T>>::const_type;

        using Type = IteratorTraitsT<Iterator>;
    };
} // namespace sb