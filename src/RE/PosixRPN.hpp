#pragma once

#include <tuple>

#include "CreatorNFA.hpp"

#include <FA/SimpleReadFile.hpp>
#include <FA/Class/FA.hpp>
#include <FA/Algo/FuncNFA.hpp>

#include <RPN/Class/RPN.hpp>
#include <RPN/Class/Table.hpp>

namespace sb {
    namespace posix {
        using Letter = char;
        using NFA = FA<1, Letter>;

        using Type = char;
        using DFA = FA<0, Letter, Type>;
        using CreatorNFA = CreatorNFA<NFA, Type>;
        using TableRPN = TableRPN<NFA, Letter>;
        using RPN = RPN<DFA, CreatorNFA, TableRPN>;

        void endUnary(NFA& nfa) {
            nfa.finish().type() = 1;
        }
    }
    
    auto makePosixRPN() {
      // make dfa
        posix::DFA* dfa = new posix::DFA();
        std::ifstream fin("../src/RE/PosixDFA.txt");
        simpleReadFileFA(fin, *dfa);
      // make creator
        posix::CreatorNFA* creator = new posix::CreatorNFA();
        creator->putAcceptLetters('!', '~'); // all letter
        // https://www.industrialnets.ru/files/misc/ascii.pdf
        creator->putTypeSimple('m');
        creator->putTypeReverse('r');
        creator->putTypeLoop('l');
        creator->putTypeInBrackets('b');
        creator->putTypeOutBrackets('p');
        creator->putTypeSpecial('s');
        creator->putLetterEmpty('e');
      // make table
        posix::TableRPN* table = new posix::TableRPN();
		    table->addBinary('\0', 1, concat<posix::NFA>);
        table->addBinary('|', 0, alter<posix::NFA>);
        table->addSufUnary('*', 2, kleene<posix::NFA>);
        table->addSufUnary('+', 2, plus<posix::NFA>);
        table->addSufUnary('?', 2, question<posix::NFA>);
        table->putEndUnary(posix::endUnary);
      // make rpn
        posix::RPN* rpn = new posix::RPN(*dfa, *creator, *table);
        rpn->putTypeTable('t');
        rpn->putTypeBrackets('o', 'c');
        rpn->putTypeEmpty(' ');
        return std::tuple{rpn, dfa, creator, table};
    }
};
