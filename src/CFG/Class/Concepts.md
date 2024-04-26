## Definitions
**CFG** - is context-free grammar.
It consists of **rules** and **symbols**. 
**Symbols** have two types **terminal** and **nonterminal**.
**Rule** is **nonterminal** to array of **symbol**.
**Terminal** contain **T**.
More about [**CFG**](https://en.wikipedia.org/wiki/Context-free_grammar).

**Rule** - is **rule**

**Symbol** - is **symbol**

**Terminal** - is **terminal**

**NonTerminal** - is **nonterminal**

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

cfg.emplace_terminal(...); // return Terminal
cfg.emplace_nonterminal(name);   // return NonTerminal

cfg.emplace_rule(nonTerminal);   // return Rule
cfg.emplace_rule(nonTerminal, {Symbol& a, ...});   // return Rule

cfg.rules(nonTerminal); // return Rules

cfg.terminals();
cfg.nonterminals();
```

## struct Symbol
### Requirements

**Symbol** must have metods:

```c++
const Symbol const_symbol;
Symbol symbol;

const_symbol.isTerminal(); // return bool
symbol.isTerminal();      // return bool

const_symbol.isNonTerminal(); // return bool
symbol.isNonTerminal();      // return bool

symbol.getTerminal();    // return Terminal
symbol.getNonterminal(); // return NonTerminal

const_symbol.getTerminal();    // return ConstTerminal
const_symbol.getNonterminal(); // return ConstNonTerminal
```

You need use it:

```c++
Symbol symbol;
if (symbol.is_terminal()) {
    auto& terminal = symbol.get_terminal();
} else {
    auto& nonterminal = symbol.get_nonterminal();
}
```

## struct Rule
### Requirements
1) **Rule** must support the syntax:
    ```c++
    Rule rule;
    for (auto& symbol: rule) {
        // your code
    }

    const Rule constRule;
    for (auto& constSymbol: constRule) {
        // your code
    }
    ```
2) **Rule** must have methods:
    ```c++
    Rule rule;
    const Rule constRule;
    
    size_t i;

    rule[i]; // return Symbol
    constRule[i]; // return ConstSymbol

    rule.emplace_back(symbol);

    rule.argument(); // return NonTerminal
    constRule.argument(); // return ConstNonTerminal
    ```

## struct Terminal
### Requirements
**Terminal** must have methods:
