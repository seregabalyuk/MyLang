#pragma once

#include <vector>
#include <map>

#include <FA/Class/Concept.hpp>

namespace sb {
    template<class Ostream, C_FA FA>
    Ostream& printFA(Ostream& out, const FA& fa) {
        using CPtrState = const FATraitsSt<FA>*;
        std::vector<CPtrState> n2s;
        std::map<CPtrState, size_t> s2n;
        for (auto iter = fa.begin(); iter != fa.end(); ++ iter) {
            s2n[&*iter] = n2s.size();
            n2s.push_back(&*iter);
        }
        for (size_t num = 0; num < n2s.size(); ++ num) {
            out << "n=" <<  num << ", t=" << n2s[num]->type() << ": ";
            for (auto& trans: *n2s[num]) {
                if (trans.letter()) { 
                    out << "'" << trans.letter() << "'"; 
                } else { out << "eps"; }
                out << "->" << s2n[&trans.next()] << ", ";
            }
            out << '\n';
        }
        return out;
    }
} // namespace sb