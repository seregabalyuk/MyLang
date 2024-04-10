#pragma once


#include <Base/SetLink.hpp>
#include <FA/Class/Concept.hpp>

namespace sb {
    template<C_FA FA>
    using SetStateFA = SetLink<FATraitsSt<FA>>;
    template<C_FA FA>
    using SetConstStateFA = SetLink<const FATraitsSt<FA>>;
    
} // namespace sb