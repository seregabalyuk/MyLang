#pragma once

namespace sb  {
    template<class T>
    struct Link2Ptr {

    };

    template<class T>
    using Link2Ptr = typename MakeLink<T>::type;
} // namespace sb