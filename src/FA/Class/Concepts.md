## Definitions
**FA** - finite automaton.
**FA** consist of nodes.
Nodes have transitions by letter to other nodes.
Nodes have type.
FA can be DFA and NFA.
More about [DFA](https://en.wikipedia.org/wiki/Deterministic_finite_automaton) and [NFA](https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton).

**State** are link to nodes.

**ConstState** are const link to nodes.

**Type** is type node type.

**Char** is letter type.

**Transition** is link to transition.

**ConstTransition** is const link to transition.

## struct FA
### Requirements

FA must have metods:
```c++
FA fa; // must have default constructor
const FA const_fa;
// not have any nodes (start too)
fa.emplace();     // emplace node return State
fa.emplace(type); // emplace node return State
fa.erase(constState);  // erase Node by state

fa.begin();       // return State
fa.cbegin();      // return ConstState
const_fa.begin(); // return ConstState
```
You can check your **FA** use concepts:
```c++
sb::C_FA<FA>         // check FA
sb::C_NFA<NFA>       // check NFA
sb::C_DFA<DFA>       // check DFA
```

### Possibilities

struct **FATraits** can get information about **FA**. Examples:
```c++
sb::FATraits<YourFA>::Char;  // return letter type
sb::FATraits<YourFA>::Type;  // return type node type
sb::FATraits<YourFA>::State;           // return State
sb::FATraits<YourFA>::ConstState;      // return ConstState
sb::FATraits<YourFA>::Transition;      // return Transition
sb::FATraits<YourFA>::ConstTransition; // return ConstTransition
```

## struct State and ConstState
### Requirements

1) **State** and **ConstState** must be **std::ranges::range**. 
    It means that thay have metods **begin()** and **end()**.
    **ConstState** mustn't change **FA**.
    **State** can change **FA**.

    So you must can iterate by **State**.
    Example:
    ```c++
    State state;
    for (auto [letter, nextState]: state) {
        // your code
    }

    ConstState constState;
    for (auto [letter, nextConstState]: constState) {
        // your code
    }
    ```

    Important:

    ```c++
    std::range::iterator_t<State>;      // get Transition
    std::range::iterator_t<ConstState>; // get ConstTransition
    ```

2) **State** and **ConstState** must have metods:
    ```c++
    constState.type();      // return const Type&

    state.type();           // return Type&
    state.emplace(letter, otherConstState);
    state.erase(letter, otherConstState);
    state.erase(letter);    // remove all transitions using this letter
    ```

3) **State** and **ConstState** must have constructors and operators:
    ```c++
    // copy
    State(const State& other);
    ConstState(const State& other);
    ConstState(const ConstState& other);
    
    State& operator=(const State& other) &;
    ConstState& operator=(const State& other) &;
    ConstState& operator=(const ConstState& other) &;
    ```

3) If **DFA** is, **State** and **ConstState** must have **operator[]**:
    ```c++
    state[letter]; // return next State
    constState[letter]; // return next ConstState
    
    ```
4) If **NFA** is, **State** and **ConstState** must have **operator[]**:
    ```c++
    for (auto& nextState: state[letter]) {
        // your code
    }
    for (const auto& nextState: constState[letter]) {
        // your code
    }
    ```
5) For correct work algorithm, you **State** and **ConstState** have to compare, if you use **std::set** and **std::map**.

You can check your **State** and **ConstState** use concepts:
```c++
sb::C_StateFA<State>    // check State in FA
sb::C_StateDFA<State>   // check State in DFA
sb::C_StateNFA<State>   // check State in NFA

sb::C_ConstStateFA<State>    // check State in FA
sb::C_ConstStateDFA<State>   // check State in DFA
sb::C_ConstStateNFA<State>   // check State in NFA
```

### Possibilities

struct **StateFATraits** can get information about **State**. Examples:
```c++
sb::StateFATraits<YourState>::Char; // return letter type
sb::StateFATraits<YourState>::Type; // return type node type
sb::StateFATraits<YourState>::Transition; // return Transition
```

## struct Transition and ConstTransition
### Requirements
Transition and ConstTransition must support this syntax:
```c++
tran
auto [letter, state] = *transition;
auto [letter, constState] = *constTransition;
// letter is Char
// state is State
// transition is Transition
```
More about [binding](https://en.cppreference.com/w/cpp/language/structured_binding)

You can check your **Transition** and **ConstTransition** use concepts:
```c++
sb::C_TransitionFA<Transition>              // check Transition in FA
sb::C_TransitionFA<ConstTransition>    // check ConstTransition in FA
// Transition in DFA and NFA don't change
```

### Possibilities

struct **TransitionFATraits** can get information about **Transition** and **ConstTransition**. Examples:
```c++
sb::TransitionFATraits<YourTransition>::State; // return State type
sb::TransitionFATraits<YourTransition>::Char;  // return letter type

sb::TransitionFATraits<YourConstTransition>::State; // return ConstState type
sb::TransitionFATraits<YourConstTransition>::Char;  // return letter type
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

## struct Char
### Requirements

Char must compare.
Char usually is char.
