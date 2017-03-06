[![Build Status](https://travis-ci.org/Rasie1/c-of-x.svg?branch=master)](https://travis-ci.org/Rasie1/c-of-x.svg)
[![Coverage Status](https://coveralls.io/repos/github/Rasie1/c-of-x/badge.svg?branch=master)](https://coveralls.io/github/Rasie1/c-of-x?branch=master)

# Welcome

This is an experimental programming language inspired by Haskell and C++ template metaprogramming.

## Build & Run

### Dependencies

- c++14 compiler
- readline
- boost

### How to build

Run following steps in terminal to compile the language:

```
mkdir build
cd build
cmake ..
make
```

### How to run

To run interpreter, execute `repl` binary which is placed in `build/repl` directory after build.

To run tests, execute `ctest` from `build` directory.

## Core Ideas

*Warning: most of code below is now in theorethical stage and can be run in interpreter only with syntactic sugar removed. Stay tuned: the language is in heavy development right now*

### Philosophy

- Protect from as many types of errors as possible with advanced type system
- Maximize readability by separating algorithm and data from implementation details
- Provide the highest level of abstraction, expressiveness and concision

### Syntax
Clear Haskell-like syntax with heavier usage of indentation and absolute lack of keywords.

### Gradual Refinement Types
Which let you to construct predicates with dynamic variables and put them **instead of types**.
```scala
main = a <- read Int
       myFunc arg:(>a) = arg + 100 // f takes anything more than a
       myFunc _        = 0
       b <- read Int
       if b > a
          print myFunc(b)
       else
          print "Couldn't put `myFunc(b)` in this block"
```

### Intersection and Union
Which enable you to easily combine types and predicates.
```haskell
0 < x < 10
myVariable0 : {x} | (>100) | String
myVariable1 = x + 10|11|12
```

### Function and First Class Types
You can even put an anonymous function instead of type.
```
Even x = (x % 2 = 0)
myFunction x:Even = 9

myVariable : (a, b, c) => a + b + c > 10
          // ^ this is type
```

### Inverse Functions
A system for unapplying functions
```
myDecrement (x + 1) = x
print myDecrement(2)

myParser a++":"++b = a ++ b
myParser "123:123"

myInt0 (Int x) = x
myInt1 x = Int x
myInt2 = Int
```

# What's Next?
Currently, the language is in a design & research stage. This repository is a prototype implementation of interpreter & type checker which is still under development. Examples above are working partially and some syntactic sugar is not implemented, but you can already try some possibilities of the type system.

Currently, I'm working on the implementation and papers about type system.
