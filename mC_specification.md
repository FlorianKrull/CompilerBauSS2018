# mC Specification

This document defines *mC* -- a tiny, C-like language used throughout this course.
The semantics of mC are identical to C unless specified otherwise.

Please note that the specification may contain errors and ambiguities.
It may therefore be modified slightly throughout the course to correct for such issues.

## Grammar

The next segment presents the grammar of mC using this notation:

- `#` starts a line comment
- `,` indicates concatenation
- `|` indicates alternation
- `( )` indicates grouping
- `[ ]` indicates optional parts (0 or 1)
- `{ }` indicates repetition (1 or more)
- `[ ]` and `{ }` can be combined to build 0 or more repetition
- `" "` indicates a terminal string
- `/ /` indicates a [RegEx]

[RegEx]: https://www.regular-expressions.info/

```
# Primitives

alpha            = /[a-zA-Z_]/

alpha_num        = /[a-zA-Z0-9_]/

digit            = /[0-9]/

identifier       = alpha , [ { alpha_num } ]

bool_literal     = "true" | "false"

int_literal      = { digit }

float_literal    = { digit } , "." , { digit }

string_literal   = /"[^"]*"/


# Operators

unary_op         = "-" | "!"

binary_op        = "+"  | "-" | "*" | "/"
                 | "<"  | ">" | "<=" | ">="
                 | "&&" | "||"
                 | "==" | "!="


# Types

type             = "bool" | "int" | "float" | "string"


# Declaration / Assignment

declaration      = type , [ "[" , int_literal , "]" ] , identifier

assignment       = identifier , [ "[" , expression , "]" ] , "=" , expression


# Expressions

expression       = single_expr , [ binary_op , expression ]

single_expr      = literal
                 | identifier , [ "[" , expression , "]" ]
                 | call_expr
                 | unary_op , expression
                 | "(" , expression , ")"

literal          = bool_literal
                 | int_literal
                 | float_literal
                 | string_literal


# Statements

statement        = if_stmt
                 | while_stmt
                 | ret_stmt
                 | declaration , ";"
                 | assignment  , ";"
                 | expression  , ";"
                 | compound_stmt

if_stmt          = "if" , "(" , expression , ")" , statement , [ "else" , statement ]

while_stmt       = "while" , "(" , expression , ")" , statement

ret_stmt         = "return" , [ expression ] , ";"

compound_stmt    = "{" , [ { statement } ] , "}"


# Function Definition / Call

function_def     = ( "void" | type ) , identifier , "(" , [ parameters ] , ")" , compound_stmt

parameters       = declaration , [ { "," , declaration } ]

call_expr        = identifier , "(" , [ arguments ] , ")"

arguments        = expression , [ { "," expression } ]


# Program

program          = [ { function_def } ]
```

## Comments

mC supports only *C-style* comments, starting with `/*` and ending with `*/`.
Like in C, they can span across multiple lines.
Comments are discarded by the parser, but do not forget to take newlines into account for line numbering.

## Special Semantics

### Boolean

For mC we consider `bool` a first-class citizen, distinct from `int`.
The operators `!`, `&&`, and `||` can only be used for booleans.

### Strings

Strings are immutable and do not support any operation (eg concatenation).
Yet, like comments, strings can span across multiple lines.

Their sole purpose is to be used with the built-in `print` function for which an implementation will be provided by the compiler.

### Arrays

Only 1D arrays with static size are supported.
The size must be stated during declaration and is part of the type.
The following statement declares an array of integers with 42 elements.

    int[42] my_array;

We do not support *any* operations on whole arrays.
For example, the following code is *invalid*:

    int[10] a;
    int[10] b;
    int[10] c;

    c = a + b;    /* not supported */

You'd have to do this via a loop, assigning every element:

    int i;
    i = 0;
    while (i < 10) {
    	c[i] = a[i] + b[i];
    	i = i + 1;
    }

### Call by Value / Call by Reference

`bool`, `int`, and `float` are passed by value, while `string` and arrays are passed by reference.

### Type Conversion

There are no type conversion, neither implicit nor explicit.

An expression used as a condition (for `if` or `while`) is expected to be of type `bool`.
This will be enforced later on by the type checker.

### Entry Point

Your top-level rule is `program` which simply consists of 0 or more function definitions.
While the parser happily accepts empty source files, we will later enforce that one function named `main` must be present.

### Declaration, Definition, and Initialization

`declaration` is used to declare variables which can then be initialised with `assignment`.

Furthermore we do not provide a way to declare functions.
All functions are declared by their definition.
It is possible to call a function before it has been defined.

### Empty Parameter List

In C, the parameter list of a function taking no arguments contains only `void`.
For mC we simply use an empty parameter list.
Hence, instead of writing `int foo(void)` we write `int foo()`, where `foo` is the name of a function returning an `int` and taking no arguments.

### Dangling Else

A [*dangling else*](https://en.wikipedia.org/wiki/Dangling_else) belongs to the innermost `if`.
The following mC code snippets are semantically equivalent:

    if (c1)              |        if (c1) {
        if (c2)          |            if (c2) {
            f2();        |                f2();
        else             |            } else {
            f3();        |                f3();
                         |            }
                         |        }

## I/O

The following built-in functions will be provided by the compiler for I/O operations:

- `void print(string)`      outputs the given string to `stdout`
- `void print_nl()`         outputs the new-line character (`\n`) to `stdout`
- `void print_int(int)`     outputs the given integer to `stdout`
- `void print_float(float)` outputs the given float to `stdout`
- `int read_int()`          reads an integer from `stdin`
- `float read_float()`      reads a float from `stdin`

An implementation is provided in [`mC_builtins.c`](mC_builtins.c).

With these, we can create simple, interactive programs.
