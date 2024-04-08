#pragma once

namespace sb {
    template<class T>
    struct Identity { using type = T; };

    template<class T, T v>
    struct TypeConstant {};
} // namespace sb