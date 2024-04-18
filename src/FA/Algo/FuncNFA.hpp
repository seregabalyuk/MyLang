#pragma once

#include <FA/Class/Concept.hpp>

namespace sb {
    template<C_NFA NFA>
    void concat(NFA& left,
                NFA&& right) {
        auto eps = FATraitsLe<NFA>();
        left.finish().emplace(eps, right.start());
        left.splice(std::move(right));
    } // OK

    template<C_NFA NFA>
    void alter(NFA& left,
               NFA&& right) {
        auto eps = FATraitsLe<NFA>();

        auto& lstart = left.start();
        auto& rstart = right.start();

        auto& lfinish = left.finish();
        auto& rfinish = right.finish();

        left.splice(std::move(right));
        left.emplace_front();
        left.emplace_back();

        left.start().emplace(eps, lstart);
        left.start().emplace(eps, rstart);
        
        lfinish.emplace(eps, left.finish());
        rfinish.emplace(eps, left.finish());
    } // OK

    template<C_NFA NFA>
    void kleene(NFA& nfa) {
        auto eps = FATraitsLe<NFA>();

        auto& prevstart = nfa.start();
        auto& prevfinish = nfa.finish();
        
        nfa.emplace_front();
        nfa.emplace_back();
        
        nfa.start().emplace(eps, nfa.finish());
        nfa.finish().emplace(eps, nfa.start());
        
        nfa.start().emplace(eps, prevstart);
        prevfinish.emplace(eps, nfa.finish());
    } // OK

    template<C_NFA NFA>
    void plus(NFA& nfa) {
        auto eps = FATraitsLe<NFA>();
        
        auto& prevstart = nfa.start();
        auto& prevfinish = nfa.finish();
        
        nfa.emplace_front();
        nfa.emplace_back();
        
        nfa.finish().emplace(eps, nfa.start());
        
        nfa.start().emplace(eps, prevstart);
        prevfinish.emplace(eps, nfa.finish());
    } // OK

    template<C_NFA NFA>
    void question(NFA& nfa) {
        auto eps = FATraitsLe<NFA>();

        auto& prevstart = nfa.start();
        auto& prevfinish = nfa.finish();
        
        nfa.emplace_front();
        nfa.emplace_back();
        
        nfa.start().emplace(eps, nfa.finish());
        
        nfa.start().emplace(eps, prevstart);
        prevfinish.emplace(eps, nfa.finish());
    } // OK

    template<C_NFA NFA>
    void setType(
        NFA& left,
        NFA&& right) {
        left.finish().type() |= right.finish().type();
    }
} // namespace sb