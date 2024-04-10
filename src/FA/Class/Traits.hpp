#pragma once

#include <type_traits>

#include <TypeTraits/DeclVal.hpp>
#include <TypeTraits/IterableTraits.hpp>


namespace sb {
// Traits
    template<class Transition>
    struct TransitionFATraits;
    // have
        // Letter
        // State
    //

    template<class State>
    struct StateFATraits;
    // have
        // Letter
        // Transition
        // Type
        // IteratorByTr
        // ConstIteratorByTr
    //

    template<class FA>
    struct FATraits;
    // have
        // Letter
        // Transition
        // Type
        // State
        // IteratorByTr
        // IteratorBySt
        // ConstIteratorByTr
        // ConstIteratorBySt
    //

    template<class FA>
    using FATraitsSt = typename FATraits<FA>::State;
    template<class FA>
    using FATraitsTr = typename FATraits<FA>::Transition;
    template<class FA>
    using FATraitsTy = typename FATraits<FA>::Type;
    template<class FA>
    using FATraitsLe = typename FATraits<FA>::Letter;
    template<class FA>
    using FATraitsItSt = typename FATraits<FA>::IteratorBySt;
    template<class FA>
    using FATraitsItTr = typename FATraits<FA>::IteratorByTr;
    template<class FA>
    using FATraitsCItSt = typename FATraits<FA>::ConstIteratorBySt;
    template<class FA>
    using FATraitsCItTr = typename FATraits<FA>::ConstIteratorByTr;

// realization
  // Transition
   // Base
    template<class Trans> 
    concept _C_BaseTransitionFA = // concept Transition FA letter
    requires(Trans trans, const Trans ctrans) {
        {trans.letter()};
        {ctrans.letter()};
        {trans.next()};
        {ctrans.next()};
    };

    template<class Transition, bool Check>
    struct _BaseTransitionFATraits { // traits Transition FA
        using Letter = void;
        using State = void;
    };

    template<class Transition>
    struct _BaseTransitionFATraits<Transition, 1> {
        using Letter = std::remove_cvref_t<decltype(DeclLVal<Transition>().letter())>;
        using State = std::remove_cvref_t<decltype(DeclLVal<Transition>().next())>;
    };
   // Traits
    template<class Transition>
    struct TransitionFATraits {
        using Letter = typename _BaseTransitionFATraits<
            Transition,
            _C_BaseTransitionFA<Transition>
            >::Letter;
        using State = typename _BaseTransitionFATraits<
            Transition,
            _C_BaseTransitionFA<Transition>
            >::State;
    };
  // State
   // Type
    template<class State> 
    concept _C_SFAT = // concept State FA Type
    requires(State state, const State cstate) {
        {state.type()};
        {cstate.type()};
    };

    template<class State, bool HaveType>
    struct _T_SFAT { // traits State FA Type
        using type = void;
    };

    template<class State>
    struct _T_SFAT<State, 1> {
        using type = std::remove_cvref_t<decltype(DeclLVal<State>().type())>;
    };
   // Traits
    template<class State>
    struct StateFATraits {
        using IteratorByTr = IterableTraitsIt<State>;
        using ConstIteratorByTr = IterableTraitsCIt<State>;
        using Transition = IterableTraitsT<State>;
        using Letter = typename TransitionFATraits<Transition>::Letter;
        using Type = typename _T_SFAT<State, _C_SFAT<State>>::type;
    };
  // FA
   // Traits
    template<class FA>
    struct FATraits {
        using IteratorBySt = IterableTraitsIt<FA>;
        using ConstIteratorBySt = IterableTraitsCIt<FA>;

        using State = IterableTraitsT<FA>;

        using IteratorByTr = typename StateFATraits<State>::IteratorByTr;
        using ConstIteratorByTr = typename StateFATraits<State>::ConstIteratorByTr;
        
        using Transition = typename StateFATraits<State>::Transition;

        using Letter = typename StateFATraits<State>::Letter;

        using Type = typename StateFATraits<State>::Type;
    };
} // namespace sb
