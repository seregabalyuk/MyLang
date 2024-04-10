#pragma once

#include <RPN/Class/RPN.hpp>
#include <RPN/Creator/NFA.hpp>
#include "NFAfunctions.hpp"

namespace sb {
    RPN<CreatorNFA<NFA>> ruleMyRE;

// Realization

    struct _StaticCreatorRuleMyRE {
        _StaticCreatorRuleMyRE() {
            ruleMyRE.addBinary('|', 2, concatNFA);
            ruleMyRE.addBinary('+', 1, alterNFA);
            ruleMyRE.addSufUnary('*', 3, kleeneNFA);
            ruleMyRE.addBrackets('(', ')');
            ruleMyRE.addEndUnary(finalyNFA);
        }
    };

    _StaticCreatorRuleMyRE _CreatorRuleMyRE;
} // namespace sb