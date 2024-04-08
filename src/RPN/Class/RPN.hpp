#pragma once

#include <functional>
#include <forward_list>
#include <vector>
#include <algorithm>

namespace sb {
    template<
        class Creator,
        class Char = char,
        class Allocator = std::allocator<Char>>
    class RPN {
    private:
      // classes
        struct _Operation;
        struct _Binary;
        template<bool Suf>
        struct _Unary;
        template<bool Open>
        struct _Brackets;
      // usings
        using _T = decltype(std::declval<Creator>().get());
        using _PreUnary = _Unary<0>;
        using _SufUnary = _Unary<1>;

        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        template<class U>
        using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
        template<class U>
        using _Ptr = _AllocTraits<U>::pointer;
        using _PtrO = _Ptr<_Operation>;
        using _PtrCO = _Ptr<const _Operation>;

        using _FuncUn = std::function<_T(_T&&)>;
        using _FuncBin = std::function<_T(_T&&, _T&&)>;

        using _SymbolToOparation = std::pair<Char, _PtrO>;

        using _VectorS2O = std::vector<_SymbolToOparation, _Alloc<_SymbolToOparation>>;
        
        using _StackT = std::forward_list<_T, _Alloc<_T>>;
        using _StackO = std::forward_list<_PtrCO, _Alloc<_PtrCO>>;
    public:
      // function
        RPN(): _s2o()  {
            _creator = _allocate<Creator>(_alloc());
        }

        RPN(const Allocator& alloc): _s2o(alloc) {
            _creator = _allocate<Creator>(_alloc());
        }

        template<class ...Args>
        RPN(const Allocator& alloc, Args&&... args): 
            _s2o(alloc) {
            _creator = _allocate<Creator>(_alloc(), std::forward<Args>(args) ...);
        }

        ~RPN() {
            auto alloc = _alloc();
            for (auto& s2o: _s2o) {
                s2o.second->deallocate(alloc, s2o.second);
            }
            _deallocate<Creator>(_alloc(), _creator);
        }
        
        void addBinary(Char symbol, uint priority, _FuncBin func) & {
            auto alloc = _alloc();
            _PtrO ptr = _allocate<_Binary>(alloc, priority, func);
            _s2o.emplace_back(symbol, ptr);
            std::sort(_s2o.begin(), _s2o.end());
        }

        void addBrackets(Char open, Char close) & {
            auto alloc = _alloc();
            _PtrO ptr1 = _allocate<_Brackets<1>>(alloc);
            _PtrO ptr2 = _allocate<_Brackets<0>>(alloc);
            _s2o.emplace_back(open, ptr1);
            _s2o.emplace_back(close, ptr2);
            std::sort(_s2o.begin(), _s2o.end());
        }

        void addSufUnary(Char symbol, uint priority, _FuncUn func) & {
            auto alloc = _alloc();
            _PtrO ptr = _allocate<_SufUnary>(alloc, priority, func);
            _s2o.emplace_back(symbol, ptr);
            std::sort(_s2o.begin(), _s2o.end());
        }

        void addPreUnary(Char symbol, uint priority, _FuncUn func) & {
            auto alloc = _alloc();
            _PtrO ptr = _allocate<_PreUnary>(alloc, priority, func);
            _s2o.emplace_back(symbol, ptr);
            std::sort(_s2o.begin(), _s2o.end());
        }

        void addEndBinary(_FuncBin func) & {
            _endFuncBin = func;
        }

        void addEndUnary(_FuncUn func) & {
            _endFuncUn = func;
        }

        template<class Istream>
        _T operator()(Istream& in) const {
            _creator->clear();
            _StackT stackT;
            _StackO stackO;
            while(in) {
                _processSymbol(in.get(),stackT, stackO);
            }
            return _completion(stackT, stackO);
        } 

        template<class Iter>
        _T operator()(Iter begin, Iter end) const {
            _creator->clear();
            _StackT stackT;
            _StackO stackO;
            for(; begin != end; ++ begin) {
                _processSymbol(*begin, stackT, stackO);
            }
            return _completion(stackT, stackO);
        } 
    private:
      // _Operator
        struct _Operation {
        private:
            int _priority;
        public:
            _Operation(int priority): _priority(priority) {}
            int& priority() { return _priority; }
            int priority() const { return _priority; }
            virtual ~_Operation() {}
            virtual void deallocate(Allocator alloc, _PtrO ptr) = 0;
            virtual void addStack(_PtrCO me,  _StackT& stackT, _StackO& stackO) const = 0;
            virtual void delStack(_StackT& stackT) const = 0;
        };
      // _Binary
        struct _Binary: public _Operation {
        private:
            using _Operation::priority;
            _FuncBin _func;
        public:
            _Binary(int priority, _FuncBin func): 
                _Operation(priority),
                _func(func) {}
            
