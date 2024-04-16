#pragma once

#include <forward_list>

namespace sb {
    template<
        class T,
        class Name = std::string,
        class Allocator = std::allocator<T>
    > struct CFG {
    public:
      // classes
        class Rule;
        class Symbol;
        class Terminal;
        class NonTerminal;
    private:
      // usings
        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        template<class U>
        using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
        template<class U>
        using _Ptr = _AllocTraits<U>::pointer;

        using _Symbols = std::forward_list<Symbol, _Alloc<Symbol>>;
        using _Rules = std::forward_list<Rule, _Alloc<Rule>>;
    public:
      // Symbol
        class Symbol {
        public:
            bool isTerminal() {
                return _isTerminal;
            }
            bool isNonTerminal() {
                return !_isTerminal;
            }
        private:
          // members
            bool _isTerminal;
            _Ptr<void> _ptr;
        };
    private:
       _Symbols symbols;
       _Rules rules;
    };
} // namespace sb