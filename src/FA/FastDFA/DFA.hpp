#pragma once

#include <vector>
#include <tuple>
#include <map>
#include <algorithm>

#include <FA/Class/Concept.hpp>

namespace sb {
    template<class Letter, class Type, class Allocator = std::allocator<size_t>>
    struct FastDFA {
    public:
      // classes
        struct State;
        struct Transition;
    private:
      // usings
        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        template<class U>
        using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
        template<class U>
        using _Ptr = _AllocTraits<U>::pointer;

        using _Transitions = std::vector<Transition, _Alloc<Transition>>;
        using _States = std::vector<State, _Alloc<State>>;
    public:
      // functions
        FastDFA(const Allocator& alloc = Allocator()):
            _states(alloc) {}

        template<C_DFA DFA>
        FastDFA(const DFA& prototype, const Allocator& alloc = Allocator()):
            _states(alloc) {
            _make(prototype);
        }

        const State& start() const { 
            return _states.front();
        }

        const State& cstart() const { 
            return _states.front();
        }

        State& start() { 
            return _states.front();
        }

        auto begin() const { return _states.begin(); }
        auto end() const { return _states.begin(); }

        auto begin() { return _states.begin(); }
        auto end() { return _states.begin(); }
        
        const State& operator[](size_t index) const {
            return _states[index];
        }

        State& operator[](size_t index) {
            return _states[index];
        }

        size_t size() const {
            return _states.size();
        }

        const _States& vec() const { return _states; }
        _States& vec() { return _states; }
      // State
        struct State {
        public:
          // functions
            State(
                const _States& states,
                const Type& type,
                const Allocator& alloc = Allocator()
                ):
                _states(&states),
                _type(type),
                _trans(alloc)
            {}
            const Type& type() const {
                return _type;
            };
            Type& type() {
                return _type;
            };

            auto begin() const {
                return _trans.begin();
            }
            auto end() const {
                return _trans.end();
            }
            auto begin() {
                return _trans.begin();
            }
            auto end() {
                return _trans.end();
            }

            size_t size() const {
                return _trans.size();
            }

            inline size_t count(Letter letter) const {
                return 1;
            }

            const State& operator[](Letter letter) const {
                size_t l = 0, r = _trans.size();
                while (l + 1 < r) {
                    size_t c = (l + r) / 2;
                    if (_trans[c].letter() <= letter) {
                        l = c;
                    } else {
                        r = c;
                    }
                }
                return parent()[_trans[l].number()];
            };

            const _Transitions& vec() const { return _trans; }
            _Transitions& vec() { return _trans; }

            const _States& parent() const {
                return *_states;
            }
        private:
          // members
            _Transitions _trans;
            const _States* _states;
            Type _type;
        };
      // Transition
        struct Transition {
        public:
            Transition(Letter letter, size_t num, const State& state):
                _letter(letter),
                _number(num),
                state(&state)
            {}
            Transition(const Transition& other):
                _letter(other._letter),
                _number(other._number),
                state(other.state) 
            {}
            Letter letter() const { return _letter; }
            size_t number() const { return _number; }
            const State& next() const {
                return state->parent()[number()];
            }
            bool operator<(const Transition& other) const {
                return _letter < other._letter;
            }
        private:
          // members
            size_t _number;
            const State* state;
            Letter _letter;
        };
    private:
      // functions
        template<C_DFA DFA>
        void _allocate(const DFA& dfa) {
            _states.reserve(dfa.size() + 1);
            for (auto& state: dfa) {
                _states.emplace_back(_states, state.type(), _states.get_allocator());
            }
            _states.emplace_back(_states, Type(), _states.get_allocator());
        }

        template<C_DFA DFA>
        auto _createSt2Num(const DFA& dfa) {
            std::map<const FATraitsSt<DFA>*, size_t> st2num;
            size_t i = 0;
            for (auto& state: dfa) {
                st2num[&state] = i++;
            }
            return st2num;
        }

        template<C_DFA DFA>
        void _make(const DFA& dfa) {
            _allocate(dfa);
            auto st2num = _createSt2Num(dfa);
            size_t end = dfa.size();
            size_t i = 0;
            for (auto& dfa_state: dfa) {
                auto& state = _states[i++];
                for (auto& trans: dfa_state) {
                    state.vec().emplace_back(
                        trans.letter(),
                        st2num[&trans.next()],
                        state
                    );
                }
                std::sort(state.vec().begin(), state.vec().end());
                _Transitions good;
                good.emplace_back(Letter(), end, state);
                if (state.vec().size()) {
                    good.emplace_back(state.vec().front());
                    for (size_t i = 1; i < state.vec().size(); ++ i) {
                        auto l1 = state.vec()[i - 1].letter();
                        auto n1 = state.vec()[i - 1].number();
                        auto l2 = state.vec()[i].letter();
                        auto n2 = state.vec()[i].number();
                        if (l1 + 1 == l2 && n1 != n2) {
                            good.emplace_back(l2, n2, state);
                        } 
                        if (l1 + 1 < l2) {
                            good.emplace_back(l1 + 1, end, state);
                            good.emplace_back(l2, n2, state);
                        }
                    }
                    auto l = state.vec().back().letter();
                    good.emplace_back(l + 1, end, state);
                }
                state.vec() = std::move(good);
            }
            _states.back().vec().emplace_back(Letter(), end, _states.back());
        }
      // members
        _States _states;
    };

    template<C_DFA DFA, class Alloc>
    FastDFA(const DFA&, const Alloc&) -> FastDFA<FATraitsLe<DFA>, FATraitsTy<DFA>, Alloc>;

    template<C_DFA DFA>
    FastDFA(const DFA&) -> FastDFA<FATraitsLe<DFA>, FATraitsTy<DFA>>;
}; 