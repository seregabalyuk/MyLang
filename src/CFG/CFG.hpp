#pragma once

#include <vector>
#include <list>

namespace sb {
    template<
        class T,
        class Name,
        class Allocator
    >
    struct CFG {
    private:
      // classes
        struct _Node;
        struct _Rule;
        struct _TNode;
        struct _NonTNode;
      // iterators
        template<bool isConst>
        struct _BaseSymbol;
        template<bool isConst>
        struct _BaseTerminal;
        template<bool isConst>
        struct _BaseNonTerminal;
        template<bool isConst>
        struct _BaseRule;
      // usings
        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        template<class U>
        using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
        template<class U>
        using _Ptr = _AllocTraits<U>::pointer;

        using _TNodes = std::list<_TNode, _Alloc<_TNode>>;
        using _NonTNodes = std::list<_NonTNode, _Alloc<_NonTNode>>;
        
    public:
    private:
      // memory
        _TNodes _tNodes;
        _NonTNodes _nonTNodes;

      // _Node
        struct _Node {
            Name name;
            virtual bool is_terminal() const = 0;
            virtual ~_Symbol() {}
        };
      // _TNode
        struct _TNode: public _Node {
            bool is_terminal() const override { return 1; }
            T value;
        };
      // _NonTNode
        struct _NonTNode: public _Node {
            using _Rules = std::list<_Rule, _Alloc<_Rule>>;
            bool is_terminal() const override { return 0; }
        };
      // _Rule 
        struct _Rule {
            using _PtrNode = _Ptr<_Node>;
            using _Nodes = std::vector<_PtrNode, _Alloc<_PtrNode>>;
            _Nodes nodes;
            _NonTNode parent;
        };
     // iterators
      // _BaseSymbol
        template<bool isConst>
        struct _BaseSymbol {
        private:
          // usings
            using _PtrNode = std::conditional_t<isConst, _Ptr<const _Node>, _Ptr<_Node>>;
            using _LinkName = std::conditional_t<isConst, const Name&, Name&>;
          // memory
            _PtrNode _ptrNode;

            friend CFG;
            friend _BaseSymbol<1>;

            _BaseSymbol(_PtrNode ptrNode): _ptrNode(ptrNode) {}
        public:
            template<bool isOtherConst>
            _BaseSymbol(const _BaseSymbol<isOtherConst>& other): _ptrNode(other->_ptrNode) {}

            bool is_terminal() { return _ptrNode->is_terminal(); }
            _LinkName name() { return _ptrNode->name; }

            _BaseTerminal<isConst> get_terminal() {
                return makeTerminal<isConst>(_ptrNode);
            }

            _BaseNonTerminal<isConst> get_nonterminal() {
                return makeNonTerminal<isConst>(_ptrNode);
            }

        };
      // _BaseTerminal
        template<bool isConst>
        struct _BaseTerminal {
        private:
          // usings
            using _PtrTNode = std::conditional_t<isConst, _Ptr<const _TNode>, _Ptr<_TNode>>;
            using _LinkT = std::conditional_t<isConst, const T&, T&>;
            using _LinkName = std::conditional_t<isConst, const Name&, Name&>;
            using _PtrT = std::conditional_t<isConst, const T*, T*>;
          // memory
            _PtrTNode _ptrTNode;

            friend CFG;
            friend _BaseTerminal<1>;

            _BaseTerminal(_PtrTNode ptrTNode): _ptrTNode(ptrTNode) {}
        public:
            template<bool isOtherConst>
            _BaseTerminal(const _BaseTerminal<isOtherConst>& other): _ptrTNode(other->_ptrTNode) {}

            _LinkT operator*() { return _ptrTNode->value; }
            _PtrT operator->() { return &_ptrTNode->value; }

            bool is_terminal() { return _ptrTNode->is_terminal(); }
            _LinkName name() { return _ptrTNode->name; }
        };

        template<bool isConst>
        static _BaseTerminal<isConst> makeTerminal(typename _BaseSymbol<isConst>::_PtrNode ptr) {
            return static_cast<typename _BaseTerminal<isConst>::_PtrTNode>(ptr);
        }
      // _BaseNonTerminal
        template<bool isConst>
        struct _BaseNonTerminal {
        private:
          // usings
            using _PtrNonTNode = std::conditional_t<isConst, _Ptr<const _TNode>, _Ptr<_TNode>>;
            using _LinkName = std::conditional_t<isConst, const Name&, Name&>;
          // memory
            _PtrNonTNode _ptrNonTNode;

            friend CFG;
            friend _BaseNonTerminal<1>;

            _BaseNonTerminal(_PtrNonTNode ptrNonTNode): _ptrNonTNode(ptrNonTNode) {}
        public:
            template<bool isOtherConst>
            _BaseNonTerminal(const _BaseNonTerminal<isOtherConst>& other): _ptrNonTNode(other->_ptrNonTNode) {}

            bool is_terminal() { return _ptrNonTNode->is_terminal(); }
            _LinkName name() { return _ptrNonTNode->name; }
        };

        template<bool isConst>
        static _BaseNonTerminal<isConst> makeNonTerminal(typename _BaseSymbol<isConst>::_PtrNode ptr) {
            return static_cast<typename _BaseNonTerminal<isConst>::_PtrNonTNode>(ptr);
        }
      // _BaseRule
        
    };
} // namespace sb