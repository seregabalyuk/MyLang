#pragma once

#include "FA.hpp"
#include "Concepts.hpp"

namespace sb {
    using NFA = FA<1>;

    template<class Char>
    using NFA = FA<1, Char>;

    template<class Char, class StateType>
    using NFA = FA<1, Char, StateType>;

    template<class Char, class StateType, class Alloc>
    using NFA = FA<1, Char, StateType, Alloc>;
    
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