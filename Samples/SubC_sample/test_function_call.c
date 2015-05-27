/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

int f2(int b)
{
	printf("f2()\n");

	return 213 + b;
}

void f1()
{
	printf("f1() in\n");

	int a = f2(7);

	printf("%d, f1() out\n", a);
}

int main()
{
	printf("main() in\n");

	f1();

	printf("main() out\n");

	return 42;
}
