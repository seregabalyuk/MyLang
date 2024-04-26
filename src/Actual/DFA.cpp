#include "DFA.hpp"
#include "RE.hpp"

#include <fstream>

#include <FA/FastDFA/Open.hpp>
#include <FA/FastDFA/Save.hpp>

namespace sb::act {
  // opens
    DFA openDFA(const std::string& filename) {
        std::ifstream fin(filename);
        DFA dfa;
        openFastDFA(fin, dfa);
        return dfa;
    }
  // saves
    void saveDFA(const std::string& filename, const DFA& dfa) {
        std::ofstream fout(filename);
        saveFastDFA(fout, dfa);
    }
} // namespace sb::act