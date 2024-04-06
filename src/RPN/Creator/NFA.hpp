#pragma once

#include "../../FA/Class/Concepts.hpp"

namespace sb {
    template<C_NFA NFA>
    class CreatorNFA {
    private:
    public:
      // usings
        using Char = FATraits<NFA>::Char;
      // function
        CreatorNFA() { _nfa.emplace(); }

        void emplace(Char letter) {
            auto finish = _nfa.finish();
            _nfa.emplace();
            finish.emplace(letter, _nfa.finish());
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
    };
} // namespace sb