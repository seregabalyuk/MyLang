#pragma once

#include <forward_list>
#include <vector>

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

        using _Terminals = std::forward_list<Terminal, _Alloc<Terminal>>;
        using _NonTerminals = std::forward_list<NonTerminal, _Alloc<NonTerminal>>;

        using _Rules = std::forward_list<Rule, _Alloc<Rule>>;
        using _Symbols = std::vector<_Ptr<Symbol>, _Alloc<_Ptr<Symbol>>>;
    public:
      // usings
        using Type = T;
      // metods
        CFG(const Allocator& alloc = Allocator()):
            _terminals(alloc),
            _nonterminals(alloc)
        {}
        template<class... Args>
        auto emplace_terminal(Args&&... args) -> Terminal& {
            auto& ref = _terminals.emplace_front(std::forward<Args>(args)...);
            if (_start == nullptr) {
                _start = &ref;
            }
            return ref;
        }
        template<class... Args>
        auto emplace_nonterminal(Args&&... args) -> NonTerminal& {
            auto& ref = _nonterminals.emplace_front(
                _nonterminals.get_allocator(),
                std::forward<Args>(args)...
            );
            if (_start == nullptr) {
                _start = &ref;
            }
            return ref;
        }
        auto emplace_rule(NonTerminal& nonterminal) -> Rule&{
            return nonterminal.rules().emplace_front(
                nonterminal,
                _nonterminals.get_allocator()
            );
        }

        const _Terminals& terminals() const { return _terminals; }
        _Terminals& terminals() { return _terminals; }
        
        const _NonTerminals& nonterminals() const { return _nonterminals; }
        _NonTerminals& nonterminals() { return _nonterminals; }
        
        _Rules& rules(NonTerminal& nonterminal) {
            return nonterminal.rules();
        }
        const _Rules& rules(const NonTerminal& nonterminal) const {
            return nonterminal.rules();
        }
      // Symbol
        class Symbol {
        public:
            bool isTerminal() {
                return _isTerminal;
            }
            bool isNonTerminal() {
                return !_isTerminal;
            }
            Terminal& getTerminal() {
              return *static_cast<Terminal*>(this);
            }
            const Terminal& getTerminal() const {
              return *static_cast<const Terminal*>(this);
            }
            NonTerminal& getNonTerminal() {
              return *static_cast<NonTerminal*>(this);
            }
            const NonTerminal& getNonTerminal() const {
              return *static_cast<const NonTerminal*>(this);
            }
        protected:
          // metods
            Symbol(bool isTerminal):
                _isTerminal(isTerminal) {}
          // members
            bool _isTerminal;
        };
      // Terminal
        class Terminal: public Symbol {
		public:
            template<class... Args>
            Terminal(Args&&... args):
                _value(std::forward<Args>(args)...),
                Symbol(true)
            {}
			T& value() {
				return _value;
			}
			const T& value() const {
				return _value;
			}
        private:
            T _value;
		};
	  // NonTerminal
	  	class NonTerminal: public Symbol {
	    public:
          // metods
            NonTerminal(const Allocator& alloc, const Name& name):
                _name(name),
                _rules(alloc),
                Symbol(false)
            {}
            NonTerminal(const Allocator& alloc, Name&& name):
                _name(std::move(name)),
                _rules(alloc),
                Symbol(false)
            {}
            
			_Rules& rules() {
				return _rules;
			}
			const _Rules& rules() const {
				return _rules;
			}
            const Name& name() const {
                return _name;
            }
        private:
          // members
            Name _name;
            _Rules _rules;
		};
      // Rule
        class Rule {
        public:
            Rule(NonTerminal& arg, const Allocator& alloc):
                _argument(&arg),
                _symbols(alloc)
            {}

            const NonTerminal& argument() const {
                return *_argument;
            }
            const Symbol& operator[](size_t index) const {
                return _symbols[index];
            }

            NonTerminal& argument() {
                return *_argument;
            }
            Symbol& operator[](size_t index) {
                return *_symbols[index];
            }

            size_t size() const { return _symbols.size(); }

            auto begin() { return _symbols.begin(); }
            auto end() { return _symbols.end(); }

            auto begin() const { return _symbols.begin(); }
            auto end() const { return _symbols.end(); }

            auto cbegin() const { return _symbols.begin(); }
            auto cend() const { return _symbols.end(); }
            
            void emplace_back(Symbol& other) {
                _symbols.emplace_back(&other);
            }
        private:
            _Ptr<NonTerminal> _argument;
            _Symbols _symbols;
        };
    private:
	  // members
        _Terminals _terminals;
        _NonTerminals _nonterminals;
        _Ptr<Symbol> _start = nullptr;
    };
} // namespace sb