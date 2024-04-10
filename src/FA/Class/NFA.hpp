#pragma once

#include "FA.hpp"
#include "Concept.hpp"

namespace sb {
    using NFA = FA<1>;
    /*
    template<class Char>
    using NFA = FA<1, Char>;

    template<class Char, class StateType>
    using NFA = FA<1, Char, StateType>;
    
    template<class Char = char, class StateType = uint64_t, class Alloc = std::allocator<Char>>
    using NFA = FA<1, Char, StateType, Alloc>;*/
} // namespace sb