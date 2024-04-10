#pragma once

#include <concepts>

#include "IteratorTraits.hpp"

namespace sb {
    template<class Iter, class ConstIter>
    concept C_IterConstIter = 
    std::same_as<
        const IteratorTraitsT<Iter>,
        IteratorTraitsT<ConstIter>
    >;
    
    template<class Iter>
    concept C_SimpleIter = 
    requires(Iter iter) {
        {++ iter} -> std::same_as<Iter&>;
        {*iter};
        {iter != iter} -> std::same_as<bool>;
    };
} // namespace sb