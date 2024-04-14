#include <iostream>

#include <FA/Print.hpp>
#include <RE/PosixRPN.hpp>
#include <FA/Algo/NFA2DFA.hpp>

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
		sb::printFA(std::cout, nfa) << '\n';
		auto dfa = sb::nfa2dfa<sb::FA<0>>(nfa);
		sb::printFA(std::cout, dfa) << '\n';
		
		std::string in;
		std::getline(std::cin, in, '\n');
		Link state = dfa.start();
		bool out = 0;
		for (auto letter: in) {
			if (state().count(letter)) {
				state = state()[letter];
			} else {
				out = 1;
				break;
			}
		}
		if (!out && state().type() == 1) {
			std::cout << "belong\n";
		} else {
			std::cout << "no\n";
		}
	} catch (const std::string& error) {
		std::cout << error << '\n';
	}
}