#pragma once

#include "Class/Concepts.hpp"
#include "Class/DFA.hpp"
#include "../RPN/Rule/MyRE.hpp"
#include "Algo/Det.hpp"
#include "Algo/Eps.hpp"
#include "Algo/Minimize.hpp"

#include "PrintFA.hpp"

namespace sb {
    template<C_DFA DFA = DefaultDFA, class Iter, class RPN = decltype(ruleMyRE)>
    DFA RE2DFA(Iter begin, Iter end, const RPN& = ruleMyRE) {
        auto nfa = ruleMyRE(begin, end);
        //printFA(std::cout, nfa) << '\n';
        sb::RemoveEps(nfa);
        //printFA(std::cout, nfa) << '\n';
        auto dfa = sb::Det<sb::DefaultDFA>(nfa);
        //printFA(std::cout, dfa) << '\n';
        return sb::Minimize<DFA>(dfa);
    }
} // namespace sb