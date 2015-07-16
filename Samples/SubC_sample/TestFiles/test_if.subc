/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

void test_if_statement()
{
	printf("TESTING: if with single statement true case\n");

	int a = 0;
	int b = 1;
	if (a == b)	a = b;
	assert(a != b);

	a = 0;
	b = 1;
	if (a != b)	a = b;
	assert(a == b);
}

void test_if_block()
{
	printf("TESTING: if with block true case\n");

	int a = 0;
	int b = 1;
	if (a == b)	{ a = b; }
	assert(a != b);

	a = 0;
	b = 1;
	if (a != b)	{ a = b; }
	assert(a == b);
}

void test_if_else()
{
	// todo crz
}

int main()
{
	test_if_statement();
	test_if_block();

	return 1729;
}
