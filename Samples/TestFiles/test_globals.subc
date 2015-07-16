/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

int g = 42;
float h = 1729.0;

void test_globals()
{
	assert(g == 42);
	assert(h == 1729.0);

	h = (float)g;

	assert(h == 42.0);
}

int c = 4;

void test_later_global()
{
	assert(4 == c);
}

float d = 5.0;

float get_latest_global()
{
	return d;
}

int main()
{
	printf("TESTING: globals\n");

	test_globals();
	test_later_global();
	assert(get_latest_global() == 5.0);

	return 1729;
}
