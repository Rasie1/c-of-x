Warning: this document is under heavy development right now. 

# Welcome to c(x)

This is an experimental programmable programming language that was designed with the idea to deliver excessive expressiveness while being comfortable to use and easy to read. The language meets object-oriented, functional and logical programming paradigms and provides endless possibilities to invent new ways of solving all kinds of problems.

*Warning: the language specification and core ideas design are still under development and will be available soon.*

This language is aimed to give user an ability to safely express his (her) thoughts at any level of abstraction and describe any relations between objects, while staying as much easy to read, learn and write as possible.

The language is intended to have:

+ Static refinement types and beyond

+ Mathematically-correct declarative style based on simple system

+ Higher order overloadable functions, types and operators

+ Optional purity, safety and totality checking

+ Pattern matching and expression unapplying

+ Powerful macro system and evaluation control

+ Minimalistic core syntax, that can be extended in-place by user to have any syntactic sugar

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

Creature = has numberOfHands : Even, Natural

Human = Creature
        has numberOfHands = 2

you : Human
```

Be aware, not all features are implemented and it will be a long time until you can run the code above.

## Thanks for visiting!

More examples and introductions will appear here soon. If you are interested, check back shortly.
