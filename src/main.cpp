#include <iostream>
#include <vector>

#include "FA/PrintFA.hpp"
#include "FA/RE2DFA.hpp"


int main() {
    std::string str = "[1-9]|[0-9]*+0";
    auto mdfa = sb::RE2DFA(str.begin(), str.end());

    sb::printFA(std::cout, mdfa);
   
}