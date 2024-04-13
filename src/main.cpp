#include <iostream>

#include <FA/Print.hpp>

#include <RE/PosixRPN.hpp>

void dd(sb::posix::NFA&, sb::posix::NFA&&) {}

template<class Func>
void add(Func func) {
	std::cout << typeid(Func).name() << '\n';
}



int main() {
	try {
		auto all = sb::makePosixRPN();
		std::string str;
		std::getline(std::cin, str, '\n');
		for (auto letter: str) {
			std::get<0>(all)->put(letter);
		}
		auto nfa = std::get<0>(all)->get();
		sb::printFA(std::cout, nfa);
	} catch (const std::string& error) {
		std::cout << error << '\n';
	}
}