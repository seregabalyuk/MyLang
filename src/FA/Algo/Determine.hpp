#pragma once

#include <unordered_map>

#include <FA/Class/Concept.hpp>

#include "SetStateFA.hpp"

namespace sb {

    template<C_FA OutFA, C_FA InFA>
    OutFA determine(const InFA& inFA);

/// Realization

    template<class Set, class Map, class FA>
    FATraitsSt<FA>* detDFS(Set&& state, Map& visits, FA& fa) {
        using Type = FATraitsTy<FA>;
        using Letter = FATraitsLe<FA>;

        if (visits.count(state)) { return visits.at(state); }

        std::unordered_map<Letter, Set> nextStates;

        Type type = Type();
        for (auto& inState: state) {
            type |= inState.type();
            for (auto& trans: inState) {
                if (!nextStates.count(trans.letter())) {
                    nextStates.emplace(trans.letter(), Set());
                }
                nextStates.at(trans.letter()).emplace(trans.next());
            }
        }

        auto* stateInFA = &fa.emplace(type);
        visits.emplace(std::move(state), stateInFA);

        for (auto& [letter, nextState]: nextStates) {
            FATraitsSt<FA>* stateFA = detDFS(std::move(nextState), visits, fa);
            stateInFA->emplace(letter, *stateFA);
        }
        return stateInFA;
    }

    template<C_FA OutFA, C_FA InFA>
    OutFA determine(const InFA& inFA) {
        using InState = FATraitsSt<InFA>;
        using OutState = FATraitsSt<OutFA>;
        using Set = SetConstStateFA<InFA>;
        using Map = std::unordered_map<Set, OutState*>;

        OutFA outFA;
        Map visits;
        Set startState;
        startState.emplace(inFA.start());
        detDFS(std::move(startState), visits, outFA);
        return outFA;
    };

    
} // namespace sb