#include <iostream>

#include <RE/RE2DFA.hpp>
#include <FA/Print.hpp>

template<sb::C_NFA FA>
void check() {}

int main() {
   std::string str = "[1-9]|[0-9]*+0";
   auto dfa = sb::RE2DFA(str.begin(), str.end());
   sb::printFA(std::cout, dfa);
}