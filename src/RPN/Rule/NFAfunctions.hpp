#pragma once

#include <FA/Class/NFA.hpp>

namespace sb {
    sb::DefaultNFA concatDefault(sb::DefaultNFA&& left, sb::DefaultNFA&& right) {
        return sb::concat(std::move(left), std::move(right));
    }

    sb::DefaultNFA alterDefault(sb::DefaultNFA&& left, sb::DefaultNFA&& right) {
        return sb::alter(std::move(left), std::move(right));
    }

    sb::DefaultNFA plusDefault(sb::DefaultNFA&& one) {
        return sb::plus(std::move(one));
    }

    sb::DefaultNFA kleeneDefault(sb::DefaultNFA&& one) {
        return sb::kleene(std::move(one));
    }

    sb::DefaultNFA finalyDefault(sb::DefaultNFA&& one) {
        one.finish().type() = 1;
        return one;
    }
}