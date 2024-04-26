#pragma once

#include <Base/Tree/Tree.hpp>
#include <Base/Link.hpp>

namespace sb {
    template<class CFG, class String>
    Tree CYK(const CFG& cfg, const String& str);
// realization
    template<class CFG, class Iter>
    Tree CYK(const CFG& cfg, const String& str) {
        using Symbol = Link<typename CFG::Symbol>;
        using Situate = Link<typename std::pair<Symbol, size_t>>;


        for (size_t l = 0; l < str.size(); ++ l) {
            for (size_t r = l + 1; r < str.size(); ++ r) {
                for (auto& t: cfg.terminals()) {

                }
                for (auto& nt: cfg.nonterminals()) {
                    
                }
            }
        }
    }
} // namespace sb