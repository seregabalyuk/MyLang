#pragma once

#include "FA.hpp"
#include "Concepts.hpp"

namespace sb {
    using DefaultNFA = FA<1, void, char, uint64_t, std::allocator<void>>;
    
    template<C_NFA NFA>
    NFA concat(NFA&& left,
               NFA&& right,
               typename FATraits<NFA>::Char eps = typename FATraits<NFA>::Char()) {
        NFA ret;
        left.finish().emplace(eps, right.start());
        ret.splice(left);
        ret.splice(right);
        return ret;
    }

    template<C_NFA NFA>
    NFA alter(NFA&& left,
              NFA&& right,
              typename FATraits<NFA>::Char eps = typename FATraits<NFA>::Char()) {
        NFA ret;
        auto finishL = left.finish();
        auto startR = right.start();
        ret.splice(left);
        ret.splice(right);
        finishL.emplace(eps, ret.finish());
        ret.start().emplace(eps, startR);
        /*
        auto start = ret.emplace();
        start.emplace(eps, left.start());
        start.emplace(eps, right.start());
        auto finishL = left.finish();
        auto finishR = right.finish();
        ret.splice(left);
        ret.splice(right);
        auto finish = ret.emplace();
        finishL.emplace(eps, finish);
        finishR.emplace(eps, finish);
        */
        return ret;
    }

    template<C_NFA NFA>
    NFA kleene(NFA&& nfa,
               typename FATraits<NFA>::Char eps = typename FATraits<NFA>::Char()) {
        NFA ret(std::move(nfa));
        ret.start().emplace(eps, ret.finish());
        ret.finish().emplace(eps, ret.start());
        return ret;
    }

    template<C_NFA NFA>
    NFA plus(NFA&& nfa,
             typename FATraits<NFA>::Char eps = typename FATraits<NFA>::Char()) {
        NFA ret(std::move(nfa));
        ret.finish().emplace(eps, ret.start());
        return ret;
    }
} // namespace sb