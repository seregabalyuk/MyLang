#pragma once

#include "FA.hpp"

namespace sb {
    using DefaultDFA = FA<0, void, char, uint64_t, std::allocator<void>>;
} // namespace sb