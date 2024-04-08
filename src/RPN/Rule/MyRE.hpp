#pragma once

#include <RPN/Class/RPN.hpp>
#include <RPN/Creator/NFA.hpp>
#include "NFAfunctions.hpp"

namespace sb {
    RPN<CreatorNFA<DefaultNFA>> ruleMyRE;

// Realization

    struct _StaticCreatorRuleMyRE {
        _StaticCreatorRuleMyRE() {
            ruleMyRE.addBinary('|', 2, concatDefault);
            ruleMyRE.addBinary('+', 1, alterDefault);
            ruleMyRE.addSufUnary('*', 3, kleeneDefault);
            ruleMyRE.addBrackets('(', ')');
            ruleMyRE.addEndUnary(finalyDefault);
        }
    };

    _StaticCreatorRuleMyRE _CreatorRuleMyRE;
} // namespace sb