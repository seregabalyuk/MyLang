#pragma once

#include <FA/Class/Concept.hpp>

#include "SetStateFA.hpp"

namespace sb {
/// Set has to work with your states

    template<C_FA FA>
    void removeUseless(FA& fa, FATraitsTy<FA> useless = FATraitsTy<FA>());

/// Realization
    template<C_FA FA>
    bool visitUseAndUseless(
        FATraitsSt<FA>& state,
        const FATraitsTy<FA>& useless, 
        SetStateFA<FA>& visits,
        SetStateFA<FA>& use) 
    {
        if (visits.count(state)) { 
            return use.count(state);
        }
        visits.emplace(state);

        bool isUse = false;
        if (state.type() != useless) {
            use.emplace(state);
            isUse = true;
        }
        for(auto& trans: state) {
            bool isNextUse = visitUseAndUseless<FA>(
                trans.next(),
                useless, 
                visits, 
                use
            );
            if (!isUse && isNextUse) {
                use.emplace(state);
                isUse = true;
            }
        }
        return isUse;
    }

    template<C_FA FA>
    void removeUseless(FA& fa, FATraitsTy<FA> useless) {
        SetStateFA<FA> visits;
        SetStateFA<FA> use;
        visitUseAndUseless<FA>(fa.start(), useless, visits, use);
        for (auto iter = fa.begin(); iter != fa.end();) {
            if (use.count(*iter)) {
                ++ iter;
            } else {
                auto del = iter ++;
                fa.erase(del);
            }
        }
        if (fa.size() == 0) {
            fa.emplace();
        }
    }    
} // namespace sb