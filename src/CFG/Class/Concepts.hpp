#pragma once

#include <TypeTraits/DeclVal.hpp>

namespace sb {
    template<class Symbol>
    struct SymbolTraits {
        using Terminal = decltype(sb::DeclLVal<Symbol>().get_terminal());
        using NonTerminal = decltype(sb::DeclLVal<Symbol>().get_nonterminal());
    };

    template<class CFG>
    struct CFGTraits {
      // no const
        using Symbol = decltype(sb::DeclLVal<CFG>().start());
        using Terminal = typename SymbolTraits<Symbol>::Terminal;
        using NonTerminal = typename SymbolTraits<Symbol>::NonTerminal;
        using Rule = decltype(*sb::DeclLVal<CFG>().rules(sb::DeclLVal<NonTerminal>()).begin());
      // const
        using ConstSymbol = decltype(sb::DeclLVal<const CFG>().start());
        using ConstTerminal = typename SymbolTraits<ConstSymbol>::Terminal;
        using ConstNonTerminal = typename SymbolTraits<ConstSymbol>::NonTerminal;
        using ConstRule = decltype(*sb::DeclLVal<const CFG>().rules(sb::DeclLVal<ConstNonTerminal>()).begin());
    };
} // namespace sb