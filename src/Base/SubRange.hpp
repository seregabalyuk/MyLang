#pragma once

#include <TypeTraits/IteratorTraits.hpp>

namespace sb {
    template<class Iter>
    struct SubRange {
    public:
      // constructors
        SubRange(const Iter& begin, const Iter& end):
            _begin(begin),
            _end(end) {}
        SubRange(Iter&& begin, Iter&& end):
            _begin(std::move(begin)),
            _end(std::move(end)) {}
        SubRange(const SubRange& other):
            _begin(other._begin),
            _end(other._end) {}
        SubRange(SubRange&& other):
            _begin(std::move(other._begin)),
            _end(std::move(other._end)) {}
      // operators =
        SubRange& operator =(const SubRange& other) {
            _begin = other._begin;
            _end = other._end;
            return *this;
        }

        SubRange& operator =(SubRange&& other) {
            _begin = std::move(other._begin);
            _end = std::move(other._end);
            return *this;
        }
      // function
        Iter& begin() { return _begin; }
        const Iter& begin() const { return _begin; }

        Iter& end() { return _end; }
        const Iter& end() const { return _end; }
            
    private:
        Iter _begin;
        Iter _end;
    };
// realization
  // 
} // namespace sb