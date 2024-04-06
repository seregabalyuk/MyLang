#pragma once

#include "Class/Concepts.hpp"
#include "Class/DFA.hpp"
#include "../RPN/Rule/MyRE.hpp"
#include "Algo/Det.hpp"
#include "Algo/Eps.hpp"
#include "Algo/Minimize.hpp"

namespace sb {
    template<C_DFA DFA = DefaultDFA, class Iter, class RPN = decltype(ruleMyRE)>
    DFA RE2DFA(Iter begin, Iter end, const RPN& = ruleMyRE) {
        auto nfa = ruleMyRE(begin, end);
        sb::RemoveEps(nfa);
        auto dfa = sb::Det<sb::DefaultDFA>(nfa);
        return sb::Minimize<DFA>(dfa);
    }
} // namespace sb