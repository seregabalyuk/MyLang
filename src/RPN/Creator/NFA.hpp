#pragma once

#include <FA/Class/Concept.hpp>

namespace sb {
    template<C_NFA NFA>
    class CreatorNFA {
    private:
    public:
      // usings
        using Letter = FATraitsLe<NFA>;
      // function
        CreatorNFA() { _nfa.emplace(); }

        void emplace(Letter letter) {
            switch(state) {
            case 0: // default state
                switch(letter) {
                case '\\':
                    state = 1;
                    break;
                case '[':
                    state = 2;
                    _nfa.emplace();
                    break;
                default:
                    auto& finish = _nfa.finish();
                    _nfa.emplace();
                    finish.emplace(letter, _nfa.finish());
                    break;
                }
                break;
            case 1: // if later was /
                switch(letter) {
                case 'e':
                    break;
                default:
                    auto& finish = _nfa.finish();
                    _nfa.emplace();
                    finish.emplace(letter, _nfa.finish());
                    break;
                }
                state = 0;
                break;
            case 2: // in [ ]
                switch(letter) {
                case ']':
                    state = 0;
                    break;
                case '-':
                    state = 3;
                    break;
                default:
                    auto& prevF = *(-- (--_nfa.end()));
                    prevF.emplace(letter, _nfa.finish());
                    helper = letter;
                    break;
                }
                break;
            case 3: // in [ - ]
                auto& prevF = *(-- (--_nfa.end()));
                for (Letter it = helper + 1; it != letter + 1; ++ it) {
                    prevF.emplace(it, _nfa.finish());
                }
                state = 2;
                break;
            }
            
        }

        NFA get() {
            NFA out = std::move(_nfa);
            _nfa.emplace();
            return out;
        }

        bool empty() { return _nfa.size() <= 1; }

        void clear() {
            _nfa.clear();
            _nfa.emplace();
        }
    private:
        NFA _nfa;
        Letter helper;
        size_t state = 0;
    };
} // namespace sb