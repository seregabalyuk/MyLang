#pragma once

#include <FA/Class/Concept.hpp>

namespace sb {
    template<C_NFA NFA>
    void concat(NFA& left,
                NFA&& right) {
        for (auto& trans: right.start()) {
            left.finish().emplace(trans.letter(), trans.next());
        }
        left.finish().type() = right.start().type();
        right.erase(right.begin());
        left.splice(std::move(right));
    }

    template<C_NFA NFA>
    void alter(NFA& left,
               NFA&& right) {
        for (auto& trans: right.start()) {
            left.start().emplace(trans.letter(), trans.next());
        }
        right.erase(right.begin());
        left.splice(std::move(right));
    }

    template<C_NFA NFA>
    void kleene(NFA& nfa) {
        auto eps = FATraitsLe<NFA>();
        nfa.start().emplace(eps, nfa.finish());
        nfa.finish().emplace(eps, nfa.start());
    }

    template<C_NFA NFA>
    void plus(NFA& nfa) {
        auto eps = FATraitsLe<NFA>();
        nfa.finish().emplace(eps, nfa.start());
    }
} // namespace sb