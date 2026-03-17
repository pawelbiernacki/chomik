# Chomik

**Chomik** is an experimental, minimalistic, interpreted general-purpose programming language with a radical idea at its core: *naming and evaluating are two different things*.

In most programming languages, a variable name is inseparable from the act of reading or computing its value. Chomik separates these explicitly. Names are first-class — they can be constructed dynamically at runtime, they can span multiple identifiers and literals, and they can name things that are not yet computed, not yet known to exist, or not yet constructable. Evaluation is a separate, explicit act performed with the `<...>` operator.

The result is a language with no data structures, no explicit loops, no conditional instructions, and no function parameters — yet powerful enough to write games, web servers, expert systems, and graphical applications.

Chomik is open source (GPL 3.0), written in C++17, and has been in active development since April 2022.

---

## The Core Idea

In C, writing `x` in an expression both names and evaluates simultaneously. In Chomik these are two distinct acts:

```chomik
variable my answer : integer;
let my answer = value integer 42;
<print <my answer>>;
```

`my answer` is a name. `<my answer>` is an evaluation — explicitly asking the machine to look up what that name refers to. This separation is not cosmetic. It enables everything else.

---

## Key Features

### No data structures — families of variables instead

Variable names in Chomik are sequences of identifiers, literals (of any built-in type), and *placeholders*. A placeholder like `(X:boolean)` causes Chomik to create a whole family of variables — one for each value in the type. The Cartesian product of all placeholders in a name defines the family.

```chomik
type person = { Alice, Bob, Carol };
type place  = { home, office, cafe };
type event  = { (P:person) is at (L:place),
                (P:person) met (Q:person) at (L:place) };
expand(2);

# variable declarations are parsed but do not create variables by themselves
variable (E:event) happened : boolean;

# assigning creates the variables - this assigns false to all of them at once
let (E:event) happened = value boolean false;

# override specific ones
let Carol met Alice at office happened = value boolean true;
let Carol met Bob at office happened = value boolean true;

# print all events with their current values
<print (E:event) "happened" "=" <(E:event) happened>>;
```

Arrays, structs, maps, and objects are all just families of variables with well-chosen names.

### No loops

Iteration over a set is implicit. Mentioning a placeholder in an executable context iterates over all its values:

```chomik
# Prints all 12 combinations, no loop needed
<print (A:1..4) "squared is" (B:1..4)>;

# Assigns 100 to all ten variables at once
let alpha (N:1..10) = value integer 100;
```

### No conditional instruction

Chomik has a built-in `boolean` enumeration and a built-in `code` type. Branching is just name resolution:

```chomik
let handle false = value code { <print "it is false">; };
let handle true  = value code { <print "it is true">; };

variable my flag : boolean;
let my flag = value boolean true;

<handle <my flag>>;   # evaluates to <handle true>, executes that code
```

The `<...>` evaluation operator can be nested arbitrarily. No `if`, no `switch`, no `?:`.

### No function parameters

There are no functions in Chomik. Instead, the "parameters" are part of the variable's name:

```chomik
let greet (NAME:string) = value code
{
    let message = value string [(NAME:string)];
    <print "Hello," <message>>;
};

<greet "world">;
<greet "Chomik">;
```

### Powerful enumerations

Enumerations in Chomik can be recursive, mutually dependent, and used as variable names or parts of variable names:

```chomik
type person  = { Alice, Bob, Carol };
type place   = { home, office, cafe };
type event   = { (P:person) is at (L:place),
                 (P:person) met (Q:person) at (L:place) };
expand(2);

# Declare a variable for each possible event
variable (E:event) happened : boolean;
```

`expand(N)` sets the recursion depth for mutually recursive type definitions.

### Ad hoc enum lists

Since version 0.2.3, inline enum lists can be used directly without a type declaration:

```chomik
<print (X:list {alpha, beta, gamma})>;
```

### Complex (compound) type names and namespaces

Since version 0.2.2, type names can be composed dynamically, enabling true library namespacing without a namespace keyword:

