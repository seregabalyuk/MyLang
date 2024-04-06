#pragma once

#include <unordered_set>
#include <list>
#include <cstdint>
#include "../../TypeTraits/Identity.hpp"

namespace sb {
    template<
        class T = void,
        class Char = char,
        class StateType = uint64_t,
        class Allocator = std::allocator<T>,
        bool IsNFA = true>
    class FA {
    private:
      // classes
        struct _Node;
        struct _Empty {};

        struct _Jump;
        struct _Hash;
        struct _EqualTo;
        
        template<bool Const, bool Braces>
        struct _BaseTransition;
        
        template<bool Const>
        struct _BaseState;

        template<bool Const>
        struct _BracesNFA;
      // usings
        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        
        using _ListNodes = std::list<_Node, _Alloc<_Node>>;
        using _SetJumps = std::unordered_set<_Jump, _Hash, _EqualTo, _Alloc<_Jump>>;

        using _IterNode = _ListNodes::iterator;
        using _CIterNode = _ListNodes::const_iterator;
        
        using _IterJump = _SetJumps::iterator;
        using _CIterJump = _SetJumps::const_iterator;

        template<bool Const>
        using _BracesType = std::conditional_t<IsNFA, _BracesNFA<Const>, _BaseState<Const>>;

        static constexpr bool _TisReal = !std::is_same_v<T, void>;
        using _ListT = std::conditional_t<_TisReal, std::list<T, _Alloc<T>>, _Empty>;

    public:
      // usings
        using State = _BaseState<0>;
        using ConstState = _BaseState<1>;

        using Transition = _BaseTransition<0, 0>;
        using ConstTransition = _BaseTransition<1, 0>;
      // functions
        FA() = default;
        FA(FA&& fa): _nodes(std::move(fa._nodes)) {}
        FA& operator=(FA&& fa) {
            _nodes.swap(fa._nodes);
            return *this;
        }

        State start() & { return _nodes.begin(); }
        ConstState start() const & { return _nodes.cbegin(); }
        ConstState cstart() const & { return _nodes.cbegin(); }

        State finish() & { return -- _nodes.end(); }
        ConstState finish() const & { return -- _nodes.end(); }
        ConstState cfinish() const & { return -- _nodes.end(); }

        State begin() & { return _nodes.begin(); }
        ConstState begin() const & { return _nodes.cbegin(); }
        ConstState cbegin() const & { return _nodes.cbegin(); }

        State end() & { return _nodes.end(); }
        ConstState end() const & { return _nodes.cend(); }
        ConstState cend() const & { return _nodes.cend(); }


        State emplace(const StateType& type = StateType()) & {
            _nodes.emplace_back(type);
            return -- _nodes.end();
        }

        void splice(FA& other) & { // other become empty
            _nodes.splice(_nodes.end(), other._nodes);
        }
        
        void erase(const ConstState& state) & {
            _nodes.erase(state._iter);
        }

        void clear() { _nodes.clear(); }

        size_t size() const { return _nodes.size(); }

        bool empty() const { return _nodes.empty(); }
    private:
      // _Node
        struct _Node {
            _SetJumps _jumps;
            StateType _type;
            [[no_unique_address]] _ListT _listT;
            _Node(const StateType& type): _type(type) {}
            _Node(_Node&& other) = default;
        };
      // _Jump
        struct _Jump {
            Char _char;
            _IterNode _nextNode;
        };

        struct _Hash {
            using is_transparent = void;
            uint64_t operator() (const _Jump& jmp)  const noexcept {
                std::hash<Char> hash;
                return hash(jmp._char);
            }

            uint64_t operator() (Char c)  const noexcept {
                std::hash<Char> hash;
                return hash(c);
            }
        };

        struct _EqualTo {
            using is_transparent = void;
            bool operator() (const _Jump& jmp1, const _Jump& jmp2) const noexcept {
                if (IsNFA) {
                    return jmp1._char == jmp2._char && jmp1._nextNode == jmp2._nextNode;
                } else {
                    return jmp1._char == jmp2._char;
                }
            }

            bool operator() (Char c, const _Jump& jmp) const noexcept {
                return jmp._char == c;
            }

            bool operator() (const _Jump& jmp, Char c) const noexcept {
                return jmp._char == c;
            }
        };
      // _Transition
        template<bool Const, bool Braces>
        struct _BaseTransition {
        private:
            using _Iter = std::conditional_t<Const, _CIterJump, _IterJump>;
            using _State = _BaseState<Const>;
            using _Pair = std::pair<Char, _State>;
            using _Once = _State;
            using _Ret = std::conditional_t<Braces, _Once, _Pair>;

            friend class _BaseTransition<0, Braces>;
            friend class FA;
            
            _Iter _iter;
            _BaseTransition(_Iter iter): _iter(iter) {}

            template<class Ret>
            Ret _helpOper(Identity<Ret>) const;

           
            _Pair _helpOper(Identity<_Pair>) const {
                return _Pair(_iter->_char, _makeState<Const>(_iter->_nextNode));
            }

            _Once _helpOper(Identity<_Once>) const {
                return _makeState<Const>(_iter->_nextNode);
            }
        public:
            template<bool OtherConst>
            _BaseTransition(const _BaseTransition<OtherConst, Braces>& other): _iter(other._iter) {}

            template<bool OtherConst>
            _BaseTransition& operator = (const _BaseTransition<OtherConst, Braces>& other) {
                _iter = other._iter;
                return *this;
            }

            _Ret operator*() const {
                return _helpOper(Identity<_Ret>());
            }

            _BaseTransition& operator++ () {
                ++_iter;
                return *this;
            }

