Warning: this document is under heavy development right now. 

# Welcome to c(x)

This is an experimental programmable programming language that was designed with the idea to maintain excessive expressiveness while being comfortable to use and easy to read. The language meets object-oriented, functional and logical programming paradigms and provides endless possibilities to invent new ways of solving all kinds of problems.

We have:

+ Static refinement types and beyond

+ Mathematically-correct declarative style based on simple system

+ Higher order overloadable functions, types and operators

+ Optional purity, safety and totality checking

+ Pattern matching and expression unwinding

+ Powerful macro system and evaluation control

+ Pointer arithmetics and memory control

+ Interactive development features


Let's begin with features we've been considering. 

**Sets, types, predicates and filters work the same way**


```scala
             // Let's define a value
x : int, > 0 // Here we say that x is a positive integer
             // Now, let's make it more specific.
x < 10       // x is now an intersection of three sets:
             // integers, positive numbers and numbers < 10
             // Let's check it

if 0 < int x < 10 
   then print "true"
   else print "not printed"
```
Things you may have noticed:

+ Operator ":" is just a syntactic sugar, "A : B" is the same as "B A". There are some more operators that do nothing but change order or priority.

+ How types work here: they take value as an arguments and return it only if it belongs to the type.

+ You can view "> 0", "isUppercase" and literally any other predicate, or even function as type: there is no distinction at all.

Now, let's do this:

```scala
x : range(2, 6), int
print x-1 // What gets printed is "12345"
```
How is that? We didn't know what exactly "x" was!

This is indeterminism. You can remember SQL languages: their variables can hold multiple values. In this language, the only thing we know of "x" is that it's between 0 and 4. Thanks for "int" being iterable, we can run "print" function on every possible value of "x".

Don't confuse "x" with sets. Sets are containers holding multiple values, and "x" is just a value that can be equal to either one or another value of the set it belongs to.

Next, some more syntactic sugar:

```scala
xs = ^x  // Operator for constructing set from indeterministic values.
     (x%2 = 0) => x
         // Yeah, that was lambda-expression as type. "xs" now holds
         // only even values

         // Call an overloaded "print" function
print xs delimiter ", " terminator ";\n"
         // stdout: "2, 4;\n"
```

As you can see, here we just intersect a set with an anonymous function. That may sound pretty anti-scientific at first, but under the hood it's logical enough: it takes value only if it can fit in the expression at left of "=>"

Writing operators without spaces increases operator precedence and reduces need for parentheses: "2+2 \* 2 = 8" and "2 + 2\*2 = 6"

Who the heck are those "delimiter", "terminator"? Argument names. They are optional and can be used for changing order of arguments and separating expressions (reducing number of parentheses)

Let's take a look at how this "print" may be defined:

```scala
print xs         : iterable
      delimiter  : string, default " "
      terminator : string, default "\n"
  = for x : xs \ xs[-1]     // Iterate on each value in "xs" excluding last
        print x & delimiter // Calling regular print
    print xs[-1] & terminator   
```

**Objects**

Let's observe how this language is object-oriented:

```scala
Person = abstract
         construct money skill
         has money : int = 0
             skill : int = 0
             think : void -> string, virtual

Student = Person
          has study() = knowledge++
              think() : override
                      = "a"

Worker = Person
         has work()  = money += skill
             think() : override
                     = "b"

Scientist = Worker, Student
            has think = Worker::think 
```


...

WIP README PART

...


```scala
f (x + 10) = x
print f(15)    //Writing calls like that increases precedence
    //stdout: 5
```
This is called unwinding. You can do this for every function if you define the "opposite" operation. As you may have notices, the "opposite" operation for constructing types is just unwrapping the value from type. You can define the language to solve even differential equations involving this syntax.

Some functions are irreversible:

```scala
f (x + y) = print x, print y
f 1+2
```
The language will give you an error on "print x", "print y" because you can't iterate on infinite containers. The reason of that is "1 + 2" should be calculated before applying to a function. So, "f" gets "3" as an argument, and there is an infinite number of ways to solve "x + y = 3".

Now let's take a look what macros can do.
```scala
f #(x + y) = print x, print y
f 1+2 //stdout: 12
```
So, macro is just a function takes inevaluated arguments. "1 + 2" is fed to the function before it gets calculated, so "f" knows what "x" and "y" are.

You can think of "f" as a function taking "code", or "abstract syntax tree". You can match against literally everything.

Macros are first class objects and behaviour of passing macros around is resolved at compile-time.
