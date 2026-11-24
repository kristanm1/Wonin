# Wonin

A tree-walking interpreter for a small scripting language written in C++

## Features

- Scanner (lexer) that recognizes all tokens of the Lox-style language
- Recursive-descent parser that builds an AST of expressions and statements
- Tree-walking interpreter using the visitor pattern
- Numbers, strings and `nil`
- Arithmetic: `+`, `-`, `*`, `/`, unary `-` and grouping with `( )`
- Variables: `var` declarations and assignment
- Block scoping with nested environments (`{ ... }`)
- `print` statements and `//` line comments
- Interactive REPL and script execution

## Example

Program examples are in [`interpreter/res/`](interpreter/res).

## Acknowledgements

Based on Robert Nystrom's book [Crafting Interpreters](https://craftinginterpreters.com).
