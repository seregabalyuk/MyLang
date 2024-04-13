## Definitions

**RPN** is reverse polish notation.
It transform from string to **T**.
More about [rus](https://habr.com/ru/articles/596925/) and [eng](https://en.wikipedia.org/wiki/Reverse_Polish_notation)

**Char** is letter type.
String consist of **Char**.

**Creator** is type create **T** by string.

**DFA** is dfa, it help distinguish symbols.

**Table** is table operations.


## struct RPN
### Requirements

**RPN** must have metods:
```c++
RPN rpn;
Char letter;
// can throw
rpn.put(letter); 
rpn.get(); // return T

// without throw
rpn.putDFA(dfa);
rpn.putCreator(creator);
rpn.putTable(table);
```

## struct DFA
### Requirements

**DFA** need be **FA**.
```c++
std::same_as<typename FATraits<DFA>::type, char>;
```

**Type** is char, and it have some meaning.

1) `'t'` - the letter belongs Table.
2) `'o'` - is open bracket.
3) `'c'` - is close bracket.
4) `' '` - the letter doesn't belong anything
5) other char transmit **Creator**

## struct Creator
### Requirements

**Creator** must have metods:
```c++
creator.put(letter, type); // type from dfa
creator.empty(); // return bool
// can throw
creator.get();   // return T 
```

## struct Table
### Requirements

**Table** must have methods:
```c++
auto& rule = table.get(letter); // return special type
rule.type(); // type of rule
// 0 - binary function
// 1 - prefix unary function
// 2 - suffix unary function
rule.priority(); // return priority
T one;
T two;
rule(one); // return void
rule(one, two); // return void
```

## struct T
### Requirements

**T** must have move constructor.
