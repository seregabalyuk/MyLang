
#pragma once

#include <vector>
#include <string>

#include <TypeTraits/TypeList.hpp>
#include "RuleRPNType.hpp"

namespace sb {
    template<
        class T,
        class Letter = char,
        class Allocator = std::allocator<Letter>
    > struct TableRPN {
    public:
      // classes
        struct Rule;
    private:
      // usings
       // from Allocator
        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        template<class U>
        using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
        template<class U>
        using _Ptr = _AllocTraits<U>::pointer;

       // from Other
        using _Rules = std::vector<Rule, _Alloc<Rule>>;
        using _Int = char;

       // for Functions
        using _ListBinFunc = TypeList<
            T(*)(T&&, T&&),
            T(*)(const T&, const T&),
            T(*)(T&, T&),
            void(*)(T&, const T&),
            void(*)(T&, T&),
            void(*)(T&, T&&)
        >;
        static constexpr size_t _binSize = Size<_ListBinFunc>::value;
        using _ListUnFunc = TypeList<
            T(*)(T&&),
            T(*)(const T&),
            T(*)(T&),
            void(*)(T&)
        >;
        static constexpr size_t _unSize = Size<_ListUnFunc>::value;
        static constexpr size_t _allSize = _binSize + 2 * _unSize;
    public:
      // functions
        template<C_BelongTo<_ListBinFunc> Func>
        void addBinary(Letter letter, _Int priority, Func func) {
            _Int type = BelongTo<Func, _ListBinFunc>::number;
            _addRule(letter, priority, type, reinterpret_cast<void*>(func));
        }

        template<C_BelongTo<_ListUnFunc> Func>
        void addPreUnary(Letter letter, _Int priority, Func func) {
            _Int type = BelongTo<Func, _ListUnFunc>::number * 2 + _binSize;
            _addRule(letter, priority, type, reinterpret_cast<void*>(func));
        }

        template<C_BelongTo<_ListUnFunc> Func>
        void addSufUnary(Letter letter, _Int priority, Func func) {
            _Int type = BelongTo<Func, _ListUnFunc>::number * 2 + 1 + _binSize;
            _addRule(letter, priority, type, reinterpret_cast<void*>(func));
        }

        template<C_BelongTo<_ListUnFunc> Func>
        void putEndUnary(Func func) {
            _Int type = BelongTo<Func, _ListUnFunc>::number * 2 + 1 + _binSize;
            _endRule = Rule(Letter(), 0, type, reinterpret_cast<void*>(func));
        }

        const Rule& get(Letter letter) const {
            size_t l = 0;
            size_t r = _rules.size();
            while (l + 1 < r) {
                size_t m = (l + r) / 2;
                if (_rules[m].letter() <= letter) {
                    l = m;
                } else {
                    r = m;
                }
            }
            if (_rules[l].letter() != letter) {
                throw std::string{"did not find rule by letter"};
            }
            return _rules[l];
        }

        const Rule& getEnd() const {
            return _endRule;
        }
      // Rule
        struct Rule {
        public:
          // functions
            Rule(Letter letter, _Int priority, _Int type, void* func):
                _func(func),
                _letter(letter),
                _priority(priority),
                _type(type)
            {}

            Rule(const Rule& other):
                _func(other._func),
                _letter(other._letter),
                _priority(other._priority),
                _type(other._type)
            {}

            Rule& operator = (const Rule& other) & {
                _func = other._func;
                _letter = other._letter;
                _priority = other._priority;
                _type = other._type;
                return *this;
            }

            const Letter& letter() const { return _letter; }
            RuleRPNType type() const {
                if (_type < _binSize) {
                    return RuleRPNType::Binary;
                } else if ((_type - _binSize) & 1) {
                    return RuleRPNType::SuffixUnary;
                } else {
                    return RuleRPNType::PrefixUnary;
                }
            }
            const _Int& priority() const { return _priority; }

            void operator()(T& first, T&& second) const {
                switch(_type) {
                case 0: {
                    auto func = reinterpret_cast<GetT<0, _ListBinFunc>>(_func);
                    first = func(std::move(first), std::move(second));
                    } break;
                case 1: {
                    auto func = reinterpret_cast<GetT<1, _ListBinFunc>>(_func);
                    first = func(first, second);
                    } break;
                case 2: {
                    auto func = reinterpret_cast<GetT<2, _ListBinFunc>>(_func);
                    first = func(first, second);
                    } break;
                case 3: {
                    auto func = reinterpret_cast<GetT<3, _ListBinFunc>>(_func);
                    func(first, second);
                    } break;
                case 4: {
                    auto func = reinterpret_cast<GetT<3, _ListBinFunc>>(_func);
                    func(first, second);
                    } break;
                case 5: {
                    auto func = reinterpret_cast<GetT<3, _ListBinFunc>>(_func);
                    func(first, std::move(second));
                    } break;
                default:
                    throw std::string{"dont find binary function from table by type"};
                }
            }

            void operator()(T& first) const {
                switch((_type - _binSize) >> 1) {
                case 0: {
                    auto func = reinterpret_cast<GetT<0, _ListUnFunc>>(_func);
                    first = func(std::move(first));
                    } break;
                case 1: {
                    auto func = reinterpret_cast<GetT<1, _ListUnFunc>>(_func);
                    first = func(first);
                    } break;
                case 2: {
                    auto func = reinterpret_cast<GetT<2, _ListUnFunc>>(_func);
                    first = func(first);
                    } break;
                case 3: {
                    auto func = reinterpret_cast<GetT<3, _ListUnFunc>>(_func);
                    func(first);
                    } break;
                default:
                    break;
                }
            }
        private:
          // members
            void* _func;
            Letter _letter;
            _Int _priority;
            _Int _type;
        };
    private:
      // function 
        inline void _addRule(Letter letter, _Int priority, _Int type, void* func) {
            auto iter = _rules.begin();
            while (iter != _rules.end() && iter->letter() < letter) {
                ++ iter;
            }
            _rules.emplace(iter, letter, priority, type, func);
        }
      // members
        _Rules _rules;
        Rule _endRule = Rule(Letter(), 0, _allSize, nullptr);
    };
};
