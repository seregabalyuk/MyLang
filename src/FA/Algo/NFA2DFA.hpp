#pragma once

#include "RemoveEps.hpp"
#include "Determine.hpp"
#include "Minimize.hpp"

namespace sb {
    template<C_DFA DFA, C_FA NFA>
    DFA nfa2dfa(NFA& nfa, FATraitsLe<NFA> eps = FATraitsLe<NFA>()) {
        removeEps(nfa, eps);
        sb::printFA(std::cout, nfa) << '\n';
        DFA dfa = determine<DFA>(nfa);
        sb::printFA(std::cout, dfa) << '\n';
        return minimize<DFA>(dfa);
    }

} // namespace sb