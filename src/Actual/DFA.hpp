#pragma once

#include <string>

#include <FA/FastDFA/DFA.hpp>
#include <FA/Class/Concept.hpp>

namespace sb::act {
    using DFA = FastDFA<char, char>;

    DFA openDFA(const std::string& filename);

    void saveDFA(const std::string& filename, const DFA& dfa);
} // namespace sb::act