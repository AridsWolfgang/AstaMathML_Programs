# Symbolic Math Interpreter

A Python-based symbolic mathematics engine that can parse, simplify, differentiate, integrate, and evaluate mathematical expressions. Includes an interactive REPL (Read-Eval-Print Loop) for exploratory computation.

---

## Table of Contents

1. [Parsing with Recursive Descent](#1-parsing-with-recursive-descent)
2. [Abstract Syntax Tree (AST)](#2-abstract-syntax-tree-ast)
3. [Algebraic Simplification](#3-algebraic-simplification)
4. [Symbolic Differentiation](#4-symbolic-differentiation)
5. [Symbolic Integration](#5-symbolic-integration)
6. [Numeric Evaluation](#6-numeric-evaluation)
7. [REPL Usage](#7-repl-usage)

---

## 1. Parsing with Recursive Descent

### What is Parsing?

Parsing is the process of converting a linear string of characters (like `"3*x^2 + 2*x + 1"`) into a structured representation that a computer can manipulate. The structured representation is an **Abstract Syntax Tree (AST)**.

### Tokenization (Lexical Analysis)

The first step breaks the input string into **tokens** — the smallest meaningful units:

```
Input:  "3*x^2 + 2*x + 1"
Tokens: [NUMBER(3), TIMES, VARIABLE(x), POW, NUMBER(2), PLUS,
         NUMBER(2), TIMES, VARIABLE(x), PLUS, NUMBER(1)]
```

Our tokenizer (in `parser.py`) uses regular expressions to match:
- **Numbers**: integer or decimal (`\d+(\.\d+)?`)
- **Variables**: alphabetic identifiers (`[a-zA-Z_][a-zA-Z0-9_]*`)
- **Operators**: `+`, `-`, `*`, `/`, `^`
- **Parentheses**: `(`, `)`
- **Commas**: `,` (for function arguments)
- **Whitespace**: ignored

### Recursive Descent Parsing

A **recursive descent parser** is a top-down parser that uses mutually recursive functions corresponding to grammar rules. Our grammar:

```
expression  →  term (('+' | '-') term)*
term        →  factor (('*' | '/') factor)*
factor      →  power ('^' factor)*
power       →  '-' power | primary
primary     →  NUMBER
             | VARIABLE '(' expression ')'
             | VARIABLE
             | '(' expression ')'
```

Each grammar rule maps to a method in the `Parser` class:

| Method | Grammar Rule | Example |
|--------|-------------|---------|
| `parse_expression()` | Lowest precedence: `+`, `-` | `x + y - 3` |
| `parse_term()` | Medium precedence: `*`, `/` | `2*x / 3` |
| `parse_power()` | High precedence: `^` | `x^2` |
| `parse_unary()` | Unary minus | `-x^2` |
| `parse_primary()` | Atoms: numbers, variables, functions, parenthesized expressions | `(x+1)` |

### Operator Precedence

The parser enforces operator precedence naturally through the call hierarchy:

```
Higher precedence        parse_power()     ^
                         parse_unary()     unary -
                         parse_term()      *, /
Lower precedence         parse_expression()  +, -
```

### Function Parsing

When the parser encounters a variable token followed by `(`, it interprets it as a function call. Supported functions include `sin`, `cos`, `log`/`ln`, `exp`, and `sqrt` (which is internally represented as `x^0.5`).

---

## 2. Abstract Syntax Tree (AST)

### What is an AST?

An **Abstract Syntax Tree** is a tree representation where each node represents a construct occurring in the source expression. The tree abstracts away syntactic details (like parentheses and operator precedence) and captures only the semantic structure.

### Node Types

All nodes inherit from the abstract `Node` class and implement structural equality. Here are the node types:

| Node Class | Fields | Represents |
|-----------|--------|------------|
| `Number` | `value: float/int` | Numeric constant (e.g., `42`, `3.14`) |
| `Variable` | `name: str` | Named variable (e.g., `x`, `y`, `t`) |
| `Add` | `left, right: Node` | Addition: `a + b` |
| `Sub` | `left, right: Node` | Subtraction: `a - b` |
| `Mul` | `left, right: Node` | Multiplication: `a * b` |
| `Div` | `left, right: Node` | Division: `a / b` |
| `Pow` | `base, exp: Node` | Exponentiation: `a^b` |
| `Neg` | `operand: Node` | Unary negation: `-a` |
| `Sin` | `arg: Node` | Sine: `sin(a)` |
| `Cos` | `arg: Node` | Cosine: `cos(a)` |
| `Log` | `arg: Node` | Natural log: `log(a)` |
| `Exp` | `arg: Node` | Exponential: `exp(a)` |

### Example AST

For the expression `3*x^2 + 2*x + 1`:

```
          Add
         /   \
      Add     Number(1)
     /   \
   Mul    Mul
  /  \   /  \
Num  Pow Num Var(x)
(3)  / \ (2)
  Var  Num
  (x)  (2)
```

### AST Construction

The parser recursively builds these nodes. When a `+` operator is encountered in `parse_expression()`, it creates an `Add(left, right)` node. When `^` is found in `parse_factor()`, it creates a `Pow(base, exp)` node. This recursive construction mirrors the grammar structure.

---

## 3. Algebraic Simplification

### Why Simplify?

Symbolic operations often produce unnecessarily complex expressions. Simplification reduces expressions to a canonical (or at least tidier) form.

### Simplification Rules

The `simplify.py` module implements a recursive tree walk that applies rewrite rules.

#### Constant Folding

Arithmetic on concrete numbers is evaluated immediately:

| Rule | Result |
|------|--------|
| `simplify(Add(Number(2), Number(3)))` | `Number(5)` |
| `simplify(Mul(Number(4), Number(0.5)))` | `Number(2.0)` |
| `simplify(Pow(Number(3), Number(2)))` | `Number(9)` |

#### Identity Removal

Algebraic identities that cancel out:

| Rule | Simplified |
|------|-----------|
| `x + 0` | `x` |
| `0 + x` | `x` |
| `x * 1` | `x` |
| `1 * x` | `x` |
| `x * 0` | `0` |
| `0 * x` | `0` |
| `x / 1` | `x` |
| `x^1` | `x` |
| `x^0` | `1` (if x ≠ 0) |
| `0^x` | `0` (if x ≠ 0) |
| `x - 0` | `x` |
| `-(-x)` | `x` |

#### Like Terms Collection

Terms with the same variable part are combined:

```
x + x       →  2*x
3*x + 2*x   →  5*x
x^2 + 2*x^2 →  3*x^2
```

### Implementation

The `simplify()` function works recursively:
1. Simplify all child nodes first
2. Apply constant folding if both children are numbers
3. Apply identity removal
4. For `Add` nodes, collect like terms via `collect_like_terms()`

The `collect_like_terms()` function:
1. Flattens nested `Add` nodes into a list of terms using `extract_terms()`
2. Separates each term into a coefficient and a "rest" (the variable part)
3. Groups terms by their variable part
4. Reconstructs the expression with combined coefficients

---

## 4. Symbolic Differentiation

### Core Differentiation Rules

The `differentiate.py` module implements Leibniz's rules for symbolic differentiation:

| Expression `f(x)` | Derivative `f'(x)` |
|-------------------|-------------------|
| `c` (constant) | `0` |
| `x` | `1` |
| `x^n` | `n·x^(n−1)` |
| `f(x) + g(x)` | `f'(x) + g'(x)` |
| `f(x) − g(x)` | `f'(x) − g'(x)` |
| `f(x) · g(x)` | `f'(x)·g(x) + f(x)·g'(x)` (product rule) |
| `f(x) / g(x)` | `(f'(x)·g(x) − f(x)·g'(x)) / g(x)²` (quotient rule) |

### Trigonometric and Transcendental Functions

| Function | Derivative |
|----------|-----------|
| `sin(f(x))` | `cos(f(x)) · f'(x)` |
| `cos(f(x))` | `−sin(f(x)) · f'(x)` |
| `log(f(x))` | `f'(x) / f(x)` |
| `exp(f(x))` | `exp(f(x)) · f'(x)` |

### The Chain Rule

Every function's derivative rule includes the **chain rule**: differentiate the outer function, multiply by the derivative of the inner function. For example:

```python
# Sin: d/dx sin(f) = cos(f) * f'
Sin: Derivative = Mul(Cos(node.arg), differentiate(node.arg, var))

# Pow with non-trivial base: d/dx f(x)^n = n * f(x)^(n-1) * f'(x)
# (handled by chain rule when base is not just 'x')
```

### Power Rule Implementation

The power rule handles three cases:

1. **Variable directly**: `d/dx x^n = n·x^(n-1)`
2. **Composite base**: `d/dx f(x)^n = n·f(x)^(n-1)·f'(x)`
3. **Variable exponent**: `d/dx a^x = a^x · ln(a)`

### Example: Step-by-Step Differentiation

```
d/dx (x³ + 2x²)
= d/dx(x³) + d/dx(2x²)              [sum rule]
= 3·x² + 2·d/dx(x²)                  [power rule, constant factor]
= 3·x² + 2·2·x                       [power rule]
= 3·x² + 4·x                         [simplify]
```

---

## 5. Symbolic Integration

### Fundamental Rules

The `integrate.py` module implements **indefinite integration** (antiderivatives):

| Expression | Integral `∫ f(x) dx` |
|-----------|---------------------|
| `0` | `C` (constant) |
| `c` (constant) | `c·x + C` |
| `x^n` (n ≠ −1) | `x^(n+1)/(n+1) + C` |
| `1/x` | `ln|x| + C` |
| `sin(x)` | `−cos(x) + C` |
| `cos(x)` | `sin(x) + C` |
| `e^x` | `e^x + C` |
| `f(x) + g(x)` | `∫ f(x) + ∫ g(x)` (sum rule) |
| `c·f(x)` | `c·∫ f(x)` (constant multiple) |

### Integration Strategy

The current implementation uses a direct pattern-matching approach:

1. **Constant**: `∫ c dx = c·x`
2. **Variable**: `∫ x dx = x²/2`
3. **Power function**: `∫ x^n dx = x^(n+1)/(n+1)` (with special case n=−1 giving `log(x)`)
4. **Sum/Difference**: Distribute over `+` and `-`
5. **Constant factor**: Pull constants outside the integral
6. **Trigonometric**: Direct rules for `sin` and `cos`
7. **Exponential**: `∫ e^x dx = e^x`

### Limitations

Symbolic integration is fundamentally harder than differentiation:
- Many elementary functions don't have elementary antiderivatives (e.g., `e^(x²)`, `sin(x)/x`)
- The current implementation handles only basic cases; more complex integrals may not be computed correctly
- No integration by substitution or integration by parts is implemented yet

### Example: Step-by-Step Integration

```
∫ x² dx
= x³/3 + C            [power rule: n=2, so ∫ x² = x^(2+1)/(2+1)]
```

```
∫ (x³ + 2x) dx
= ∫ x³ dx + ∫ 2x dx    [sum rule]
= x⁴/4 + 2·∫ x dx      [power rule + constant multiple]
= x⁴/4 + 2·x²/2        [power rule]
= x⁴/4 + x²            [simplify]
```

---

## 6. Numeric Evaluation

### Purpose

While symbolic manipulation is powerful, we often need to compute actual numeric values. The `evaluate.py` module walks the AST and computes concrete results given variable assignments.

### Evaluation Algorithm

The `evaluate(node, env)` function recursively:

1. **Number**: Return `node.value`
2. **Variable**: Look up `node.name` in the environment dictionary; built-in constants `e` and `pi` are recognized
3. **Binary operations**: Evaluate left and right children, then apply Python's `+`, `-`, `*`, `/`, `**`
4. **Unary negation**: Evaluate operand, negate the result
5. **Functions**: Evaluate argument, then call `math.sin`, `math.cos`, `math.log`, or `math.exp`

### Example

```python
evaluate(parse("x^2 + 1"), {"x": 5})
# → 26.0
```

The function traverses the tree:
- `Add`: evaluate left `x^2` → 25, evaluate right `1` → 1, return 25 + 1 = 26
- `Pow`: evaluate base `x` → 5, evaluate exp `2` → 2, compute 5² = 25

---

## 7. REPL Usage

### Running the Interpreter

```bash
python main.py
```

Or evaluate a single expression:

```bash
python main.py "x^2 + 2*x + 1"
```

### Commands

#### `diff <expression> [variable]`

Differentiate an expression with respect to a variable (default: `x`):

```
>>> diff x^3 + 2*x^2
d/dx(x^3 + 2*x^2) = 3*x^2 + 4*x

>>> diff sin(x^2) x
d/dx(sin(x^2)) = cos(x^2)*(2*x)
```

#### `integrate <expression> [variable]`

Compute the indefinite integral:

```
>>> integrate x^2
∫(x^2) dx = (x^3)/(3) + C

>>> integrate cos(x)
∫(cos(x)) dx = sin(x) + C
```

#### `simplify <expression>`

Apply algebraic simplification rules:

```
>>> simplify x + x
simplify(x + x) = 2*x

>>> simplify 3*x^2 + 2*x^2 + x
simplify(3*x^2 + 2*x^2 + x) = 5*x^2 + x

>>> simplify x*0 + 1
simplify(x*0 + 1) = 1
```

#### `eval <expression> var1=val1 var2=val2 ...`

Evaluate an expression with given variable values:

```
>>> eval x^2 + 1 x=3
eval(x^2 + 1) at x=3 = 10.0

>>> eval sin(x) + cos(y) x=0 y=0
eval(sin(x) + cos(y)) at x=0, y=0 = 1.0
```

#### `expand <expression>`

Expand/simplify an expression (currently an alias for simplify):

```
>>> expand (x+1)^2
expand((x+1)^2) = (x+1)^2
```

#### Raw Expression

If the input doesn't match a command, it's parsed and simplified:

```
>>> x^2 + 2*x + 1
x^2 + 2*x + 1
```

#### `quit`

Exit the REPL.

---

## Project Structure

```
symbolic-math-interpreter/
├── main.py              # REPL interface and expression stringifier
├── parser.py            # Tokenizer and recursive descent parser
├── ast_nodes.py         # AST node class definitions
├── simplify.py          # Algebraic simplification engine
├── differentiate.py     # Symbolic differentiation
├── integrate.py         # Symbolic integration
├── evaluate.py          # Numeric evaluation
└── data/
    └── functions.py     # Function metadata definitions
```

## Further Reading

- Aho, Lam, Sethi, Ullman. *Compilers: Principles, Techniques, and Tools* (the "Dragon Book").
- Cohen, J. S. *Computer Algebra and Symbolic Computation: Mathematical Methods*.
- Geddes, Czapor, Labahn. *Algorithms for Computer Algebra*.
- Wikipedia: [Recursive descent parser](https://en.wikipedia.org/wiki/Recursive_descent_parser), [Symbolic computation](https://en.wikipedia.org/wiki/Symbolic_computation), [Differentiation rules](https://en.wikipedia.org/wiki/Differentiation_rules), [List of integrals](https://en.wikipedia.org/wiki/List_of_integrals)
