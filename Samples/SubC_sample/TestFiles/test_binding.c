/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

int binding_target = 13;

int main()
{
	printf("SubC::main() : binding_target = %d\n", binding_target );

	binding_target = 1729;
}

int return_int_test()
{
	int a = 314;
	
	printf("SubC::return_int_test() : returning %d\n", a);

	return a;
}

void increment_test()
{
	++binding_target;

	printf("SubC::increment_test() : binding_target = %d\n", binding_target );
}

void add_test(int n)
{
	binding_target += n;

	printf("SubC::add_test(%d) : binding_target = %d\n", n, binding_target );
}

int sum_test(int a, int b)
{
	int c = a + b;

	printf("SubC::sum_test(%d, %d) : returning %d\n", a, b, c);

	return c;
}

float sum_test_float(float a, float b)
{
	float c = a + b;

	printf("SubC::sum_test_float(%f, %f) : returning %f\n", a, b, c);

	return c;
}

void test_printf(char const *s, int i, float f)
{
	printf("SubC::test_printf('%s', %d, %f)\n", s, i, f);
}