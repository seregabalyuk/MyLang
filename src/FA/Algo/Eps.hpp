#pragma once

#include <algorithm>

#include <FA/Class/Concept.hpp>

#include "SetStateFA.hpp"

namespace sb {
/// Set has to work with your states

    template<C_FA FA>
    void removeEps(FA& fa, FATraitsLe<FA> eps = FATraitsLe<FA>());

/// Realization
    template<C_FA FA>
    void findEps(FATraitsSt<FA>& state, FATraitsLe<FA> eps, SetStateFA<FA>& visits) {
        if (visits.count(state)) { return; }
        visits.emplace(state);
        for(auto& trans: state) {
            if (trans.letter() == eps) {
                findEps<FA>(trans.next(), eps, visits);
            }
        }
    }

    template<C_FA FA>
    void visitAll(FATraitsSt<FA>& state, FATraitsLe<FA> eps, SetStateFA<FA>& visits) {
        if (visits.count(state)) { return; }
        visits.emplace(state);

        SetStateFA<FA> epsmates;
        findEps<FA>(state, eps, epsmates);

        for (auto& mate: epsmates) {
            state.type() |= mate.type();
            for(auto& trans: mate) {
                state.emplace(trans.letter(), trans.next());
            }
        }

        for(auto& trans: state) {
            visitAll<FA>(trans.next(), eps, visits);
        }
    }
    
    template<class FA>
    struct _RemoverFA {
        SetStateFA<FA>& _set;

        bool operator()(FATraitsSt<FA>& state) const {
            return !_set.count(state);
        };
    };

    template<C_FA FA>
    void removeEps(FA& fa, FATraitsLe<FA> eps) {
        SetStateFA<FA> visits;
        visitAll<FA>(fa.start(), eps, visits);
        for (auto& state: visits) {
            state.erase(eps);
        }
        std::remove_if(fa.begin(), fa.end(), _RemoverFA<FA>(visits));
    }

    
} // namespace sb