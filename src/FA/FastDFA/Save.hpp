#pragma once

#include <Base/Stream.hpp>
#include "DFA.hpp"

namespace sb {
    template<class ofstream, class A, class B, class C>
    ofstream& saveFastDFA(ofstream& fout, const FastDFA<A, B, C>& dfa) {
        write(fout, dfa.size());
        for (auto& state: dfa.vec()) {
            write(fout, state.type());
            write(fout, state.size());
            for (auto& trans: state) {
                write(fout, trans.letter());
                write(fout, trans.number());
            }
        }        
        return fout;
    }
} // namespace sb