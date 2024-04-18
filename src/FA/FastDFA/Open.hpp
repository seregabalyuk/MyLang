#pragma once

#include <Base/Stream.hpp>
#include "DFA.hpp"

namespace sb {
    template<class ifstream, class A, class B, class C>
    ifstream& openFastDFA(ifstream& fin, FastDFA<A, B, C>& dfa) {
        size_t size;
        read(fin, size);
        for (size_t i = 0; i < size; ++ i) {
            B type;
            read(fin, type);
            auto& state = dfa.vec().emplace_back(dfa.vec(), type, dfa.vec().get_allocator());
            size_t state_size;
            read(fin, state_size);
            for (size_t j = 0; j < state_size; ++ j) {
                A letter;
                read(fin, letter);
                size_t next;
                read(fin, next);
                state.vec().emplace_back(letter, next, state);
            }
        }
        return fin;
    }
} // namespace sb