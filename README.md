Warning: this document is under heavy development right now. 

# Welcome to c(x)

This is an experimental programmable programming language that was designed with the idea to maintain expressiveness and usability while being fully able to meet functional, object oriented, logical or any other paradigm at the same time.

We have:

+ Dependent refinement types and beyond

+ Ability to be compiled, interpreted and JIT-compiled

+ Higher order overloadable functions, types and operators

+ Mathematically-correct imperative style

+ Optional purity, safety and automatic theorem proving

+ Full dependent pattern matching

+ Evaluation control, laziness and strictness

+ Highly convenient object oriented features

+ Pointer arithmetics and memory control

+ Multi-thread features, optional automatic parallelism

+ Automatic selection of interface implementation based on performance

+ Packaging system inside language, easy to use FFI

+ Interactive development features

+ Human readability while staying concise


Let's begin with new features we've been considering. 

**Sets define types. Predicates define sets.**

```c
                    // Let's define a value
x : integer, (>= 0) // Here we say that x is a non-negative integer
                    // Now, let's make it more specific.
x < 5               // Actually, this is some kind of assignment.

print x+1           // "12345" is printed
```
Let's think about what happened. "print" takes just one value. However, at the stage of using it, it is only known that "x" is in range [0; 5). This means that "print" could be applied to any of them. And as the function is impure, it has to be evaluated right now, so it iterates all possible x's, and it can be done thanks to the integer being an ordered finite set.

However, here is overloaded "print" that takes set and some strings:
```c
print xs         : container, ordered
      delimiter  : string
      terminator : string 
  = for x in xs \ xs[-1]  // Iterate on each value in "xs" excluding last
        print (x, ", ")   // Call to overloaded "print" that takes tuple
    print (xs[-1], ";")   // as an argument
// Usage
print [1..5] with delimiter ", " and terminator ";\n'
// "1, 2, 3, 4, 5;\n" is printed
```
What's up with those "delimiter", "terminator", "with", "and" words? They are named arguments ("delimiter", "terminator") and empty words ("with", "and"). Both things are fully optional. They can separate things written in 1 line, decreasing need of parentheses, allow changing order of arguments and they also can be optional arguments. By the way, "for" and "if" are defined like that. There is no such thing as "keywords" in this language.

Now, about object-oriented stuff. Here we create and "inherit" classes, override methods and use events (those are just functions that can be called when another events are executed)

```c
Creature = class
         - health : integer, (< 200) // Notice "-", "+" and "#" as in UML notation
                    property         // Makes variable exposed in constructor
           die    : void -> void
                    event
           dead   : boolean
         + takeDamage : integer -> void
                      $ amount
                      = if amount >= health then die()
                                                 dead = true
                                            else health -= amount

Human = Creature
      - damage     : integer = random {1..99}
        experience : integer = 0
        die   : override
        die() = print "Ugh" 
      + attack enemy = enemy.takeDamage this.damage
                       onAttackedMonsterDeath.boundEvents = {}
                       onAttackedMonsterDeath.bind enemy.die
                                                   with argument enemy
      - onAttackedEnemyDeath : event
                             $ enemy : Creature
                             = experience += 100
                             
// Usage
a, b : Human
for i in range(0, 10)
    a.attack b
```
