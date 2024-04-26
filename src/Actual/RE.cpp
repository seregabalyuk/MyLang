#include "RE.hpp"

#include <RE/PosixRPN.hpp>
#include <RE/PosixPlusRPN.hpp>
#include <FA/Algo/NFA2DFA.hpp>

namespace sb::act {
  // local
    posix::TableRPN& gTable(void* table) {
        return *reinterpret_cast<posix::TableRPN*>(table);
    }

    posix::RPN& gRPN(void* rpn) {
        return *reinterpret_cast<posix::RPN*>(rpn);
    }

    posix::CreatorNFA& gCreator(void* creator) {
        return *reinterpret_cast<posix::CreatorNFA*>(creator);
    }

    posix::DFA& gDFA(void* dfa) {
        return *reinterpret_cast<posix::DFA*>(dfa);
    }
  // global
   // RERPN
    RERPN::RERPN(void* rpn, void* dfa, void* creator, void* table):
        _rpn(rpn),
        _creator(creator),
        _dfa(dfa),
        _table(table)
    {}

    RERPN::~RERPN() {
        delete &gTable(_table);
        delete &gCreator(_creator);
        delete &gDFA(_dfa);
        delete &gRPN(_rpn);
    }

    void RERPN::put(char c) {
        gRPN(_rpn).put(c);
    }

    DFA RERPN::get() {
        auto nfa = gRPN(_rpn).get();
        auto dfa = nfa2dfa<sb::FA<0, char, char>>(nfa);
        return dfa;
    }
   // other
    RERPN makeRE() {
        auto [r, d, c, t] = makePosixRPN();
        return {r, d, c, t};
    }

    RERPN makePlusRE() {
        auto [r, d, c, t] = makePosixPlusRPN();
        return {r, d, c, t};
    }
} // namespace sb::act