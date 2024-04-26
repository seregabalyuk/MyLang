#pragma once

#include <map>
#include <string>

#include <Actual/DFA.hpp>
#include <Actual/RE.hpp>
#include <Base/Link.hpp>

namespace sb {
    template<class Stream, class CFG>
    Stream& printCFG(Stream& out, const CFG& cfg) {
        using Name2NT = std::map<std::string, Link<typename CFG::NonTerminal>>;
        using Rule = Link<typename CFG::Rule>;
        using NonTerminal = Link<typename CFG::NonTerminal>;
        using Terminal = Link<typename CFG::Terminal>;
        for (auto& nt: cfg.nonterminals()) {
            for (auto& rule: nt.rules()) {
                std::cout << nt.name() << " -> ";
                for (auto& symbol: rule) {
                    if (symbol->isTerminal()) {
                        std::cout << "\"\" ";
                    } else {
                        std::cout << "<" << symbol->getNonTerminal().name() << "> ";
                    }
                }
                std::cout << '\n';
            }
            std::cout << '\n';
        }
        return out;
    }
};



