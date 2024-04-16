#pragma once

#include <vector>
#include <list>
#include <string>

namespace sb {
    template<
        class T,
        class Name = std::string,
        class Allocator = std::allocator<T>
    >
    struct CFG {
    public:
      // classes
        struct Symbol;
        struct Terminal;
        struct NonTerminal;
        struct Rule;
    private:
      // usings
        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        template<class U>
        using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
        template<class U>
        using _Ptr = _AllocTraits<U>::pointer;

        using _Symbols = std::list<_Ptr<Symbol>, _Alloc<_Ptr<Symbol>>>;
        using _Rules = std::list<Rule, _Alloc<Rule>>;
    public:
      // functions
       // constructors and destructor
        CFG() = default;
        CFG(const Allocator& alloc): _symbols(alloc) {}

        ~CFG() {
            for (auto ptrSymbol: _symbols) {
                ptrSymbol->_deallocate(ptrSymbol, _symbols.get_allocator());
            }
        }
       // change
        template<class ...Args>
        Terminal& emplace_terminal(const Name& name, Args&&... args) & {
            auto ptr = _allocate<Terminal>(name, std::forward<Args>(args)...);
            _symbols.emplace_back(ptr);
            return *ptr;
        }

        NonTerminal& emplace_nonterminal(const Name& name) & {
            auto ptr = _allocate<NonTerminal>(name);
            _symbols.emplace_back(ptr);
            return *ptr;
        }

        Rule& emplace_rule(const NonTerminal& nonterminal) & {
            return const_cast<NonTerminal&>(nonterminal).emplace_rule();
        }
       // getters
        _Rules& rules(const NonTerminal& nonterminal) & {
            return const_cast<NonTerminal&>(nonterminal).rules();
        }

        const _Rules& rules(const NonTerminal& nonterminal) const & {
            return nonterminal.rules();
        }

        Symbol& start() & { return _symbols.front(); }
        const Symbol& start() const & { return _symbols.front(); }
        const Symbol& cstart() const & { return _symbols.front(); }

        auto begin() & { return _symbols.begin(); }
        auto begin() const & { return _symbols.cbegin(); }
        auto cbegin() const & { return _symbols.cbegin(); }

        auto end() & { return _symbols.end(); }
        auto end() const & { return _symbols.cend(); }
        auto cend() const & { return _symbols.cend(); }

        size_t size() const & { return _symbols.size(); }

      // Symbol
        struct Symbol {
        public:
          // functions
            Symbol(const Name& name): _name(name) {}

            Name& name() & { return _name; }
            const Name& name() const & { return _name; }


            virtual bool is_terminal() const & = 0;


            Terminal& get_terminal() & { 
                return static_cast<Terminal&>(*this);
            }
            const Terminal& get_terminal() const & {
                return static_cast<const Terminal&>(*this);
            }


            NonTerminal& get_nonterminal() & { 
                return static_cast<NonTerminal&>(*this);
            }
            const NonTerminal& get_nonterminal() const & {
                return static_cast<const NonTerminal&>(*this);
            }
            
            virtual ~Symbol() {}
        protected:
          // functions
            virtual void _deallocate(_Ptr<Symbol> self, Allocator alloc) & = 0;
        private:
          // friends
            friend CFG;
          // members
            Name _name;
        };
      // Terminal
        struct Terminal: public Symbol {
        public:
          // functions
            template<class... Args>
            Terminal(const Name& name, Args&&... args): Symbol(name), _value(std::forward<Args>(args)...) {}

            bool is_terminal() const & override { return 1; }

            T& operator *() & { return _value; }
            const T& operator *() const & { return _value; }

            T* operator ->() & { return &_value; }
            const T* operator ->() const & { return &_value; }
        private:
          // functions
            void _deallocate(_Ptr<Symbol> self, Allocator alloc) & override {
                using This = Terminal;
                auto thisPtr = static_cast<_Ptr<This>>(self);
                _Alloc<This> localAlloc(alloc);
                This* realPtr = &*thisPtr;
                _AllocTraits<This>::destroy(localAlloc, realPtr);
                _AllocTraits<This>::deallocate(localAlloc, thisPtr, 1);
            }
          // usings
            using Symbol::get_nonterminal;
            using Symbol::get_terminal;
          // friends
            friend CFG;
          // members
            T _value;
        };
      // NonTerminal
        struct NonTerminal: public Symbol {
        private:
           // usings
            using Symbol::get_nonterminal;
            using Symbol::get_terminal;
        public:
          // functions
            NonTerminal(const Name& name): Symbol(name) {}

