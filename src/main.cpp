#include <iostream>
#include <map>
#include <sstream>


#include "FA/PrintFA.hpp"
#include "FA/RE2DFA.hpp"


int main() {
    std::string str = "(1+2+3+4+5+6+7+8+9)|(0+1+2+3+4+5+6+7+8+9)*+0";
    auto mdfa = sb::RE2DFA(str.begin(), str.end());

    sb::printFA(std::cout, mdfa) << '\n';

}