#pragma once

#include <FA/Class/Concept.hpp>

namespace sb {
    template<C_NFA NFA>
    NFA concat(NFA&& left,
               NFA&& right,
               FATraitsLe<NFA> eps = FATraitsLe<NFA>()) {
        NFA ret;
        left.finish().emplace(eps, right.start());
        ret.splice(left);
        ret.splice(right);
        return ret;
    }

    template<C_NFA NFA>
    NFA alter(NFA&& left,
              NFA&& right,
              FATraitsLe<NFA> eps = FATraitsLe<NFA>()) {
        NFA ret;
        auto& finishL = left.finish();
        auto& startR = right.start();
        ret.splice(left);
        ret.splice(right);
        finishL.emplace(eps, ret.finish());
        ret.start().emplace(eps, startR);
        return ret;
    }

    template<C_NFA NFA>
    NFA kleene(NFA&& nfa,
               FATraitsLe<NFA> eps = FATraitsLe<NFA>()) {
        NFA ret(std::move(nfa));
        ret.start().emplace(eps, ret.finish());
        ret.finish().emplace(eps, ret.start());
        return ret;
    }

    template<C_NFA NFA>
    NFA plus(NFA&& nfa,
             FATraitsLe<NFA> eps = FATraitsLe<NFA>()) {
        NFA ret(std::move(nfa));
        ret.finish().emplace(eps, ret.start());
        return ret;
    }
} // namespace sb