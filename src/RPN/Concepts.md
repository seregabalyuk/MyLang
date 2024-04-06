## Definitions

**RPN** is reverse polish notation.
It transform from string to **T**.
More about [rus](https://habr.com/ru/articles/596925/) and [eng](https://en.wikipedia.org/wiki/Reverse_Polish_notation)

**Creator** is type create **T** by string

**Char** is letter type.


## struct RPN
### Requirements

**RPN** must have metods:
```c++
RPN rpn(creator);
const_RPN c_rpn;

rpn.addBinary(letter, priority, func);
rpn.addBrackets(open, close);
rpn.addSufUnary(letter, priority, func);
rpn.addPreUnary(letter, priority, func);
rpn.addEndBinary(func);

c_rpn(istream);    // return OutT by istream
c_rpn(begin, end); // return OutT by iterators
```
## struct Creator
### Requirements

**Creator** must have metods:
```c++
creator.emplace(letter);
creator.get();   // return OutT
creator.empty(); // return bool
creator.clear(); // clear
```

## struct T
### Requirements

**T** must have move constructor.
