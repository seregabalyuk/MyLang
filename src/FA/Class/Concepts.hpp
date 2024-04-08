#pragma once

#include <concepts>

#include "../../TypeTraits/DeclVal.hpp"
#include "../../TypeTraits/Bindings.hpp"

namespace sb {
// Traits
    template<class Transition>
    struct TransitionFATraits {
        using PairType = std::remove_cvref_t<decltype(*std::declval<Transition>())>;
        using State = typename get_bindings<PairType, 2>::T2;
        using Char = typename get_bindings<PairType, 2>::T1;
    };

    template<class State>
    struct StateFATraits {
        using Transition = decltype(std::declval<State>().begin());
        using Char = typename TransitionFATraits<Transition>::Char;
        using Type = std::remove_cvref_t<decltype(std::declval<State>().type())>;

        static_assert(std::same_as<typename TransitionFATraits<Transition>::State, State>,
            "State not equal Transition::State");
    };

    template<class FA>
    struct FATraits {
        using State      = decltype(DeclLVal<FA>().start());
        using ConstState = decltype(DeclLVal<FA>().cstart());

        using Transition = typename StateFATraits<State>::Transition;
        using ConstTransition = typename StateFATraits<ConstState>::Transition;

        using Char = typename StateFATraits<State>::Char;
        using Type = typename StateFATraits<State>::Type;

        static_assert(std::same_as<typename StateFATraits<State>::Type, 
                                   typename StateFATraits<ConstState>::Type>,
            "Type in State and ConstState not equal");
        static_assert(std::same_as<typename StateFATraits<State>::Char, 
                                   typename StateFATraits<ConstState>::Char>,
            "Char in State and ConstState not equal");
        
    };

// Concepts
  // Type
    template<class Type>
    concept C_TypeFA = 
        std::is_copy_constructible_v<Type> &&
        std::is_copy_assignable_v<Type> &&
        requires(Type type, const Type c_type) {
            {c_type | c_type} -> std::same_as<Type>;
            {type |= c_type}  -> std::same_as<Type&>;
    };
  // Transition
    template<class Transition>
    concept C_TransitionFA = requires(Transition tran) {
        {*tran};
        {++ tran} -> std::same_as<Transition&>;
    };
  // State
    template<class State>
    concept C_BaseStateFA =
        C_TypeFA<typename StateFATraits<State>::Type> &&
        std::is_copy_constructible_v<State> &&
        std::is_copy_assignable_v<State> &&
        C_TransitionFA<typename StateFATraits<State>::Transition> &&
        std::same_as<typename TransitionFATraits<typename StateFATraits<State>::Transition>::State, State> &&
        requires(State state) {
            {state.begin()} -> std::same_as<typename StateFATraits<State>::Transition>;
            {state.end()}   -> std::same_as<typename StateFATraits<State>::Transition>;
    };

    template<class State>
    concept C_StateFA =
        C_BaseStateFA<State> &&
        requires(State state, typename StateFATraits<State>::Char letter) {
            {state.type()}   -> std::same_as<typename StateFATraits<State>::Type&>;
            {state.erase(letter, state)};
            {state.erase(letter)};
            {state.emplace(letter, state)};
    };

    template<class State>
    concept C_ConstStateFA =
        C_BaseStateFA<State> &&
        requires(State state, typename StateFATraits<State>::Char letter) {
            {state.type()}   -> std::same_as<const typename StateFATraits<State>::Type&>;
    };

    template<class State>
    struct __BracesNFAT {
        using Char = typename StateFATraits<State>::Char;
        using Range = decltype(std::declval<State>()[std::declval<Char>()]);
        using Iter = decltype(std::declval<Range>().begin());
        using Type = decltype(*std::declval<Iter>());
    };

    /*template<class State>
    concept C_BaseStateNFA = 
        std::same_as<typename __BracesNFAT<State>::Type, State> &&
        requires(typename __BracesNFAT<State>::Range type) {
            {type.begin()} -> std::same_as<typename __BracesNFAT<State>::Iter>;
            {type.end()}   -> std::same_as<typename __BracesNFAT<State>::Iter>; 
    };

    template<class State>
    concept C_StateNFA = 
        C_StateFA<State> &&
        C_BaseStateNFA<State>
    ;

    template<class State>
    concept C_ConstStateNFA = 
        C_ConstStateFA<State> &&
        C_BaseStateNFA<State>
    ;*/

    template<class State>
    struct __BracesDFAT {
        using Char = typename StateFATraits<State>::Char;
        using Type = decltype(std::declval<State>()[std::declval<Char>()]);
    };

    template<class State>
    concept C_StateDFA = 
        C_StateFA<State> &&
        std::same_as<State, typename __BracesDFAT<State>::Type>
    ;

    template<class State>
    concept C_ConstStateDFA = 
        C_ConstStateFA<State> &&
        std::same_as<State, typename __BracesDFAT<State>::Type>
    ;

  // FA
    template<class FA>
    concept C_FA = 
        C_StateFA<typename FATraits<FA>::State> &&
        C_ConstStateFA<typename FATraits<FA>::ConstState> &&
        std::is_default_constructible_v<FA> &&
        std::is_constructible_v<typename FATraits<FA>::ConstState, typename FATraits<FA>::State> &&
        requires(FA fa,
        const FA c_fa,
        typename FATraits<FA>::State state,
        typename FATraits<FA>::ConstState c_state,
        typename FATraits<FA>::Type type) {
            {c_fa.start()}       -> std::same_as<typename FATraits<FA>::ConstState>;
            {fa.cstart()}        -> std::same_as<typename FATraits<FA>::ConstState>;
            {fa.start()}         -> std::same_as<typename FATraits<FA>::State>;

            {fa.erase(c_state)};
            {fa.emplace()}       -> std::same_as<typename FATraits<FA>::State>;
            {fa.emplace(type)}   -> std::same_as<typename FATraits<FA>::State>;

            {c_state = state};
    };

    /*template<class FA>
    concept C_NFA = C_FA<FA> && 
        C_StateNFA<typename FATraits<FA>::State> &&
        C_ConstStateNFA<typename FATraits<FA>::ConstState>;
    */

    template<class FA>
    concept C_DFA = C_FA<FA> && 
        C_StateDFA<typename FATraits<FA>::State> &&
        C_ConstStateDFA<typename FATraits<FA>::ConstState>;

} // namespace sb