            bool is_terminal() const & override { return 0; }

            Rule& emplace_rule() & {
                Rule& rule = _rules.emplace_back();
                rule._arg = this;
                return rule;
            }

            _Rules& rules() & {
                return _rules;
            }

            const _Rules& rules() const & {
                return _rules;
            }
        private:
          // functions
            void _deallocate(_Ptr<Symbol> self, Allocator alloc) & override {
                using This = NonTerminal;
                auto thisPtr = static_cast<_Ptr<This>>(self);
                _Alloc<This> localAlloc(alloc);
                This* realPtr = &*thisPtr;
                _AllocTraits<This>::destroy(localAlloc, realPtr);
                _AllocTraits<This>::deallocate(localAlloc, thisPtr, 1);
            }
          // members
            _Rules _rules;
        };
      // Rule
        struct Rule {
        private:
          // classes
            template<bool isConst>
            struct _BaseIter;
          // usings
            using _Symbols = std::vector<_Ptr<Symbol>, _Alloc<_Ptr<Symbol>>>;
          // friends
            friend NonTerminal;
        public:
          // usings
            using iterator = _BaseIter<0>;
            using const_iterator = _BaseIter<1>;
          // functions
            NonTerminal& argument() & { return *_arg; }
            const NonTerminal& argument() const & { return *_arg; }

            iterator begin() & { return _symbols.begin(); }
            const_iterator begin() const & { return _symbols.cbegin(); }
            const_iterator cbegin() const & { return _symbols.cbegin(); }

            iterator end() & { return _symbols.end(); }
            const_iterator end() const & { return _symbols.cend(); }
            const_iterator cend() const & { return _symbols.cend(); }

            auto emplace_back(const Symbol& symbol) & {
                return _symbols.emplace_back(&const_cast<Symbol&>(symbol));
            }
          // operators
           // []
            Symbol& operator [](size_t i) & { return *_symbols[i]; }
            const Symbol& operator [](size_t i) const & { return *_symbols[i]; }
            
          
        private:
          // _BaseIter
            template<bool isConst>
            struct _BaseIter {
            private:
              // usings
                using _Iter = std::conditional_t<isConst, 
                    typename _Symbols::const_iterator, 
                    typename _Symbols::iterator>;
                using _Link = std::conditional_t<isConst, const Symbol&, Symbol&>;
                using _Ptr = std::conditional_t<isConst, const Symbol*, Symbol*>;
              // friends
                friend Rule;
                friend _BaseIter<1>;
              // members
                _Iter _iter;
              // constructors
                _BaseIter(_Iter iter): _iter(iter) {}
            public:
              // constructors
                template<bool isOtherConst>
                _BaseIter(const _BaseIter<isOtherConst>& other): _iter(other._iter) {}
              // operators
               // = 
                template<bool isOtherConst>
                _BaseIter& operator =(const _BaseIter<isOtherConst>& other) & {
                    _iter = other._iter;
                    return *this;
                }
               // * and ->
                _Link operator*() const & { return **_iter; }
                _Ptr operator->() const & { return *_iter; }
               // changing
                _BaseIter& operator ++() & {
                    ++ _iter;
                    return *this;
                }

                _BaseIter operator ++(int) & {
                    return _iter++;
                } 

                _BaseIter& operator --() & {
                    -- _iter;
                    return *this;
                }

                _BaseIter operator --(int) & {
                    return _iter--;
                }
               // compair
                bool operator == (const _BaseIter& other) const {
                    return _iter == other._iter;
                }

                bool operator != (const _BaseIter& other) const {
                    return _iter != other._iter;
                }

                auto operator <=> (const _BaseIter& other) const {
                    return _iter <=> other._iter;
                }

            };
          // members
            NonTerminal* _arg;
            _Symbols _symbols;
        };
    private:
      // functions
        template<class U, class... Args>
        _Ptr<U> _allocate(Args&&... args) {
            _Alloc<U> alloc =_symbols.get_allocator();
            _Ptr<U> ptr = _AllocTraits<U>::allocate(alloc, 1);
            _AllocTraits<U>::construct(alloc, &*ptr, std::forward<Args>(args) ...);
            return ptr;
        }
      // members
        _Symbols _symbols;
    };
} // namespace sb