# c(x)

This is a in-development prototype for c(x) (or c-of-x) functional programming language that can work as extension to C.

The project aims not to compete with C or any other language in terms of performance, but to show some new beautiful ideas/semantics/syntax inspired by Haskell flexibility and C clarity.

It does sacrifice some type safety to provide math-like syntax with automatic type deduction from the context. 

The programming language is interpreted, so a lot of metaprogramming will be possible, for example, you will be able to tear out whole lines of your functions while paying (or not paying) attention to their environment).

If you are interested, I expect you to come back in a short time because now I can show you only:

1. Parser that still only supports application of argument to function (without any priority, binary operators, etc)
2. Working lists, assignments, lambda expressions and some random stuff like reading from file, etc.

To get you interested, I will show you an example of what you will be able to do:

This is a correct program on c(x)

	#include <stdio.h>

	int f(int x, int y)
	{
		int i;
		for (int i = 1; i < 10; ++i)
			x += y * i;
	    return x;
	}

	int main()
	{
	    int i;
	    scanf("%d",&i);
	    printf("%d", f(i, i));

	    return 0;
	}

Which is equal to

	#include <stdio.h>

	f (x, int y)
	{
	    for (i = 1; i < 10; ++i)
	        x += y * i
	    finally
	        x
	}

	main
	{
	    scanf("%d", &i);
	    printf("%d", f i i);
	    0
	}

Or even

	f x y = x,
	        for i in range(1, 10)
	            x += y * i

	main = 0,
	       print (f i i), i = scan


Pattern matching is there as well, by the way.
Thanks for reading! 