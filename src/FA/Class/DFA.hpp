#pragma once

#include "FA.hpp"

namespace sb {
    using DFA = FA<0>;
    /*
    template<class Char>
    using DFA = FA<0, Char>;

    template<class Char, class StateType>
    using DFA = FA<0, Char, StateType>;

    template<class Char, class StateType, class Alloc>
    using DFA = FA<0, Char, StateType, Alloc>;*/
} // namespace sb