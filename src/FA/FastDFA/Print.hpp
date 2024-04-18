#pragma once

#include "DFA.hpp"

namespace sb {
    template<class Cout, class A, class B, class C>
    Cout& printFastDFA(Cout& cout, const FastDFA<A, B, C>& fastdfa) {
        for (size_t i = 0; i < fastdfa.size(); ++ i) {
			auto& state = fastdfa[i];
			cout << i << ", " << state.type() << ": ";
			for (auto& trans: state) {
				cout << (int)trans.letter() << ">" << trans.number() << ", ";
			}
			cout << '\n';
		}
		cout << '\n';
        return cout;
    }
} // namespace sb