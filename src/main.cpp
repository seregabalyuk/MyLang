#include <iostream>

#include <FA/Print.hpp>
#include <RE/PosixRPN.hpp>
#include <FA/Algo/NFA2DFA.hpp>
#include <CFG/Class/CFG.hpp>
#include <FA/Class/FastDFA.hpp>

struct A {
	char a, b, c;
};

int main() {
	try {
		std::string str;
		{ std::ifstream fin("../src/example.txt");
		  std::getline(fin, str, '\n');
		}
		std::cout << str << '\n';

		auto all = sb::makePosixRPN();
		for (auto letter: str) {
			std::get<0>(all)->put(letter);
		}
		auto nfa = std::get<0>(all)->get();
		auto dfa = sb::nfa2dfa<sb::FA<0>>(nfa);
		sb::FastDFA fastdfa(dfa);
		
		
		std::string in;
		std::getline(std::cin, in, '\n');
		Link state = fastdfa.start();
		for (auto letter: in) {
			state = state()[letter];
		}
		if (state().type() == 1) {
			std::cout << "belong\n";
		} else {
			std::cout << "no\n";
		}
	} catch (const std::string& error) {
		std::cout << error << '\n';
	}
}