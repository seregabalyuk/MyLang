#pragma once

#include <map>
#include <string>
#include <fstream>

#include <Actual/DFA.hpp>
#include <Actual/RE.hpp>
#include <Base/Link.hpp>

namespace sb {
    template<class CFG>
    void readFileCFG(std::ifstream& fin, CFG& cfg) {
        using Name2NT = std::map<std::string, Link<typename CFG::NonTerminal>>;
        using Rule = Link<typename CFG::Rule>;
        using NonTerminal = Link<typename CFG::NonTerminal>;
        using Terminal = Link<typename CFG::Terminal>;
        

        const auto dfa = act::openDFA("../src/CFG/Reader.dfa");
        auto re = act::makeRE();
        Link state = dfa.start();

        Name2NT name2nt;
        std::string name;

        Rule rule;
        bool isArg = 1;

        while(fin) {
            char c = fin.get();
            if (c == '\n' ||c == EOF) {
                name = "";
                state = dfa.start();
                isArg = 1;
                rule = nullptr;
            } else {
                if (!state().count(c)) { throw std::string{"state not find"};}
                state = state()[c];
                char type = state().type();
                //std::cout << c << "-" << type <<' ';
                //std::cout << name << '\n';
                switch (type) {
                case 'd':
                    re.put(c);
                    break;
                case 'F': { // need work
                    Terminal t = cfg.emplace_terminal(re.get());
                    if (rule.empty()) { throw "dfa cannot be argue"; }
                    rule().emplace_back(t());
                  } break;
                case 'n':
                    name += c;
                    break;
                case 'f': {
                    NonTerminal nt;
                    auto iter = name2nt.find(name);
                    if (iter == name2nt.end()) {
                        nt = cfg.emplace_nonterminal(name);
                        name2nt.emplace(std::move(name), nt);
                    } else {
                        nt = iter->second;
                    }
                    name = "";
                    
                    if (isArg) {
                        rule = cfg.emplace_rule(nt());                
                    } else {
                        rule().emplace_back(nt());
                    }
                  }  break;
                case '=':
                    isArg = 0;
                    break;
                default:
                    break;
                }
            }
        }
    }
};



