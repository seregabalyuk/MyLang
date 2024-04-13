#pragma once

#include <FA/Class/Concept.hpp>
#include <FA/Class/FA.hpp>
#include <RPN/Rule/MyRE.hpp>

#include <FA/Algo/Determine.hpp>
#include <FA/Algo/RemoveEps.hpp>
#include <FA/Algo/Minimize.hpp>

namespace sb {
    template<C_DFA DFA = sb::FA<0>, class Iter, class RPN = decltype(ruleMyRE)>
    DFA RE2DFA(Iter begin, Iter end, const RPN& = ruleMyRE) {
        auto nfa = ruleMyRE(begin, end);
        sb::removeEps(nfa);
        auto dfa = sb::determine<sb::FA<0>>(nfa);
        return sb::minimize<DFA>(dfa);
    }
} // namespace sb