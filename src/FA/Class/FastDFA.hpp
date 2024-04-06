#pragma once

#include <vector>
#include <algorithm>

namespace sb {
    template<
        class T,
        class Char,
        class Allocator
    >
    struct FastDFA {
    private:
      // classes
        class _Node;
        class _
      // usings
        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;

        using _Nodes = std::vector<_Node, _Alloc<_Node>>;
    public:

    private:
      // memory
        _Nodes _nodes;
      // _Node
        struct _Node {
          // usings
            using _Nodes = std::vector<size_t, _Alloc<size_t>>;
            using _Chars = std::vector<Char, _Alloc<Char>>;
          // functions
            auto getNodeIter(auto iterChar) const {
                return nodes.begin() + static_cast<size_t>(iterChar - chars.begin());
            }

            void addTransition(Char letter, size_t id) {
                auto iterChar = std::lower_bound(chars.begin(), chars.end(), letter);
                if (iterChar == chars.end() || *iterChar != letter) {
                    auto iterNode = getNodeIter(iterChar);
                    chars.emplace(iterChar, letter);
                    nodes.emplace(iterNode, id);
                }
            }

            size_t next(Char letter) const {
                auto iterChar = std::lower_bound(chars.begin(), chars.end(), letter);
                if (iterChar != chars.end() && *iterChar == letter) {
                    auto iterNode = getNodeIter(iterChar);
                    return *iterNode;
                }
                return -1LL;
            }
          // memory
            _Nodes nodes;
            _Chars chars;
        };
      // _
    };
};