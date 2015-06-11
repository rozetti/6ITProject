/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

float ff(float p1, int p2)
{
	printf("ff() says: f1: %f, f2:%d, (float)f2: %f\n", p1, p2, (float)2);

	return p1 + (float)p2;
}

void test_float()
{
	int i = 0;
	float f1 = 3.14;
	float f2 = 2.1;

	printf("calling ff(%f, %f)\n", f1, f2);
	float fff = ff(f1, i);
	printf("fff: %f\n", fff);

	float f3 = 0.0;

	f3 = f1 + f2;
	printf("%f + %f = %f\n", f1, f2, f3);
	f3 = f1 - f2;
	printf("%f - %f = %f\n", f1, f2, f3);
	f3 = f1 * f2;
	printf("%f * %f = %f\n", f1, f2, f3);
	f3 = f1 / f2;
	printf("%f / %f = %f\n", f1, f2, f3);

	i = f1 == f2;
	printf("%f == %f = %d\n", f1, f2, i);
	i = f1 != f2;
	printf("%f != %f = %d\n", f1, f2, i);
	i = f1 < f2;
	printf("%f < %f = %d\n", f1, f2, i);
	i = f1 <= f2;
	printf("%f <= %f = %d\n", f1, f2, i);
	i = f1 > f2;
	printf("%f > %f = %d\n", f1, f2, i);
	i = f1 >= f2;
	printf("%f >= %f = %d\n", f1, f2, i);

	printf("(int)%f = %d\n", f1, (int)f1);
	printf("(int)%f = %d\n", f2, (int)f2);

	printf("(float)%d = %f\n", 1, (float)1);
	printf("(float)%d = %f\n", 3, (float)3);

	printf("(float)3 > 2.0 = %d\n", (float)3 > 2.9);
	printf("(int)3.1 > 3 = %d\n", (int)3.1 > 3);

	printf("calling ff(%f, %f)\n", f1, f2);

	for (f1 = 0.0; f1 < 10.0; f1 += 1.23456789)
	{
		printf("%d: %f\n", (int)f1, f1);
	}
}

int main()
{
	test_float();

	return 1729;
}
