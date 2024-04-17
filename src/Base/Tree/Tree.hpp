#pragma once

#include <vector>

namespace sb {
    class Tree {
      // classes
        class _BaseVertex;
        template<class T>
        class _VertexT;
        template<class T, class Alloc>
        class _Vertex;
      // _BaseVertex
        class _BaseVertex {
          // friends
            friend Tree;
          // members
            _BaseVertex* _parent;
        protected:
          // metods
            virtual void deallocate() = 0;
            virtual void emplace(_BaseVertex& son) = 0;
        public:
          // metods
           // constructors
            _BaseVertex(): _parent(this) {}
            _BaseVertex(_BaseVertex& parent): _parent(&parent) {
                _vertexEmplace(parent, *this);
            }
           // virtual
            virtual ~_BaseVertex() {}
           // virtual = 0;
            virtual size_t count_sons() const = 0;
            virtual bool has_value() const = 0;
            virtual const std::type_info& type()const = 0;
            virtual _BaseVertex& operator[](size_t i) = 0;
            virtual const _BaseVertex& operator[](size_t i) const = 0;
            virtual void swap_sons(size_t i1, size_t i2) = 0;
            virtual void erase(size_t i) = 0;
           // other
            _BaseVertex& parent() { return *_parent; }
            const _BaseVertex& parent() const { return *_parent; }
        };
      // _VertexT
        template<class T>
        class _VertexT: public _BaseVertex {
          // members
            [[no_unique_address]]T _value;
        public:
          // metods
           // constructors
            template<class... Args>
            _VertexT(_BaseVertex& parent, Args&&... args): 
                _BaseVertex(parent),
                _value(std::forward<Args>(args)...) {}
            template<class... Args>
            _VertexT(char, Args&&... args): 
                _BaseVertex(),
                _value(std::forward<Args>(args)...) {}
           // virtual
            virtual ~_VertexT() {}
           // override
            bool has_value() const override { return !std::is_same_v<T, void>; }
            const std::type_info& type() const override { return typeid(T); }
           // other
            T& get() { return _value; }
            const T& get() const { return _value; }
            
        };
      // _Vertex
        template<class T, class Allocator>
        class _Vertex: public _VertexT<T>  {
          // usings
           // allocator
            template<class U>
            using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
            template<class U>
            using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
            template<class U>
            using _Ptr = _AllocTraits<U>::pointer;
           // vertex
            using _PtrV = _Ptr<_BaseVertex>;
            using _Sons = std::vector<_PtrV, _Alloc<_PtrV>>;
          // members
            _Sons _sons;
        protected:
          // metods
            void deallocate() override {
                _Alloc<Vertex> meAlloc(_sons.get_allocator());
                _Ptr<Vertex> me = this;
                _AllocTraits<Vertex>::destroy(meAlloc, this);
                _AllocTraits<Vertex>::deallocate(meAlloc, me, 1);
            }
            void emplace(_BaseVertex& son) override {
                _sons.emplace_back(&son);
            }
        public:
          // metods
           // constructors and operator = 
            template<class... Args>
            _Vertex(_BaseVertex& parent, const Allocator& alloc, Args&&... args): 
                _VertexT<T>(parent, std::forward<Args>(args)...),
                _sons(alloc) {}
            template<class... Args>
            _Vertex(const Allocator& alloc, Args&&... args): 
                _VertexT<T>('\0', std::forward<Args>(args)...),
                _sons(alloc) {}
           // virtual
            virtual ~_Vertex() {
                for (auto& son: _sons) {
                    son->deallocate();
                }
            }
           // override
            _BaseVertex& operator[](size_t i) override {
                return *_sons[i];
            }

            const _BaseVertex& operator[](size_t i) const override {
                return *_sons[i];
            }

            size_t count_sons() const override {
                return _sons.size();
            }

            void swap_sons(size_t i1, size_t i2) override {
                std::swap(_sons[i1], _sons[i2]);
            }

            void erase(size_t i) override {
                _sons[i]->deallocate();
            }
        };
      // metods
       // static
        template<class U, class Alloc, class... Args>
        static U* _allocate(const Alloc& alloc, Args&&... args) {
            using _Alloc = typename std::allocator_traits<Alloc>::template rebind_alloc<U>;
            using _AllocTraits = typename std::allocator_traits<Alloc>::template rebind_traits<U>;
            using _Ptr = _AllocTraits::pointer;
            _Alloc _alloc(alloc);
            _Ptr ptr = _AllocTraits::allocate(_alloc, 1);
            _AllocTraits::construct(_alloc, &*ptr, std::forward<Args>(args)...);
            return &*ptr;
        }
        static void _vertexEmplace(_BaseVertex& parent, _BaseVertex& son) {
            parent.emplace(son);
        }
      // members
        _BaseVertex* _root = nullptr;
    public:
      // usings
        using Vertex = _BaseVertex;
      // metods
       // local
        Tree() {}
        ~Tree() {
            _root->deallocate();
        }
        bool empty() { return _root == nullptr; }
        Vertex& root() { return *_root; }
        const Vertex& root() const { return *_root; }
       // static
        template<class T, class Alloc, class... Args>
        static Vertex& allocVertex(Tree& tree, const Alloc& alloc, Args&&... args) {
            Vertex* ptr = _allocate<_Vertex<T, Alloc>>(alloc, alloc, std::forward<Args>(args)...);
            if (!tree.empty()) {
                tree._root->deallocate();
            }
            tree._root = ptr;
            return *ptr;
        }

        template<class T, class Alloc, class... Args>
        static Vertex& allocVertex(Vertex& vertex, const Alloc& alloc, Args&&... args) {
            Vertex* ptr = _allocate<_Vertex<T, Alloc>>(alloc, vertex, alloc, std::forward<Args>(args)...);
            return *ptr;
        }

        template<class T, class... Args>
        static Vertex& makeVertex(Tree& tree, Args&&... args) {
            std::allocator<T> alloc;
            return allocVertex<T>(tree, alloc, std::forward<Args>(args)...);
        }

        template<class T, class... Args>
        static Vertex& makeVertex(Vertex& vertex, Args&&... args) {
            std::allocator<T> alloc;
            return allocVertex<T>(vertex, alloc, std::forward<Args>(args)...);;
        }

        template<class T>
        static T& castVertex(Vertex& vertex) {
            return static_cast<_VertexT<T>&>(vertex).get();
        }

        template<class T>
        static const T& castVertex(const Vertex& vertex) {
            return static_cast<const _VertexT<T>&>(vertex).get();
        }

    };
} // namespace sb

