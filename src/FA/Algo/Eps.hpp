#pragma once

#include <set>

#include <FA/Class/Concepts.hpp>

namespace sb {
/// Set has to work with your states

    template<C_FA FA, class Set = std::set<typename FATraits<FA>::State>>
    void RemoveEps(FA& fa, typename FATraits<FA>::Char eps = 0);

/// Realization
    template<class State, class Char, class Set>
    void FindEps(State state, Char eps, Set& visits) {
        if (visits.count(state)) { return; }
        visits.emplace(state);
        for(auto [letter, next]: state) {
            if (letter == eps) {
                FindEps(next, eps, visits);
            }
        }
    }

    template<class State, class Char, class Set>
    void VisitAll(State state, Char eps, Set& visits) {
        if (visits.count(state)) { return; }
        visits.emplace(state);

        Set epsmates;
        FindEps(state, eps, epsmates);

        for (auto mate: epsmates) {
            state.type() |= mate.type();
            for(auto [letter, next]: mate) {
                state.emplace(letter, next);
            }
        }

        for(auto [letter, next]: state) {
            VisitAll(next, eps, visits);
        }
    }

    template<class State, class Set>
    void VisitErase(State state, Set& visits) {
        if (!visits.count(state)) { return; }
        visits.erase(state);
        for(auto [letter, next]: state) {
            VisitErase(next, visits);
        }
    }

    template<C_FA FA, class Set>
    void RemoveEps(FA& fa, typename FATraits<FA>::Char eps) {
        Set visits;
        VisitAll(fa.start(), eps, visits);
        for (auto state: visits) {
            state.erase(eps);
        }
        VisitErase(fa.start(), visits);
        for (auto state: visits) {
            fa.erase(state);
        }
    }

    
} // namespace sb