```chomik
let my lib prefix = value integer 0;
include "my_library.chomik"

let my lib prefix = value integer 1;
include "my_library.chomik"   # two independent instances of the same library
```

Inside the library, all types and variables use `<my lib prefix>` as part of their name. The client controls the namespace simply by setting a variable.

---

## Getting Started

### Prerequisites

- C++17 compiler (GCC or Clang)
- autotools (`autoconf`, `automake`, `libtool`) — or CMake (also supported)
- For SDL\_Chomik: SDL2, SDL2\_ttf, SDL2\_image

### Building

```bash
git clone https://github.com/pawelbiernacki/chomik.git
cd chomik
autoreconf -i
./configure
make
sudo make install
```

### Hello World

```chomik
#!/usr/local/bin/chomik
<print "Hello world">;
```

Run with:

```bash
chomik hello.chomik
```

---

## The Package

The Chomik repository contains several components:

### `chomik` — the core interpreter

The main Chomik interpreter and `libchomik`, the C++ library for embedding Chomik as a scripting language in your own projects or extending it with new built-in variable families.

### `sdl_chomik` — graphical dialect

An extension of Chomik that inherits all core features and adds bindings to SDL2, SDL2\_ttf, and SDL2\_image. Used for writing graphical applications and games entirely in Chomik. SDL\_Chomik programs use the same syntax and the same parser — only the machine is different.

```bash
sdl_chomik my_game.sdl_chomik
```

### `http_chomik` and `fancy_http_chomik` — web sandboxes

Two Chomik-based HTTP server environments. Chomik programs running inside these sandboxes can generate HTML dynamically. `fancy_http_chomik` provides a richer environment. These demonstrate Chomik's potential as a server-side scripting engine.

### `libhttpchomik` — HTTP library

A C++ library for building your own Chomik-based HTTP applications.

---

## A Larger Example

This snippet demonstrates the combination of families, implicit iteration, and conditional dispatch — no loops, no if statements:

```chomik
type result = { win, loss, draw };
expand(1);

let message win  = value code { <print "You won!">; };
let message loss = value code { <print "You lost.">; };
let message draw = value code { <print "It's a draw.">; };

variable outcome : result;
let outcome = value result win;

<message <outcome>>;
```

And arithmetic, which follows the same pattern — built-in operations write their result into a named variable:

```chomik
variable x : integer;
let x = value integer 7;

<add "integer" <x> 3>;
let x = <the add result "integer">;   # x is now 10

<print <x>>;
```

---

## Philosophy

Chomik is an argument that *naming* and *evaluating* are fundamentally different acts that programming languages have conflated since Fortran. A mathematician can name an object without knowing whether it exists, whether it is computable, or how to construct it. Traditional languages cannot express this distinction — if you can compile the code, the object exists and is computable, by assumption.

Chomik makes the distinction explicit. Names are sequences of identifiers and literals. Evaluation is an operator. The structure of a program lives in its names — not in its control flow, data structures, or parameter lists. This makes obfuscation semantically destructive: you cannot rename `handle true` to `h1` without breaking the conditional dispatch that depends on that name.

The consequence is that Chomik rewards good naming and makes bad naming mechanically costly. The quality of the names *is* the quality of the program.

---

## Project Status

Chomik is a hobby project by Pawel Biernacki, developed in Kraków and Vantaa since 2022. It is experimental and evolving. The interpreter is functional and has been used to write games, graphical applications, and web servers. A compiler backend (using LLVM) has been started but is not a current priority — the language itself is the focus.

Contributions, questions, and experiments are welcome.

**Website:** [https://www.chomik.tech](https://www.chomik.tech)  
**Tutorial:** [https://www.perkun.org/chomik_tutorial.html](https://www.perkun.org/chomik_tutorial.html)  
**Blog:** [https://chomik.hashnode.dev](https://chomik.hashnode.dev) — articles on Chomik, SDL\_Chomik, and the "Chomik For Programmers" series  
**Contact:** pawel.f.biernacki@gmail.com

---

## License

GPL 3.0. For commercial use, contact the author.
