#pragma once

#include <Base/Tree/Tree.hpp>

namespace sb {
    template<class CFG, class Iter>
    Tree CYK(const CFG& cfg, Iter begin, Iter end);
// realization
    template<class CFG, class Iter>
    Tree CYK(const CFG& cfg, Iter begin, Iter end) {
        
    }
} // namespace sb