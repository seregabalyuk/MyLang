#pragma once

#include <vector>
#include <fstream>
#include <sstream>

#include <FA/Class/Concept.hpp>
#include <FA/Class/FA.hpp>

///////////////////////////////
//
//  123 
//   |   
// type
//  
// 2:a 3: 4:s
//  |
// number state:letter
// 
// special letter \a - all char
// special letter \e - eps char
// 
// every 2 line new state
// numbering start 0
//
//////////////////////////////

namespace sb {
    template<C_FA FA>
    void simpleReadFileFA(std::ifstream& fin, FA& fa) {
        using State = FATraitsSt<FA>;
        using Letter = FATraitsLe<FA>;
        std::string str;
        std::vector<State*> i2s;

        auto get = [&i2s, &fa] (size_t i) -> State& {
            while(i2s.size() <= i) {
                State& state = fa.emplace();
                i2s.emplace_back(&state);
            }
            return *i2s[i];
        };

        size_t id = 0;

        while (fin) {
            std::getline(fin, str, '\n');
            std::stringstream ss(std::move(str), std::ios_base::in);
            auto& state = get(id);
            ss >> str;
            if (str == "sp") {
                state.type() = ' ';
            } else {
                state.type() = str[0];
            }
            while(ss) {
                ss >> str;
                size_t i = 0;
                size_t nextId = 0;
              // find nextId
                for (; i < str.size(); ++ i) {
                    if (str[i] == ':') {
                        break;
                    } else {
                        nextId *= 10;
                        nextId += str[i] - '0';
                    }
                }
                auto& nextState = get(nextId);
              // find letter
                ++ i;
                if (i < str.size()) {
                    if (str[i] == '\\') {
                        ++ i;
                        if (i < str.size())
                        switch(str[i]) {
                        case 'a': // all letter
                          // https://www.industrialnets.ru/files/misc/ascii.pdf
                            for (char i = ' '; i != '~'; ++ i) {
                                state.emplace(i, nextState);
                            }
                            break;
                        case 'e': // zero transition
                            state.emplace(0, nextState);
                            break;
                        case '\\': // \ transition
                            state.emplace('\\', nextState);
                            break;
                        }
                    } else {
                        state.emplace(str[i], nextState);
                    }
                } else {
                    state.emplace(0, nextState);
                }
            } // while(ss)
            ++ id;
        } // while (fin)
    }
};



