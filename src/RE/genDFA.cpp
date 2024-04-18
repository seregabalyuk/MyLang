#include <fstream>
#include <iostream>

#include <FA/FastDFA/Save.hpp>
#include <FA/FastDFA/Print.hpp>
#include <FA/Algo/NFA2DFA.hpp>

#include "PosixRPN.hpp"
#include "PosixPlusRPN.hpp"


int main(int argc, char** argv) try {
    if (argc != 4) {
        std::cerr << "Arguments must be 3\n";
        return 0;
    }
    std::string in;
    std::string out;
    size_t type = 0;
    bool name_out = 0;
    for (size_t i = 1; i < argc; ++ i) {
        std::string str(argv[i]);
        if (str == "-p+") {
            type = 2;
            name_out = 1;
        } else if (str == "-p") {
            type = 1;
            name_out = 1;
        } else if (name_out) {
            name_out = 0;
            out = str;
        } else {
            in = str;
        }
    }
    decltype(sb::makePosixRPN()) all;
    if (type == 1) {
        all = sb::makePosixRPN();
    } else if (type == 2) {
        all = sb::makePosixPlusRPN();
    }
    std::ifstream fin(in);
    while(fin) {
        if (fin.peek() != '\n' && fin.peek() != '\0' && fin.peek() != EOF) {
            char chr = fin.get();
            std::get<0>(all)->put(chr);
        } else {
            break;
        }
    }
    fin.close();
    auto nfa = std::get<0>(all)->get();
    auto dfa = sb::nfa2dfa<sb::FA<0, char, char>>(nfa);
    sb::FastDFA fastdfa(dfa);
    std::ofstream fout(out);
    sb::printFastDFA(std::cout, fastdfa);
    sb::saveFastDFA(fout, fastdfa);
} catch (const std::string& str) {
    std::cerr << str << '\n';
}
