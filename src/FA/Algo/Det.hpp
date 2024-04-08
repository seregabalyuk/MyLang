#pragma once

#include <set>
#include <map>

#include <FA/Class/Concepts.hpp>

namespace sb {

    template<C_FA OutFA, C_FA InFA>
    OutFA Det(const InFA& inFA);

/// Realization

    template<class Set, class Map, class FA>
    auto DetDFS(Set&& state, Map& visits, FA& fa) {
        using Type = typename FATraits<FA>::Type;
        using Char = typename FATraits<FA>::Char;

        if (visits.count(state)) { return visits.at(state); }

        std::map<Char, Set> nextStates;

        Type type = Type();
        for (auto inState: state) {
            type |= inState.type();
            for (auto [letter, inNextState]: inState) {
                if (!nextStates.count(letter)) {
                    nextStates.emplace(letter, Set());
                }
                nextStates.at(letter).emplace(inNextState);
            }
        }

        auto stateInFA = fa.emplace(type);
        visits.emplace(std::move(state), stateInFA);

        for (auto& [letter, nextState]: nextStates) {
            auto stateFA = DetDFS(std::move(nextState), visits, fa);
            stateInFA.emplace(letter, stateFA);
        }
        return stateInFA;
    }

    template<C_FA OutFA, C_FA InFA>
    OutFA Det(const InFA& inFA) {
        using InState = typename FATraits<InFA>::ConstState;
        using OutState = typename FATraits<OutFA>::State;
        using Set = std::set<InState>;
        using Map = std::map<Set, OutState>;

        OutFA outFA;
        Map visits;
        Set startState = {inFA.start()};
        DetDFS(std::move(startState), visits, outFA);
        return outFA;
    };

    
} // namespace sb