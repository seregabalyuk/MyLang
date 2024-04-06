#pragma once

#include <vector>
#include <map>
#include <set>

#include "../Class/Concepts.hpp"

namespace sb {
    template<C_FA InFA, C_FA OutFA = InFA>
    OutFA Minimize(const InFA& fa) {
        using InState = typename FATraits<InFA>::ConstState;
        using OutState = typename FATraits<OutFA>::State;
        using Type = typename FATraits<InFA>::Type;
        using Char = typename FATraits<InFA>::Char;

        using Chars = typename std::vector<Char>;
        using SetChar = typename std::set<Char>;

        using Group = std::set<InState>;
        using Groups = std::vector<Group>;
        using T2G = std::map<Type, size_t>;

        using Queue = std::vector<size_t>;

        using In2Out = std::map<InState, OutState>;
        
        Groups groups;
        Chars chars;
        Queue queue;
        { // Starting groups
            T2G t2g;
            SetChar schar;

            for (auto state = fa.begin(); state != fa.end(); ++ state) {
                if (!t2g.count(state.type())) {
                    t2g.emplace(state.type(), groups.size());
                    groups.emplace_back();
                }
                groups[t2g.at(state.type())].emplace(state);

                for (auto [letter, nextState]: state) {
                    schar.emplace(letter);
                }
            }
            /*if (!t2g.count(Type())) {
                t2g.emplace(Type(), groups.size());
                groups.emplace_back();
            }
            groups[t2g.at(Type())].emplace(fa.end());*/

            chars.reserve(schar.size());
            for (auto letter: schar) {
                chars.emplace_back(letter);
            }

            for (size_t i = 0; i < groups.size(); ++ i) {
                queue.emplace_back(i);
            }
        }
        
        { // Find groups
            while(queue.size()) {
                auto& spliter = groups[queue.back()];
                queue.pop_back();
                for (auto letter: chars) {
                    for (size_t i = 0; i < groups.size(); ++ i) {
                        auto& group = groups[i];
                        Group group_in;
                        Group group_out;
                        for (auto& state: group) {
                            InState next = fa.end();
                            if (/*state != fa.end() &&*/ state.count(letter)) {
                                next = state[letter];
                            }
                            if (spliter.count(next)) {
                                group_in.emplace(state);
                            } else {
                                group_out.emplace(state);
                            }
                        }
                        if (group_in.size() > 0 && group_out.size() > 0) {
                            group = std::move(group_in);
                            queue.push_back(groups.size());
                            groups.emplace_back(std::move(group_out));
                            
                        }
                    }
                }
            }
        }

        OutFA ret;
        In2Out in2out;


        { // Add state in ret
            for (auto& group: groups) {
                auto outState = ret.emplace();
                for (const auto& inState: group) {
                    /*if (inState != fa.end()) {*/
                        outState.type() |= inState.type();
                        in2out.emplace(inState, outState);
                    /*}*/
                }
            }
        }

        { // Add transition in ret
            size_t counter = 0;
            for (auto inState = fa.begin(); inState != fa.end(); ++ inState, ++ counter) {
                auto outState = in2out.at(inState);
                for (auto [letter, nextInState]: inState) {
                    auto nextOutState = in2out.at(nextInState);
                    outState.emplace(letter, nextOutState);
                }
            }
        }
        
        return ret;
    }

} // namespace sb