            void deallocate(Allocator alloc, _PtrO ptr) override {
                using This = _Binary;
                auto thisPtr = static_cast<_Ptr<This>>(ptr);
                _Alloc<This> localAlloc(alloc);
                This* realPtr = &*thisPtr;
                _AllocTraits<This>::destroy(localAlloc, realPtr);
                _AllocTraits<This>::deallocate(localAlloc, thisPtr, 1);
            }
            void addStack(_PtrCO me, _StackT& stackT, _StackO& stackO) const override {
                while(!stackO.empty() && stackO.front()->priority() >= priority()) {
                    stackO.front()->delStack(stackT);
                    stackO.pop_front();
                }
                stackO.emplace_front(me);
            }
            void delStack(_StackT& stackT) const override {
                auto iter1 = stackT.begin();
                auto iter2 = iter1 ++;
                *iter1 = _func(std::move(*iter1), std::move(*iter2));
                stackT.pop_front();
            }
        };
      // _Unary
        template<bool Suf>
        struct _Unary: public _Operation {
        private:
            using _Operation::priority;
            _FuncUn _func;
        public:
            _Unary(int priority, _FuncUn func): 
                _Operation(priority),
                _func(func) {}
            
            void deallocate(Allocator alloc, _PtrO ptr) override {
                using This = _Unary;
                auto thisPtr = static_cast<_Ptr<This>>(ptr);
                _Alloc<This> localAlloc(alloc);
                This* realPtr = &*thisPtr;
                _AllocTraits<This>::destroy(localAlloc, realPtr);
                _AllocTraits<This>::deallocate(localAlloc, thisPtr, 1);
            }

            void addStack(_PtrCO me, _StackT& stackT, _StackO& stackO) const override {
                if (Suf) {
                    stackT.front() = _func(std::move(stackT.front()));
                } else {
                    while(!stackO.empty() && stackO.front()->priority() >= priority()) {
                        stackO.front()->delStack(stackT);
                        stackO.pop_front();
                    }
                    stackO.emplace_front(me);
                }
            }

            void delStack(_StackT& stackT) const override {
                if (!Suf) {
                    stackT.front() = _func(std::move(stackT.front()));
                }
            }
        };
      // _Brackets
        template<bool Open>
        struct _Brackets: public _Operation {
            static constexpr int _bracketPrior = -1;
        public:
            _Brackets(): 
                _Operation(_bracketPrior) {}
            
            void deallocate(Allocator alloc, _PtrO ptr) override {
                using This = _Brackets;
                auto thisPtr = static_cast<_Ptr<This>>(ptr);
                _Alloc<This> localAlloc(alloc);
                This* realPtr = &*thisPtr;
                _AllocTraits<This>::destroy(localAlloc, realPtr);
                _AllocTraits<This>::deallocate(localAlloc, thisPtr, 1);
            }

            void addStack(_PtrCO me, _StackT& stackT, _StackO& stackO) const override {
                if (Open) {
                    stackO.emplace_front(me);
                } else {
                    while(!stackO.empty() && stackO.front()->priority() != _bracketPrior) {
                        stackO.front()->delStack(stackT);
                        stackO.pop_front();
                    }
                    stackO.pop_front();
                }
            }
            
            void delStack(_StackT& stackT) const override {}
        };
      // member
        _VectorS2O _s2o;
        _FuncBin _endFuncBin;
        _FuncUn _endFuncUn;
        _Ptr<Creator> _creator;
      // functions
        template<class U, class... Args>
        static _Ptr<U> _allocate(Allocator alloc, Args&&... args) {
            _Alloc<U> localAlloc(alloc);
            _Ptr<U> ptr = _AllocTraits<U>::allocate(localAlloc, 1);
            U* realPtr = &*ptr;
            _AllocTraits<U>::construct(localAlloc, realPtr, std::forward<Args>(args)...);
            return ptr;
        }

        template<class U>
        static void _deallocate(Allocator alloc, _Ptr<U> ptr) {
            _Alloc<U> localAlloc(alloc);
            U* realPtr = &*ptr;
            _AllocTraits<U>::destroy(localAlloc, realPtr);
            _AllocTraits<U>::deallocate(localAlloc, ptr, 1);
        }

        static bool _comp(const _SymbolToOparation& s2o, char letter) {
            return s2o.first < letter;
        }

        auto _alloc() {
            return _s2o.get_allocator();
        }

        inline void _processSymbol(Char letter, _StackT& stackT, _StackO& stackO) const {
            auto iter = std::lower_bound(_s2o.begin(), _s2o.end(), letter, _comp);
            if (iter != _s2o.end() && iter->first == letter) {
                if (!_creator->empty()) {
                    stackT.emplace_front(_creator->get());
                }
                iter->second->addStack(iter->second, stackT, stackO);
            } else {
                _creator->emplace(letter);
            }
        }

        inline _T _completion(_StackT& stackT, _StackO& stackO) const {
            if (!_creator->empty()) {
                stackT.emplace_front(_creator->get());
            }
            while(!stackO.empty()) {
                stackO.front()->delStack(stackT);
                stackO.pop_front();
            }
            _T ret = std::move(stackT.front());
            stackT.pop_front();
            if(_endFuncBin) {
                while(!stackT.empty()) {
                    ret = _endFuncBin(std::move(ret), std::move(stackT.front()));
                    stackT.pop_front();
                }
            }
            if(_endFuncUn) {
                ret = _endFuncUn(std::move(ret));
            }
            return ret;
        }
    };
} // namespace sb