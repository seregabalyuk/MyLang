#pragma once

#include <concepts>

#include "C_Iterator.hpp"
#include "IterableTraits.hpp"

namespace sb {
    template<class T>
    concept C_IterableT = 
        C_IterConstIter<IterableTraitsIt<T>, IterableTraitsCIt<T>> &&
        C_SimpleIter<IterableTraitsIt<T>> &&
        C_SimpleIter<IterableTraitsCIt<T>> &&
    requires(T t, const T ct) {
        {t.begin()} -> std::same_as<IterableTraitsIt<T>>;
        {t.end()} -> std::same_as<IterableTraitsIt<T>>;

        {t.cbegin()} -> std::same_as<IterableTraitsCIt<T>>;
        {t.cend()} -> std::same_as<IterableTraitsCIt<T>>;
        
        {ct.begin()} -> std::same_as<IterableTraitsCIt<T>>;
        {ct.end()} -> std::same_as<IterableTraitsCIt<T>>;

        {ct.cbegin()} -> std::same_as<IterableTraitsCIt<T>>;
        {ct.cend()} -> std::same_as<IterableTraitsCIt<T>>;
    };
} // namespace sb