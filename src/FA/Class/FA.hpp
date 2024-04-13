#pragma once

#include <unordered_set>
#include <list>
#include <cstdint>
#include <ranges>

#include <TypeTraits/Identity.hpp>
#include <Base/SubRange.hpp>

namespace sb {
    template<
        bool IsNFA,
        class Char = char,
        class StateType = uint64_t,
        class Allocator = std::allocator<Char>
    > class FA {
    public:
      // classes
        struct State;
        struct Transition;  
    private:
      // classes
        struct _Hash;
        struct _EqualTo;
        struct _UnaryPred;
        struct _Compare;
        template<bool isConst>
        struct _BaseIter;
      // usings
        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        template<class U>
        using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
        template<class U>
        using _Ptr = _AllocTraits<U>::pointer;
        
        using _Transitions = std::unordered_set<Transition, _Hash, _EqualTo, _Alloc<Transition>>;
        using _TransIter = _Transitions::iterator;
        using _TransCIter = _Transitions::const_iterator;

        using _States = std::list<State, _Alloc<State>>;
    public:
      // usings
        using IteratorByTr = _BaseIter<0>;
        using ConstIteratorByTr = _BaseIter<1>;
      // functions
        FA() = default;
        FA(const Allocator& alloc): _states(alloc) {}
        FA(FA&& fa): _states(std::move(fa._states)) {}
        FA& operator=(FA&& fa) {
            _states.swap(fa._states);
            return *this;
        }

        State& start() & { return _states.front(); }
        const State& start() const & { return _states.front(); }

        State& finish() & { return _states.back(); }
        const State& finish() const & { return _states.back(); }

        auto begin() & { return _states.begin(); }
        auto begin() const & { return _states.begin(); }
        auto cbegin() const & { return _states.cbegin(); }
        
        auto end() & { return _states.end(); }
        auto end() const & { return _states.end(); }
        auto cend() const & { return _states.cend(); }

        auto erase(_States::const_iterator iter) & {
            return _states.erase(iter);
        }

        State& emplace() & {
            return _states.emplace_back();
        }

        State& emplace(const StateType& type) & {
            return _states.emplace_back(type);
        }

        void splice(FA&& other) & { // other become empty
            _states.splice(_states.end(), other._states);
        }

        void clear() { _states.clear(); }

        size_t size() const { return _states.size(); }

        bool empty() const { return _states.empty(); }
      // State
        struct State {
        private:
          // usings
            using _BracketsT = std::conditional_t<IsNFA, SubRange<IteratorByTr>, State&>;
            using _BracketsCT = std::conditional_t<IsNFA, SubRange<ConstIteratorByTr>, const State&>;
        public:
          // functions
            State() = default;
            State(const StateType& type): _type(type) {}

            auto begin() & { return makeIterTr<0>(_trans.begin()); }
            auto begin() const & { return makeIterTr<1>(_trans.begin()); }
            auto cbegin() const & { return makeIterTr<1>(_trans.cbegin()); }

            auto end() & { return makeIterTr<0>(_trans.end()); }
            auto end() const & { return makeIterTr<1>(_trans.end()); }
            auto cend() const & { return makeIterTr<1>(_trans.cend()); }

            StateType& type() & { return _type; }
            const StateType& type() const & { return _type; }
            
            template<class... Args>
            Transition& emplace(Args&& ... args) & {
                auto [iter, _] = _trans.emplace(std::forward<Args>(args)...);
                return *makeIterTr<0>(iter);
            }

            void erase(Char letter, const State& state) & {
                _trans.erase(Transition(letter, const_cast<State&>(state)));
            }

            void erase(Char letter) & {
                std::erase_if(_trans, _UnaryPred(letter));
            }

            size_t count(Char letter) const {
                return _trans.count(letter);
            }

            size_t size() const {
                return _trans.size();
            }
            
            _BracketsT operator [](Char letter) & {
                return Brackets(letter, TypeConstant<bool, IsNFA>());
            }

            _BracketsCT operator [](Char letter) const & {
                return Brackets(letter, TypeConstant<bool, IsNFA>());
            }
        private:
          // functions
            State&  Brackets(Char letter, TypeConstant<bool, 0>) {
                auto iter = makeIterTr<0>(_trans.find(letter));
                return iter->next();
            }

            const State&  Brackets(Char letter, TypeConstant<bool, 0>) const {
                auto iter = makeIterTr<1>(_trans.find(letter));
                return iter->next();
            }

            SubRange<IteratorByTr> Brackets(Char letter, TypeConstant<bool, 1>) {
                auto [begin, end] = _trans.equal_range(letter);
                return {makeIterTr<0>(begin), makeIterTr<0>(end)};
            }

            SubRange<ConstIteratorByTr>  Brackets(Char letter, TypeConstant<bool, 1>) const {
                auto [begin, end] = _trans.equal_range(letter);
                return {makeIterTr<1>(begin), makeIterTr<1>(end)};
            }
          // members
            _Transitions _trans;
            StateType _type;
        };
      // Transition
        struct Transition {
        public:
            Transition(Char letter, State& next): _letter(letter), _next(&next) {}
            Transition(Transition& other) = default;

            Char letter() const & { return _letter; }

            State& next() & { return *_next; }
            const State& next() const & { return *_next; }
        private:    
          // members
            Char _letter;
            _Ptr<State> _next;
        };
    private:
      // _Hash
        struct _Hash {
            using is_transparent = void;
            uint64_t operator() (const Transition& trans)  const noexcept {
                std::hash<Char> hash;
                return hash(trans.letter());
            }

            uint64_t operator() (Char c)  const noexcept {
                std::hash<Char> hash;
                return hash(c);
            }
        };
      // _EqualTo
        struct _EqualTo {
            using is_transparent = void;
            bool operator() (const Transition& trans1, const Transition& trans2) const noexcept {
                if (IsNFA) {
                    return trans1.letter() == trans2.letter() && &trans1.next() == &trans2.next();
                } else {
                    return trans1.letter() == trans2.letter();
                }
            }

            bool operator() (Char c, const Transition& trans) const noexcept {
                return c == trans.letter();
            }

            bool operator() (const Transition& trans, Char c) const noexcept {
                return trans.letter() == c;
            }
        };
      // _UnaryPred
        struct _UnaryPred {
            Char letter;
            bool operator()(const Transition& trans) const {
                return trans.letter() == letter;
            }
        };
      // _Compare
        struct _Compare {
            bool operator() (const Transition& trans1, const Transition& trans2) const noexcept {
                if (IsNFA) {
                    if (trans1.letter() == trans2.letter()) {
                        return &trans1.next() < &trans2.next();
                    }
                    return trans1.letter() < trans2.letter();
                } else {
                    return trans1.letter() < trans2.letter();
                }
            }
        };
      // _BaseIter
        template<bool isConst>
        struct _BaseIter {
        private:
          // using
            using _Iter = std::conditional_t<isConst, _TransCIter, _TransIter>;
            using _Link = std::conditional_t<isConst, const Transition&, Transition&>;
            using _Ptr = std::conditional_t<isConst, const Transition*, Transition*>;
          // friends
            friend FA;
            friend _BaseIter<1>;
          // member
            _Iter _iter;
            _BaseIter(_Iter iter): _iter(iter) {}
        public:
            template<bool OtherConst>
            _BaseIter(const _BaseIter<OtherConst>& other): _iter(other._iter) {}

            template<bool OtherConst>
            _BaseIter& operator = (const _BaseIter<OtherConst>& other) {
                _iter = other._iter;
                return *this;
            }

            _Link operator*() {
                return const_cast<_Link>(*_iter);
            }

            _Ptr operator->() {
                return const_cast<_Ptr>(&*_iter);
            }

            _BaseIter& operator++() {
                ++ _iter;
                return *this;
            }

            _BaseIter operator++(int) {
                return _iter ++;
            }

            template<bool OtherConst>
            bool operator != (const _BaseIter<OtherConst>& other) {
                return _iter != getIterTr(other);
            }

            template<bool OtherConst>
            bool operator == (const _BaseIter<OtherConst>& other) {
                return _iter == getIterTr(other);
            }

            template<bool OtherConst>
            auto operator <=>(const _BaseIter<OtherConst>& other) {
                return _iter <=> getIterTr(other);
            }
        };
      // functions
        template<bool isConst>
        static const auto& getIterTr(const _BaseIter<isConst>& iter) {
            return iter._iter;
        }

        template<bool isConst>
        static _BaseIter<isConst> makeIterTr(const typename _BaseIter<isConst>::_Iter& iter) {
            return iter;
        }
      // members
        _States _states;
    };
} // namespace sb
