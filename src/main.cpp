#include <iostream>
#include <vector>

#include "FA/PrintFA.hpp"
#include "FA/RE2DFA.hpp"

#include "CFG/Class/CFG.hpp"


int main() {
    std::string str = "([1-9]|[0-9]*+0)";
    auto mdfa = sb::RE2DFA(str.begin(), str.end());
    
    sb::CFG<char> cfg;
    cfg.emplace_terminal("dota2", 'o');
    auto cock = cfg.emplace_nonterminal("cock");
    cfg.emplace_rule(cock);

    sb::printFA(std::cout, mdfa);
   
}