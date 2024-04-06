# Description

**RE** is regular expression.
This is the recording form that specifies the language.

More about [rus](https://ru.wikipedia.org/wiki/%D0%A0%D0%B5%D0%B3%D1%83%D0%BB%D1%8F%D1%80%D0%BD%D1%8B%D0%B5_%D0%B2%D1%8B%D1%80%D0%B0%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F) and [eng](https://en.wikipedia.org/wiki/Regular_expression)

# Facts

This forms set the same set of languages:

1) **RE**
2) **DFA** [more](https://ru.wikipedia.org/wiki/%D0%94%D0%B5%D1%82%D0%B5%D1%80%D0%BC%D0%B8%D0%BD%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%BD%D0%B5%D1%87%D0%BD%D1%8B%D0%B9_%D0%B0%D0%B2%D1%82%D0%BE%D0%BC%D0%B0%D1%82)
3) **NFA** [more](https://ru.wikipedia.org/wiki/%D0%9D%D0%B5%D0%B4%D0%B5%D1%82%D0%B5%D1%80%D0%BC%D0%B8%D0%BD%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%BD%D0%B5%D1%87%D0%BD%D1%8B%D0%B9_%D0%B0%D0%B2%D1%82%D0%BE%D0%BC%D0%B0%D1%82)

# Syntax

## Operations

1) **Concatinate**      `(RS)`  - write string R, than S
2) **Alternation**      `(R|S)` - write string R or S
3) **Kleene star**      `(R*)`  - write string R 0 or N repeatedly
4) **Plus**             `(R+)`  - write string R 1 or N repeatedly
5) **Question**         `(R?)`  - not write or write string R
6) **Square brackets**  `([S])` - write one simbol from S 

## Special Simbols

1) **Shield** `\` - shield next symbol
2) **Eps**    `\e` - empty string
