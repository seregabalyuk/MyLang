#pragma once

#include <FA/Class/Concept.hpp>

namespace sb {
    template<C_NFA NFA>
    void concat(NFA& left,
                NFA&& right) {
        auto eps = FATraitsLe<NFA>();

        left.finish().type() = FATraitsTy<NFA>();

        left.finish().emplace(eps, right.start());

        left.splice(std::move(right));
    }

    template<C_NFA NFA>
    void alter(NFA& left,
               NFA&& right) {
        auto eps = FATraitsLe<NFA>();
        
        left.finish().type() = FATraitsTy<NFA>();

        left.start().emplace(eps, right.start());
        left.finish().emplace(eps, right.finish());
        
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

    template<C_NFA NFA>
    void question(NFA& nfa) {
        auto eps = FATraitsLe<NFA>();
        nfa.start().emplace(eps, nfa.finish());
    }
} // namespace sb