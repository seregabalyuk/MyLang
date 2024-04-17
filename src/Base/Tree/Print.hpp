#pragma once

#include "Tree.hpp"
#include <string>

namespace sb {
    std::string to_string(const Tree::Vertex& vertex) {
        if (vertex.type() == typeid(std::string)) {
            return Tree::castVertex<std::string>(vertex);
        } else {
            return "<none>";
        }
    }
    
    template<class ostream>
    void dfsPrintTree(ostream& out, const Tree::Vertex& vertex, std::vector<size_t>& tabs) {
        std::string str = to_string(vertex);
        out << str;
        if (vertex.count_sons()) {
            tabs.back() += str.size() + 1;
            if (vertex.count_sons() == 1) {
                out << "───";
                tabs.back() += 2;
                dfsPrintTree(out, vertex[0], tabs);
                tabs.back() -= 2;
            } else {
                
                std::string str;
                for(size_t j = 0; j + 1 < tabs.size(); ++ j) {
                    str += std::string(tabs[j], ' ') + "│";
                }
                str += std::string(tabs.back(), ' ');
                tabs.emplace_back(1);
                out << "─┬─";
                size_t i = 0;
                dfsPrintTree(out, vertex[i ++], tabs);
                for (; i + 1 < vertex.count_sons(); ++ i) {
                    out << str << "├─";
                    dfsPrintTree(out, vertex[i], tabs);
                }
                out << str << "└─";
                tabs.pop_back();
                tabs.back() += 2;
                dfsPrintTree(out, vertex[i], tabs);
                tabs.back() -= 2;
            }
            tabs.back() -= str.size() + 1;
        } else {
            out << '\n';
        }
    }

    template<class ostream>
    ostream& printTree(ostream& out, const Tree& tree) {
        std::vector<size_t> tabs(1, 0);
        dfsPrintTree(out, tree.root(), tabs);
        return out;
    }
} // namespace sb