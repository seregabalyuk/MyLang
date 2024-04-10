#include <bits/stdc++.h>

#include <FA/PrintFA.hpp>
//#include "FA/RE2DFA.hpp"

#include <FA/Class/FA.hpp>

#include <FA/Class/Concept.hpp>
#include <FA/Algo/Eps.hpp>
#include <FA/Algo/Determine.hpp>

#include <RPN/Rule/MyRE.hpp>


template<sb::C_NFA FA>
void check() {}

int main() {
   std::string str = "[1-9]|[0-9]*+0";
   auto nfa = sb::ruleMyRE(str.begin(), str.end());

   sb::removeEps(nfa);
   auto nfa2 = sb::determine(nfa);
   sb::printFA(std::cout, nfa2);
}