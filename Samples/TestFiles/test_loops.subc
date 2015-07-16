/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

void test_while_block()
{
	printf("TESTING: while loop with block\n");

	int i = 0;

	while (i < 10)
	{
		printf("%d, ", i);
		++i;
	}

	printf("after loop: %d\n", i);
	assert(10 == i);
}

void test_while_statement()
{
	printf("TESTING: while loop with single statement\n");

	int i = 0;
	while (i < 10) printf("%d, ", i++);
	printf("after loop: %d\n", i);
	assert(10 == i);

	int i = 0;
	while (i++ < 10) printf("%d, ", i);
	printf("after loop: %d\n", i);
	assert(11 == i);
}

int main()
{
	test_while_block();
	test_while_statement();

	return 1729;
}
