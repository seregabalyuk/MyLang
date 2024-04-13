#pragma once

#include "RemoveEps.hpp"
#include "Determine.hpp"
#include "Minimize.hpp"

namespace sb {
    template<C_DFA DFA, C_FA NFA>
    DFA nfa2dfa(NFA& nfa, FATraitsLe<NFA> eps = FATraitsLe<NFA>()) {
        removeEps(nfa, eps);
        DFA dfa = determine<DFA>(nfa);
        return minimize<DFA>(dfa);
    }

} // namespace sb