#pragma once

#include <vector>
#include <map>

#include <FA/Class/Concepts.hpp>

namespace sb {
    template<class Ostream, C_FA FA>
    Ostream& printFA(Ostream& out, const FA& fa) {
        using State = typename FATraits<FA>::ConstState;
        std::vector<State> n2s;
        std::map<State, size_t> s2n;
        for (auto state = fa.begin(); state != fa.end(); ++ state) {
            s2n[state] = n2s.size();
            n2s.push_back(state);
        }
        for (size_t num = 0; num < n2s.size(); ++ num) {
            out << "n=" <<  num << ", t=" << n2s[num].type() << ": ";
            for (auto [letter, state]: n2s[num]) {
                if (letter) { out << "'" << letter << "'"; } else { out << "eps"; }
                out << "->" << s2n[state] << ", ";
            }
            out << '\n';
        }
        return out;
    }
} // namespace sb