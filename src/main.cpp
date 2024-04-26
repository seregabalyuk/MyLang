#include <iostream>

#include <CFG/Class/CFG.hpp>
#include <CFG/ReadFile.hpp>
#include <CFG/Print.hpp>
#include <CFG/Algo/Earley.hpp>
#include <CFG/Class/CFG.hpp>

#include <Actual/DFA.hpp>


int main() try {
    sb::CFG<sb::act::DFA> cfg;
    std::ifstream fin("../test/first.cfg");
    sb::readFileCFG(fin, cfg);
    sb::printCFG(std::cout, cfg);
} catch (const std::string& str) {
    std::cout << str << '\n';
    throw;
}