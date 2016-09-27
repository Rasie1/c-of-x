# Lexical Structure

Lexical analyzer is intended to have more complex structure than it is common. It have to gather some information from the language enviroment to provide new features.
 
Also, the language is indentation-sensitive and uses different indentation levels to express different things, that are determined by strict set of rules and it is dependent on data from previous lines.

Programs are represented with sequences of Unicode characters. Unicode escapes (like ```\u2665```) can be used to represent program using only ASCII characters.

It is recommended to represent program with monospace characters as it relies on space and character length. Different-width characters will make program unreadable.

At the end of lexical analysis, sequence of unicode characters is reduced to sequence of tokens - input elements that are used in Parsing stage.

Tokens are Identifiers, Literals, Tabulations, Line Breaks, No-Spaces, Opening Braces, Closing Braces and Comments.

## Issues

+ No example for application with tabulations
+ Case when two indentation levels are overlapping is not explained
+ Should somehow redefine Tokens, Identifiers and Non-Space characters for making better explanation possible

## Tokens

### Identifier

Identifier can be any sequence of characters.

### Literal

+ Integer Literal ```123```, ```-12```, ```00```
+ String Literal ```"myString"```, ```""```
+ Floating Point Literal ```123.0```, ```.043```, ```25.```

### Tabulations

Consecutive spaces at beginning of line are used to create Tabulation token.

One tabulation token can represent multiple indentation levels: it may hold unsigned integer value.

Possible indentation levels are determined by previous line: 

+ At the beginning of the line
+ Third character of line
+ New indentation levels are set at the same distance from the beginning of line, where space character after non-space character is met on the previous line. 
+ If blah blah, then third character level will be the only. todo: explain

Here is an example of different indentation levels:

```
myFunction myArgument = 
0-1-------2----------3-4
```
Here is example of program that relies on this indentation scheme:

```
main args =
todo: nice example of program

```

If no readable token is seen on current line, tabulation token is not generated.

### Line Breaks

Line break is meaningful to parser. However, Lexer skips subsequent Line Break as the behaviour shouldn't change depending on number of line-breaks in the row.

### No-Spaces

Operator priority can be manipulated using No-Space tokens, so parser should know where exactily No-Space is used.

Writing two tokens without spaces increases operator priority to make usage more comfortable.

Here are some examples

```
print sin(x) // the same as "print(sin x))"
print sin x  // the same as "(print sin)(x)"

print a+b    // the same as "print(a+b)"
print a + b  // the same as "(print a) + b"
```

Following two expressions are intended to mean different things

```
print (2 + 2 * 2)
print (2+2 * 2)
```

Cases when No-Space token is included between tokens (todo: need common name for identifier and splitting sequence and possibly splitting sequence and brace):

+ No space between splitting sequence and identifier
+ No space between splitting sequence and splitting sequence
+ No space between identifier and opening brace at right
+ No space between closing brace at left and identifier

### Opening & Closing Braces

There are multiple kinds of opening braces:

+ Parenthesis ```(abc)```, ```()```
+ Brackets ```[1, 2, 3]```
+ Curly braces ```{1, 2, 3}```
+ Tags ```<myIdentifier>abc</myIdentifier>``` - those can hold any identifier inside and basically mean function application

### Comments

```
// This is single-line comment
/*
This is multi-line comment
*/
```
