#pragma once

#include "DFA.hpp"

namespace sb::act {
    class RERPN;

    RERPN makeRE();
    RERPN makePlusRE();
// realization
    class RERPN {
      // members
        void* _rpn;
        void* _creator;
        void* _dfa;
        void* _table;
    public:
      // functions
        RERPN(void*, void*, void*, void*);
        void put(char);
        DFA get();
        ~RERPN();
    };
} // namespace sb::act