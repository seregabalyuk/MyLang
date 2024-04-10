## Definitions
**FA** - finite automaton.
**FA** consist of nodes.
Nodes have transitions by letter to other nodes.
Nodes have type.
FA can be DFA and NFA.
More about [DFA](https://en.wikipedia.org/wiki/Deterministic_finite_automaton) and [NFA](https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton).

**State** is node.
Usually is State&.

**Transition** is transition. 
Usually is Transition&.

**Type** is type node type.

**Letter** is letter type.


## struct FA
### Requirements

1) FA must have metods:
    ```c++
    FA fa;
    const FA constFa;
    fa.emplace();     // emplace node return State&
    fa.emplace(type); // emplace node return State&
    fa.erase(iter);   // erase Node by iter

    fa.start();       // return State&
    constFa.start();  // return const State&

    fa.begin();       // return iter
    constFa.cbegin(); // return constiter
    constFa.begin();  // return constiter

    fa.end();       // return iter
    constFa.cend(); // return constiter
    constFa.end();  // return constiter

    constFa.size(); // return size_t

    constFa.empty(); // return bool
    ```
2) FA must support this syntax:
    ```c++
    FA fa;
    for (auto& state: fa) {
        // your code
    }

    const FA constFa;
    for (auto& constState: constFa) {
        // your code
    }
    ```
You can check your **FA** use concepts:
```c++
sb::C_FA<FA>    // check FA
sb::C_DFA<DFA>  // check DFA
sb::C_NFA<NFA>  // check NFA
```

### Possibilities

struct **FATraits** can get information about **FA**. Examples:
```c++
sb::FATraits<YourFA>::Letter;       // return letter type
sb::FATraits<YourFA>::Type;         // return type node type
sb::FATraits<YourFA>::State;        // return State
sb::FATraits<YourFA>::Transition;   // return Transition
sb::FATraits<YourFA>::IteratorBySt; // return iterator by state
sb::FATraits<YourFA>::IteratorByTr; // return iterator by transition
sb::FATraits<YourFA>::ConstIteratorBySt; // return const iterator by state
sb::FATraits<YourFA>::ConstIteratorByTr; // return const iterator by transition
```
FATraits never call compilation errors.
And have little name:
```c++
using FATraitsSt = typename FATraits<FA>::State;
using FATraitsTr = typename FATraits<FA>::Transition;
using FATraitsTy = typename FATraits<FA>::Type;
using FATraitsLe = typename FATraits<FA>::Letter;
using FATraitsItSt = typename FATraits<FA>::IteratorBySt;
using FATraitsItTr = typename FATraits<FA>::IteratorByTr;
using FATraitsCItSt = typename FATraits<FA>::ConstIteratorBySt;
using FATraitsCItTr = typename FATraits<FA>::ConstIteratorByTr;
```

## struct State
### Requirements

1) **State** must be **std::ranges::range**. 
    It means that thay have metods **begin()** and **end()**.

    So you must can iterate by **State**.
    Example:
    ```c++
    State state;
    for (auto& transition: state) {
        // your code
    }
    ```

2) **State** must have metods:
    ```c++
    State state;
    const State constState;
    constState.type();      // return const Type&

    state.type();           // return Type&
    state.emplace(letter, otherConstState); // return Transition&
    state.erase(letter, otherConstState);
    state.erase(letter);    // remove all transitions using this letter
    consState.count(letter); // return size_t count Transition with the letter
    state.count(letter); // return size_t count Transition with the letter
    state.size(); // return size_t count Transition
    ```

3) If **DFA** is, **State** must have **operator[]**:
    ```c++
    state[letter]; // return next State
    constState[letter]; // return next ConstState
    ```
4) If **NFA** is, **State** must have **operator[]**:
    ```c++
    for (auto& transition: state[letter]) {
        // your code
    }
    for (auto& transition: constState[letter]) {
        // your code
    }
    ```

### Possibilities

struct **StateFATraits** can get information about **State**. Examples:
```c++
sb::StateFATraits<YourState>::Letter; // return letter type
sb::StateFATraits<YourState>::Type; // return type node type
sb::StateFATraits<YourState>::Transition; // return Transition
sb::StateFATraits<YourState>::IteratorByTr; // return Iterator by Transition
```

## struct Transition
### Requirements
Transition must have methods:
```c++
Transition transition;
const Transition constTransition;

transition.letter(); // return Letter
constTransition.letter(); // return Letter

transition.next(); // return State&
constTransition.next(); // return const State&
```


### Possibilities

struct **TransitionFATraits** can get information about **Transition**. Examples:
```c++
sb::TransitionFATraits<YourTransition>::State; // return State type
sb::TransitionFATraits<YourTransition>::Letter;  // return letter type
```

## struct Type
### Requirements

**Type** must have operators:
```c++
type = type | type;
type |= type;
// It use when states are united
```

You can check your **Type** use concepts:
```c++
sb::C_TypeFA<Type>    // check Type in FA
```

## struct Letter
### Requirements

Letter must compare.
Letter usually is char.
