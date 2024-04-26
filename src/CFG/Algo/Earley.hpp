#pragma once

#include <list>
#include <vector>
#include <set>

#include <Base/Link.hpp>

namespace sb {
    template<class CFG>
    class Earley {
      // classes
        struct _SttnCFG; // Sttn is situation
        struct _CompareCFG;
        struct _SttnDFA; 
      // usings
        using _State = Link<const typename CFG::Type::State>;
        using _Rule = Link<const typename CFG::Rule>;
        using _Symbol = Link<const typename CFG::Symbol>;
        using _Terminal = Link<const typename CFG::Terminal>;
        using _Letter = typename CFG::Letter;

        using _SttnL = std::multiset<_SttnCFG>;
        using _ParseL = std::vector<_SttnL>;
        using _StateL = std::list<_SttnDFA>;
        
      // _SttnCFG
        struct _SttnCFG {
            _Rule rule;
            size_t point;
            size_t begin;
            _Symbol next() const {
                if (point < rule().size()) {
                    return rule()[point];
                } else {
                    return nullptr;
                }
            }
            bool operator <(const _SttnCFG& other) {
                return next() < other.next();
            }
        };
      // _SttnDFA
        struct _SttnDFA {
            _State state;
            size_t begin;
            _Terminal term;
        };
      // metods
        void checkStates(_Letter chr) {
            for (auto iter = _states.begin(); iter != _states.end();){
                auto& state = *iter;
                state.state = state.state()[chr];
                if (state.term().value().finish() == state.state) {
                    _states.erase(iter ++);
                } else {
                    ++ iter;
                }
            }
        }

        void scan() {
            if (index == 0) { return; }
            for (auto& state: _states) {
                if (state.state().type()) {
                    auto [begin, end] = _parselist[state.begin].equal_range(state.term);
                    for (; begin != end; ++ begin) {
                        auto rule = begin->rule;
                        auto point = begin->point + 1;
                        auto start = begin->begin;
                        _parselist[index].emplace(rule, point, start);
                    }
                }
            }
        }

        void complete() {
            
        }

        void predict() {

        }
      // members
        _ParseL _parselist;
        const CFG* _cfg;
        size_t index = 0;
        _StateL _states;
    public:
      // metods
        void put(_Letter chr) {
            
            ++ index;
        }
    };
} // namespace sb