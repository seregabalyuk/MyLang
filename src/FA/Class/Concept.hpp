#pragma once

#include <concepts>

#include <TypeTraits/C_IterableT.hpp>

#include "Traits.hpp"

namespace sb {
// Concepts
  // Type
    template<class Type>
    concept C_TypeFA = 
        std::is_copy_constructible_v<Type> &&
        std::is_copy_assignable_v<Type> &&
        requires(Type type, const Type c_type) 
    {
        {c_type | c_type} -> std::same_as<Type>;
        {type |= c_type}  -> std::same_as<Type&>;
    };
  // FA
    template<class FA>
    concept C_FA =
        C_IterableT<FA> &&
        C_IterableT<FATraitsSt<FA>> &&
        C_TypeFA<FATraitsTy<FA>> &&
    requires(FA fa, const FA cfa,

            FATraitsSt<FA> state,
            const FATraitsSt<FA> cstate,

            FATraitsTr<FA> trans,
            const FATraitsTr<FA> ctrans,

            FATraitsLe<FA> letter,
            const FATraitsLe<FA> cletter,

            FATraitsTy<FA> type,
            const FATraitsTy<FA> ctype,
            
            FATraitsItSt<FA> itst,
            FATraitsCItSt<FA> citst,
            
            FATraitsItTr<FA> ittr,
            FATraitsCItTr<FA> cittr) 
    {
    //FA
      // values
        {fa.start()} -> std::same_as<FATraitsSt<FA>&>;
        {cfa.start()} -> std::same_as<const FATraitsSt<FA>&>;
      // change
        {fa.emplace()} -> std::same_as<FATraitsSt<FA>&>;
        {fa.emplace(ctype)} -> std::same_as<FATraitsSt<FA>&>;

        {fa.erase(citst)};
      // parametrs
        {fa.size()} -> std::same_as<size_t>;
        {cfa.size()} -> std::same_as<size_t>;

        {fa.empty()} -> std::same_as<bool>;
        {cfa.empty()} -> std::same_as<bool>;
    // State
      // values
        {state.type()} -> std::same_as<FATraitsTy<FA>&>;
        {cstate.type()} -> std::same_as<const FATraitsTy<FA>&>;
      // change
        {state.emplace(cletter, state)} -> std::same_as<FATraitsTr<FA>&>;
        {state.erase(cletter, cstate)};
        {state.erase(cletter)};
      // parametrs
        {state.count(cletter)} -> std::same_as<size_t>;
        {state.size()} -> std::same_as<size_t>;
    // Transition
        {trans.letter()} -> std::convertible_to<FATraitsLe<FA>>;
        {ctrans.letter()} -> std::convertible_to<FATraitsLe<FA>>;

        {trans.next()} -> std::same_as<FATraitsSt<FA>&>;
        {ctrans.next()} -> std::same_as<const FATraitsSt<FA>&>;
    };
  // DFA
    template<class DFA>
    concept C_DFA = 
        C_FA<DFA> && 
    requires(
            FATraitsSt<DFA> state,
            const FATraitsSt<DFA> cstate,

            FATraitsLe<DFA> letter,
            const FATraitsLe<DFA> cletter) 
    {
        {state[cletter]} -> std::same_as<FATraitsSt<DFA>&>;
        {cstate[cletter]} -> std::same_as<const FATraitsSt<DFA>&>;
    };
  // NFA
    template<class NFA>
    concept C_NFA = 
        C_FA<NFA> && 
    requires(
            FATraitsSt<NFA> state,
            const FATraitsSt<NFA> cstate,

            FATraitsLe<NFA> letter,
            const FATraitsLe<NFA> cletter) 
    {
        {state[cletter].begin()} -> std::convertible_to<FATraitsItTr<NFA>>;
        {cstate[cletter].begin()} -> std::convertible_to<FATraitsCItTr<NFA>>;

        {state[cletter].end()} -> std::convertible_to<FATraitsItTr<NFA>>;
        {cstate[cletter].end()} -> std::convertible_to<FATraitsCItTr<NFA>>;
    };
} // namespace sb
