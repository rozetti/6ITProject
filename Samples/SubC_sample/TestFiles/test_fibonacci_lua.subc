/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

extern float lua_global;

float lua_fib(float f);

void test_lua()
{
	printf("TESTING: Fibonacci in Lua\n");

	printf("lua_global = %f\n", lua_global);

	int i = 0;
	for (i = 0; i <= 38; ++i)
	{
		printf("lua_fib(%d) = %f\n", i, lua_fib((float)i));
	}
}

int main()
{
	test_lua();

	return 42;
}