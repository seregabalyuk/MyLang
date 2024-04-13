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

		auto all = sb::makePosixRPN();
		for (auto letter: str) {
			std::get<0>(all)->put(letter);
		}
		auto nfa = std::get<0>(all)->get();
		sb::printFA(std::cout, nfa) << '\n';

		auto dfa = sb::nfa2dfa<sb::FA<0>>(nfa);
		sb::printFA(std::cout, dfa) << '\n';
	} catch (const std::string& error) {
		std::cout << error << '\n';
	}
}