/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

int fib(int n)
{
	if (n == 0)	return 0;
	if (n == 1)	return 1;

	return fib(n - 1) + fib(n - 2);
}

int main()
{
	printf("TESTING: Fibonacci in SubC\n");

	int i = 0;

	for (i = 0; i <= 38; ++i)
	{
		printf("F%d = %d\n", i, fib(i));
	}

	return 42;
}