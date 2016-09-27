Warning: this document is under heavy development right now. 

# Welcome to c(x)

This is an experimental programmable programming language that was designed with the idea to deliver excessive expressiveness while being comfortable to use and easy to read. The language meets object-oriented, functional and logical programming paradigms and provides endless possibilities to invent new ways of solving all kinds of problems.

The language specification, introduction and core ideas design are still under development and will be available shortly.

The language is aimed to:

+ 

+ Provide an ability to express maximum level of abstraction as one can think of

+ Be easily writeable, readable and maintanable

+ Have minimalistic core syntax, that can be extended in-place by user to have any syntactic sugar

The language is intended to have:

+ Static refinement types and beyond

+ Mathematically-correct declarative style based on simple system

+ Higher order overloadable functions, types and operators

+ Optional purity, safety and totality checking

+ Pattern matching and expression unapplying

+ Powerful macro system and evaluation control

+ Interactive development features

## Build & Run

Run following steps in terminal to compile the language (note: you should have C++17 compiler available):

```
sudo apt-get install libreadline-dev
mkdir build
cd build
cmake ..
make
```

To run interpreter, execute `Language` binary.

To run tests, execute `ctest` from `build` directory.

## Some examples

```scala
Natural = Integer, (> 0)
Even x = 
    x :mod: 2 = 0

Creature = has numberOfHands : Odd, Natural

Human = Creature
        has numberOfHands = 2

you : Human
```

## Thanks for visiting!

More examples and introductions will appear here soon. If you are interested, check back shortly.
