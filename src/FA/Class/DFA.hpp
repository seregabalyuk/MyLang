#pragma once

#include "FA.hpp"

namespace sb {
    using DefaultDFA = FA<void, char, uint64_t, std::allocator<void>, 0>;
} // namespace sb