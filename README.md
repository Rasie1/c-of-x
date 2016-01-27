Warning: this document is under heavy development right now. 

# Welcome to c(x)

This is an experimental programmable programming language that was designed with the idea to maintain excessive expressiveness while being comfortable to use and easy to read. The language meets object-oriented, functional and logical paradigms and provides endless possibilities to invent new ways of solving all kinds of problems.

We have:

+ Dependent refinement types and beyond

+ Compilation and optimization is done within the language

+ Higher order overloadable functions, types and operators

+ Mathematically-correct imperative style based on simple system

+ Optional purity, safety and totality checking

+ Pattern matching and expression unwinding

+ Powerful macro system and evaluation control

+ Pointer arithmetics and memory control

+ Interactive development features

+ Easy-to-use FFI


Let's begin with features we've been considering. 

**Sets define types. Predicates define sets. Predicates are filters**

```c
             // Let's define a value
x : int, > 0 // Here we say that x is a positive integer
             // Now, let's make it more specific.
x < 10       // Actually, this is some kind of assignment.
             // And x is now an intersection of three sets:
             // integers, positive numbers and numbers < 10
             // Let's check it

if 0 < int x < 10 
   then print "true"
   else print "this will not get printed"
```
That's how you can deal with predicates.

Things you may have noticed:

+ Operator ":" is just a syntactic sugar, "A : B" is the same as "B A". There are some more operators that do nothing but change order or priority.

+ "int" takes a value as an argument and returns it if it's in set. That's how types work here.


```c
             // Let's define a value
x : int, > 0 // Here we say that x is a positive integer
             // Now, let's make it more specific.
x < 10       // Actually, this is some kind of assignment.
             // And x is now an intersection of three sets:
             // integers, positive numbers and numbers < 10
             // Let's check it

if 0 < int x < 10 
   then print "true"
   else print "this will not get printed"
```
That's how you can deal with predicates.

Things you may have noticed:

+ Operator ":" is just a syntactic sugar, "A : B" is the same as "B A". There are some more operators that do nothing but change order or priority.

+ "int" takes a value as an argument and returns it if it's in set. That's how types work here.

+ You can view "> 0", "isUppercase" and literally any other function as type: there is no distinction.

Now, let's do something strange:

```c
print x+1 // What gets printed is "2345678910"
```
How is that? We didn't know what exactly "x" was!

This is indeterminism. But, by saying that "x" is an integer, we had stated that it's one of infinite number of values. Then we are cutting both sides of infinity with other predicates and what left is only 9 values.

We could do any operation on that "x" if the operation supports taking arguments of the same type of "x". It just wouldn't evaluate until it's needed. And then we triggered the evaluation using impure "print" - it has to iterate all possible variants.

Compiler would give us a warning about ambiguous "x" used with impure, because such situation might be unwanted. 

Don't confuse "x" with sets. Sets are containers holding multiple values, and "x" is just a value that can be equal to either one or another value.

Next, some more syntactic sugar:

```c
xs = ^x  // Operator for constructing set from indeterministic values.
         // Let's leave only even values is xs using
         // anonymous function, equality and modulo operators:
xs &= (x%2 = 0) => x
         // Call an overloaded "print" function
print mySet delimiter ", " terminator ";\n"
         // stdout: "2, 4, 6, 8;\n"
```

As you can see, here we just intersect a set with an anonymous function. That may sound pretty anti-scientific at first, but under the hood it's logical enough: it takes value only if it can fit in the expression at left of "=>"

Writing operators without spaces increases operator precedence and reduces need for parentheses: "2+2 \* 2 = 8" and "2 + 2\*2 = 6"

Who the heck are those "delimiter", "terminator"? Argument names. They are optional and can be used for changing order of arguments and separating expressions (reducing number of parentheses)

Let's take a look at how this "print" may be defined

```c
print xs         : container, ordered
      delimiter  : string, default " "
      terminator : string, default "\n"
  = for x in xs \ xs[-1]    // Iterate on each value in "xs" excluding last
        print x & delimiter // Calling regular print
    print xs[-1] & terminator   
```
**Objects**

Let's observe how this language is object-oriented:

```c
Person = class, abstract
         construct money skill
         has money : int = 0
             skill : int = 0
             think : void -> string

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


```c
f (x + 10) = x
print f(15)    //Writing calls like that increases precedence
    //stdout: 5
```
This is called unwinding. You can do this for every function if you define the "opposite" operation. As you may have notices, the "opposite" operation for constructing types is just unwrapping the value from type. You can define the language to solve even differential equations involving this syntax.

But what if we do

```c
f (x + y) = print x, print y
f 1+2
```
The language will give you an error on "print x", "print y" because you can't iterate on infinite containers. The reason of that is "1 + 2" should be calculated before applying to a function. So, "f" gets "3" as an argument, and there is an infinite number of ways to solve "x + y = 3".

Now let's take a look what macros can do.
```c
f #(x + y) = print x, print y
f 1+2 //stdout: 12
```
So, macro is just a function takes inevaluated arguments. "1 + 2" is fed to the function before it gets calculated, so "f" knows what "x" and "y" are.

You can think of "f" as a function taking "code", or "abstract syntax tree". You can match agaisnt literally everything.

Macros are first class objects and behaviour of passing macros around is resolved at compile-time.
