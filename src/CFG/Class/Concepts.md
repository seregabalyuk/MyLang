## Definitions
**CFG** - is context-free grammar.
It consists of **rules** and **symbols**. 
**Symbols** have two types **terminal** and **nonterminal**.
**Rule** is **nonterminal** to array of **symbol**.
**Terminal** contain **T**.
More about [**CFG**](https://en.wikipedia.org/wiki/Context-free_grammar).

**Rule** - is link to **rule**

**ConstRule** - is link to **const rule**

**Symbol** - is link to **symbol**

**ConstSymbol** - is link to **const symbol**

**Terminal** - is link to **terminal**

**ConstTerminal** - is link to **const terminal**

**NonTerminal** - is link to **nonterminal**

**ConstNonTerminal** - is link to **const nonterminal**

**T** - is type like char or DFA.

## struct CFG
### Requirements
**CFG** must have metods:
```c++
const CFG const_cfg;
CFG cfg;
cfg.start();        // return Symbol
cfg.cstart();       // return ConstSymbol
const_cfg.start();  // return ConstSymbol
const_cfg.cstart(); // return ConstSymbol

cfg.emplace_terminal(name, ...); // return Terminal
cfg.emplace_nonterminal(name);   // return NonTerminal

cfg.emplace_rule(nonTerminal);   // return Rule

cfg.rules(nonTerminal); // return {begin, end} of Rule
const_cfg.rules(nonTerminal); // return {begin, end} of ConstRule
```

## struct Symbol and ConstSymbol
### Requirements

**Symbol** must have metods:

```c++
ConstSymbol constSymbol;
Symbol symbol;

constSymbol.is_terminal(); // return bool
Symbol.is_terminal();      // return bool

Symbol.name();      // return name&
constSymbol.name(); // return const name&

Symbol.get_terminal();    // return Terminal
Symbol.get_nonterminal(); // return NonTerminal

constSymbol.get_terminal();    // return ConstTerminal
constSymbol.get_nonterminal(); // return ConstNonTerminal
```

You need use it:

```c++
Symbol symbol;
if (symbol.is_terminal()) {
    auto terminal = symbol.get_terminal();
} else {
    auto nonterminal = symbol.get_nonterminal();
}
```

## struct Rule and ConstRule
### Requirements
1) **Rule** must support the syntax:
    ```c++
    Rule rule;
    for (auto symbol: rule) {
        // your code
    }

    ConstRule constRule;
    for (auto constSymbol: constRule) {
        // your code
    }
    ```
2) **Rule** must have methods:
    ```c++
    Rule rule;
    ConstRule constRule;

    size_t i;

    rule[i]; // return Symbol
    constRule[i]; // return ConstSymbol

    rule.emplace(symbol);

    rule.argument(); // return NonTerminal
    constRule.argument(); // return ConstNonTerminal
    ```

## struct Terminal and ConstTerminal
### Requirements
**Terminal** must have methods:
