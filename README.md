# Wonin

A tree-walking interpreter for a small scripting language written in C++

## Features

- Lexer (Scanner) that recognizes all tokens of the Lox-style language
- Recursive-descent parser that builds an AST of expressions and statements
- Tree-walking interpreter using the visitor pattern
- Numbers, booleans, strings and `nil`
- Arithmetic: `+`, `-`, `*`, `/`, unary `-` and grouping with `( )`
- Variables: `var` declarations and assignment
- Block scoping with nested environments (`{ ... }`)
- Functions: `fun` closures with captured environment, calls
- `print` statements and `//` line comments
- Interactive REPL and script execution

## Example

Program examples are in [`interpreter/res/`](interpreter/res).

## Known Limitations

Captured environments are not deallocated when closures become unreachable. They remain allocated until the program terminates.

## Acknowledgements

Based on Robert Nystrom's book [Crafting Interpreters](https://craftinginterpreters.com).
