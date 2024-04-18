#include <iostream>
#include <fstream>

#include <FA/FastDFA/Open.hpp>
#include <FA/FastDFA/Print.hpp>
#include <Base/Link.hpp>


int main() {
    std::ifstream fin("../test/example.dfa");
    sb::FastDFA<char, char> fastdfa;
    sb::openFastDFA(fin, fastdfa);

    sb::printFastDFA(std::cout, fastdfa);

    std::string in;
    std::getline(std::cin, in, '\n');
    Link state = fastdfa.cstart();
    for (auto letter: in) {
        state = state()[letter];
        std::cout << state().type();
    }
    std::cout << '\n';
}