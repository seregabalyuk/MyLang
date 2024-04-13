#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <FA/Class/Concept.hpp>

#include "SetStateFA.hpp"

namespace sb {
    template<C_FA FA>
    auto getAllLetter(const FA& fa) {
        using SetLetter = std::unordered_set<FATraitsLe<FA>>;
        SetLetter letters;
        for (auto& state: fa) {
            for (auto& trans: state) {
                letters.emplace(trans.letter());
            }
        }
        using Letters = std::vector<FATraitsLe<FA>>;
        Letters ret;
        ret.reserve(letters.size());
        for (auto letter: letters) { ret.emplace_back(letter); }
        return ret;
    }

    template<C_DFA OutFA, C_DFA InFA>
    OutFA minimize(const InFA& fa) {
        using State = FATraitsSt<InFA>;
        using Class = SetConstStateFA<InFA>;
        
        using QueClass = SetLink<Class>;
        using Classes = std::list<Class>;

        Classes classes;
        { // First classes
            using Type2Class = std::unordered_map<FATraitsTy<InFA>, Class*>;
            Type2Class type2class;
            for (auto& state: fa) {
                if (!type2class.count(state.type())) {
                    auto& _class = classes.emplace_back();
                    type2class.emplace(state.type(), &_class);
                }
                type2class[state.type()]->emplace(state);
            }
        }
        auto letters = getAllLetter(fa);
        QueClass queClass;
      #ifdef FA_ALGO_DEBUG
        // lambda
        auto drawQue = [&fa, &queClass] {
            std::map<const State*, size_t> s2i;
            size_t i = 0;
            for (auto& state: fa) {
                s2i[&state] = i ++;
            }
            std::cout << "Que {";
            for (auto& _class: queClass) {
                std::cout << '\n';
                for(auto& state: _class) {
                    std::cout << s2i[&state] << ' ';
                }
            }
            std::cout << "}\n";
        };
        auto drawClasses = [&fa, &classes] {
            std::map<const State*, size_t> s2i;
            size_t i = 0;
            for (auto& state: fa) {
                s2i[&state] = i ++;
            }
            std::cout << "Classes {";
            for (auto& _class: classes) {
                std::cout << '\n';
                for(auto& state: _class) {
                    std::cout << s2i[&state] << ' ';
                }
            }
            std::cout << "}\n";
        };
      #endif
        { // First que
            for(auto& _class: classes) {
                queClass.emplace(_class);
            }
        }

        { // find partition  FA
            while(queClass.size()) {
                auto& spliter = *queClass.begin();
                queClass.erase(spliter);
                for (auto letter: letters) {
                    for (auto& _class: classes) {
                        //auto& _class = classes[i];
                        Class left, right;
                        // split by spliter and letter
                        for (auto& state: _class) {
                            if (state.count(letter) && spliter.count(state[letter])) {
                                left.emplace(state);
                            } else {
                                right.emplace(state);
                            }
                        }
                        // emplace in queClass
                        if (left.size() && right.size()) {
                            if (queClass.count(_class)) {
                                queClass.erase(_class);
                                _class.swap(left);
                                auto& back = classes.emplace_back(std::move(right));
                                queClass.emplace(_class);
                                queClass.emplace(back);
                            } else {
                                _class.swap(left);
                                auto& back = classes.emplace_back(std::move(right));
                                if (back.size() < _class.size()) {
                                    queClass.emplace(back);
                                } else {
                                    queClass.emplace(_class);
                                }
                            }
                        } // emplace in queClass
                    } // for by classes
                } // for by letters
            } // while
        }

        OutFA ret;
        { // Create ret by classes
            using In2Out = std::unordered_map<const State*, FATraitsSt<OutFA>*>;
            In2Out translator;
            // class with start make first
            auto iter = classes.begin();
            while ( iter != classes.end() && !iter->count(fa.start())) {
                ++ iter;
            }
            classes.emplace_front(std::move(*iter));
            classes.erase(iter);
            // add state in out
            for (auto& _class: classes) {
                auto& stateOut = ret.emplace();
                for (auto& stateIn: _class) {
                    translator[&stateIn] = &stateOut;
                }
            }
            // add transition in out
            for (auto& _class: classes) {
                auto& stateIn = *_class.begin();
                auto& stateOutPrev = *translator[&stateIn];
                stateOutPrev.type() = stateIn.type();
                for (auto& trans: stateIn) {
                    auto& stateOutNext = *translator[&trans.next()];
                    stateOutPrev.emplace(trans.letter(), stateOutNext);
                }
            }
        }
        return ret;
    }

} // namespace sb