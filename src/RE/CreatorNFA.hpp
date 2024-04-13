#pragma once

#include <string>

#include <FA/Class/Concept.hpp>

namespace sb {
    template<C_NFA NFA, class Type>
    struct CreatorNFA {
    private:
      // usings
        using _Letter = FATraitsLe<NFA>;
    public:
      // functions
        template<class ...Args>
        CreatorNFA(Args&& ... args): _nfa(std::forward<Args>(args)...) {
            _default();
        }

        void putAcceptLetters(_Letter begin, _Letter end) {
            _begin = begin;
            _end = end;
        }

        void putLetterEmpty(_Letter empty) {
            _empty = empty;
        }

        void putTypeSimple(const Type& type) {
            _simple = type;
        }

        void putTypeReverse(const Type& type) {
            _reverse = type;
        }

        void putTypeLoop(const Type& type) {
            _loopInBrackets = type;
        }

        void putTypeInBrackets(const Type& type) {
            _inBrackets = type;
        }

        void putTypeOutBrackets(const Type& type) {
            _outBrackets = type;
        }

        void putTypeSpecial(const Type& type) {
            _special = type;
        }
      // for RPN
        void put(_Letter letter, const Type& type) try {
            if (type == _simple) {
                _nfa.start().emplace(letter, _nfa.finish());
                state = 1;
            } else if (type == _reverse) {
                for (_Letter iter = _begin; iter != _end; ++ iter) {
                    _nfa.start().emplace(iter, _nfa.finish());
                }
                state = 2;
            } else if (type == _inBrackets) {
                _addInBrackets(letter);
            } else if (type == _loopInBrackets) {
                size_t inloop = 0;
                constexpr size_t maxinloop = 1000;
                while (inloop ++ < maxinloop && ++_back != letter) {
                    _addInBrackets(_back);
                }
                if (inloop >= maxinloop) {
                    throw std::string{"not correct loop"};
                }
                _addInBrackets(_back);
            } else if (type == _outBrackets) {
                state = 1;
            } else if (type == _special) {
                if (letter == _empty) {
                    _nfa.start().emplace(_Letter(), _nfa.finish());
                } else {
                    _nfa.start().emplace(letter, _nfa.finish());
                }
                state = 1;
            }
            _back = letter;
        } catch (const std::string& error) {
            _default();
            throw std::string{"Creator put: "} + error;
        }

        bool empty() {
            return state != 1;
        }

        NFA get() {
            NFA ret = std::move(_nfa);
            _default();
            return ret;
        }

        void clear() {
            _nfa.clear();
            _default();
        }
    private:
      // functions
        void _addInBrackets(_Letter letter) {
            if (state == 2) {
                _nfa.start().erase(letter, _nfa.finish());
            } else {
                _nfa.start().emplace(letter, _nfa.finish());
            }
        }
        void _default() {
            state = 0;
            _nfa.emplace(); // add start state
            _nfa.emplace(); // add finish state
            _nfa.finish().type() = 1;
        }
      // members
        NFA _nfa;
        char state = 0;

        _Letter _back;
        _Letter _begin;
        _Letter _end;

        _Letter _empty;

        Type _simple;
        Type _reverse;
        Type _loopInBrackets;
        Type _inBrackets;
        Type _outBrackets;
        Type _special;
    };
} // namespace sb