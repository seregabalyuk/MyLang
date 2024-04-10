#pragma once

#include <FA/Class/NFA.hpp>
#include <FA/Algo/FuncNFA.hpp>


namespace sb {
    NFA concatNFA(NFA&& left, NFA&& right) {
        return concat(std::move(left), std::move(right));
    }

    NFA alterNFA(NFA&& left, NFA&& right) {
        return alter(std::move(left), std::move(right));
    }

    NFA plusNFA(NFA&& one) {
        return plus(std::move(one));
    }

    NFA kleeneNFA(NFA&& one) {
        return kleene(std::move(one));
    }

    NFA finalyNFA(NFA&& one) {
        one.finish().type() = 1;
        return one;
    }
}