            _BaseTransition operator++ (int) {
                return _iter ++;
            }

            _BaseTransition& operator-- () {
                --_iter;
                return *this;
            }

            _BaseTransition operator-- (int) {
                return _iter --;
            }

            template<bool OtherConst>
            bool operator != (const _BaseTransition<OtherConst, Braces>& other) {
                return _iter != _getIterByTrans(other);
            }

            template<bool OtherConst>
            bool operator == (const _BaseTransition<OtherConst, Braces>& other) {
                return _iter == _getIterByTrans(other);
            } 
        };

        template<bool Const, bool Braces>
        static _BaseTransition<Const, Braces> _makeTrans(const _BaseTransition<Const, Braces>::_Iter& iter) { return iter; }

        template<bool Const, bool Braces>
        static _BaseTransition<Const, Braces>::_Iter _getIterByTrans(const _BaseTransition<Const, Braces>& trans) { return trans._iter; }
      // _State need correct in BracesNFA
        template<bool Const>
        struct _BaseState {
        private:
            using _Iter = std::conditional_t<Const, _CIterNode, _IterNode>;
            using _Trans = _BaseTransition<Const, 0>;
            using _LinkType = std::conditional_t<Const, const StateType&, StateType&>;
            using _LinkListT = std::conditional_t<Const, const _ListT&, _ListT&>;
            using _PtrListT = std::conditional_t<Const, const _ListT*, _ListT*>;
            using _RetBraces = std::conditional_t<IsNFA, _BracesNFA<Const>, _BaseState>;

            friend class _BaseState<0>;
            friend class FA;

            _Iter _iter;
            _BaseState(_Iter iter): _iter(iter) {}

            template<class Ret>
            Ret _helpOper(Char letter, Identity<Ret>) const;

            _BracesNFA<Const> _helpOper(Char letter, Identity<_BracesNFA<Const>>) const {
                auto [begin, end] = _iter->_jumps.equal_range(letter);
                return _makeBracesNFA<Const>(begin, end);
            }

            _BaseState _helpOper(Char letter, Identity<_BaseState>) const {
                return _BaseState(_iter->_jumps.find(letter)->_nextNode);
            }
        public:
            template<bool OtherConst>
            _BaseState(const _BaseState<OtherConst>& other): _iter(other._iter) {}

            template<bool OtherConst>
            _BaseState& operator = (const _BaseState<OtherConst>& other) {
                _iter = other._iter;
                return *this;
            }

            _Trans begin() { return _makeTrans<Const, 0>(_iter->_jumps.begin()); }
            _Trans end() { return _makeTrans<Const, 0>(_iter->_jumps.end()); }
            
            _LinkType type() const { return _iter->_type; }


            void emplace(Char letter, const _BaseState<0>& state) {
                static_assert(!Const, "State must be non-const");
                _iter->_jumps.emplace(letter, state._iter);
            }

            void erase(Char letter, const _BaseState<0>& state) {
                static_assert(!Const, "State must be non-const");
                _iter->_jumps.erase(_Jump(letter, state._iter));
            }

            void erase(Char letter) {
                static_assert(!Const, "State must be non-const");
                auto [begin, end] = _iter->_jumps.equal_range(letter);
                _iter->_jumps.erase(begin, end);
            }

            size_t count(Char letter) const {
                return _iter->_jumps.count(letter);
            }

            _LinkListT operator* () {
                static_assert(_TisReal, "NFA isn't container");
                return _iter->_listT;
            }

            _PtrListT operator-> () {
                static_assert(_TisReal, "NFA isn't container");
                return &_iter->_listT;
            }

            _BaseState& operator++ () {
                ++_iter;
                return *this;
            }

            _BaseState operator++ (int) {
                return _iter ++;
            }

            _BaseState& operator-- () {
                --_iter;
                return *this;
            }

            _BaseState operator-- (int) {
                return _iter --;
            }

            bool operator == (const _BaseState& other) const {
                return _iter == other._iter;
            }

            bool operator != (const _BaseState& other) const {
                return _iter != other._iter;
            }

            bool operator < (const _BaseState& other) const {
                return reinterpret_cast<uint64_t>(&*_iter) <
                       reinterpret_cast<uint64_t>(&*other._iter);
            }

            _RetBraces operator[](Char letter) const {
                return _helpOper(letter, Identity<_RetBraces>());
            }
        };

        template<bool Const>
        static _BaseState<Const> _makeState(const _BaseState<Const>::_Iter& iter) { return iter; }
      // _BracesNFA
        template<bool Const>
        struct _BracesNFA {
        private:
            using _Iter = std::conditional_t<Const, _CIterJump, _IterJump>;
            using _Trans = _BaseTransition<Const, 1>;
            friend class _BracesNFA<0>;
            friend class FA;

            _Iter _begin, _end;
            _BracesNFA(_Iter begin, _Iter end): _begin(begin), _end(end) {}
        public:
            template<bool OtherConst>
            _BracesNFA(const _BracesNFA<OtherConst>& other):
                _begin(other._begin), 
                _end(other._end) {}
            
            template<bool OtherConst>
            _BracesNFA& operator = (const _BracesNFA<OtherConst>& other) {
                _begin = other._begin;
                _end = other._end;
                return *this;
            }

            _Trans begin() { return  _makeTrans<Const, 1>(_begin); }
            _Trans end() { return _makeTrans<Const, 1>(_end); }
        };

        template<bool Const>
        static _BracesNFA<Const> _makeBracesNFA(const _BracesNFA<Const>::_Iter& begin, 
                                                const _BracesNFA<Const>::_Iter& end) { return {begin, end}; }
      // memory
        _ListNodes _nodes;
    };
} // namespace sb
