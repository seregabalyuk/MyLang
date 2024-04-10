#pragma once

#include <unordered_set>
#include <cstdint>

namespace sb  {
    template<class T, class Allocator = std::allocator<uint64_t>>
    struct SetLink {
    private:
      // classes
        template<bool isConst>
        struct _BaseIter;
      // usings
        using Int = uint64_t;

        template<class U>
        using _Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
        template<class U>
        using _AllocTraits = typename std::allocator_traits<Allocator>::template rebind_traits<U>;
        template<class U>
        using _Ptr = _AllocTraits<U>::pointer;

        using _SetInt = std::unordered_set<Int, std::hash<Int>, std::equal_to<Int>, _Alloc<Int>>;
        using _SetIter = _SetInt::iterator;
        using _SetCIter = _SetInt::const_iterator;
    public:
      // usings
        using iterator = _BaseIter<0>;
        using const_iterator = _BaseIter<1>;
        
      // functions
        SetLink(): _set() {}
        SetLink(SetLink&& other): _set(std::move(other._set)), _hash(other._hash) {}
        SetLink(const SetLink& other): _set(other._set), _hash(other._hash) {}
        
        
        iterator emplace(T& value) {
            auto [iter, _] = _set.emplace(getInt(value));
            _hash += getInt(value);
            return iter;
        }

        void erase(T& value) {
            _set.erase(getInt(value));
            _hash -= getInt(value);
        }

        iterator begin() {
            return _set.begin();
        }

        const_iterator begin() const {
            return _set.begin();
        }

        const_iterator cbegin() const {
            return _set.cbegin();
        }

        iterator end() {
            return _set.end();
        }

        const_iterator end() const {
            return _set.end();
        }

        const_iterator cend() const {
            return _set.cend();
        }

        size_t size() const {
            return _set.size();
        }

        bool empty() const {
            return _set.empty();
        }

        size_t count(T& value) const {
            return _set.count(getInt(value));
        }

        iterator find(T& value) {
            return _set.find(getInt(value));
        }

        const_iterator find(T& value) const {
            return _set.find(getInt(value));
        }

        bool operator !=(const SetLink& other) const {
            return _set != other._set;
        }

        bool operator ==(const SetLink& other) const {
            return _set == other._set;
        }

        size_t hash() const noexcept {
            return std::hash<Int>()(_hash);
        }

        void swap(SetLink& other) {
            std::swap(_hash, other._hash);
            std::swap(_set, other._set);
        }
    private:
      // _BaseIter
        template<bool isConst>
        struct _BaseIter {
        private:
          // using
            using _Iter = std::conditional_t<isConst, _SetCIter, _SetIter>;
            using _Link = std::conditional_t<isConst, const T&, T&>;
            using _Ptr = std::conditional_t<isConst, const T*, T*>;
          // friends
            friend SetLink;
            friend _BaseIter<1>;
          // member
            _Iter _iter;
          // functions
            _BaseIter(_Iter iter): _iter(iter) {}

            Int getInt() const {
                return *_iter;
            }
        public:
            template<bool OtherConst>
            _BaseIter(const _BaseIter<OtherConst>& other): _iter(other._iter) {}

            template<bool OtherConst>
            _BaseIter& operator = (const _BaseIter<OtherConst>& other) {
                _iter = other._iter;
                return *this;
            }

            _Link operator*() const {
                return *reinterpret_cast<_Ptr>(getInt());
            }

            _Ptr operator->() const {
                return reinterpret_cast<_Ptr>(getInt());
            }

            _BaseIter& operator++() {
                ++ _iter;
                return *this;
            }

            _BaseIter operator++(int) {
                return _iter ++;
            }

            template<bool OtherConst>
            bool operator != (const _BaseIter<OtherConst>& other) const {
                return _iter != getIter(other);
            }

            template<bool OtherConst>
            bool operator == (const _BaseIter<OtherConst>& other) const {
                return _iter == getIter(other);
            }
        };
      // functions
        template<bool isConst>
        static const auto& getIter(const _BaseIter<isConst>& iter) {
            return iter._iter;
        }

        static Int getInt(T& value) {
            return reinterpret_cast<Int>(&value);
        }
      // members
        _SetInt _set;
        Int _hash = 0;
    };
} // namespace sb

namespace std {
    template<class T, class Alloc>
    struct hash<sb::SetLink<T, Alloc>> {
        size_t operator ()(const sb::SetLink<T, Alloc>& setLink) const noexcept {
            return setLink.hash();
        }
    };
}