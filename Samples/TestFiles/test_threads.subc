/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

// crz: the 6IT threading is highly expermental and does not work properly yet!

#include <assert.h>

int main()
{
	printf("TESTING: Threads\n");

	int i = 0;
	int b = 3;

	for (i = 0; i <= 10000000; ++i)
	{
		b += (b * b) % 1000;

		if (i % 100000 == 0) printf("main thread iteration %d: b = %d\n", i, b);
	}

	return 42;
}

void worker1_inner(int tt)
{
	int u;
	int a = 0;
	for (u = 0; u < 1000000; ++u)
	{
		a += tt * u;
	}

	printf("worker thread 1 iteration %d\n", tt);
}

int worker1()
{
	int t;

	for (t = 0; t < 10000000; ++t)
	{
		worker1_inner(t);
	}

	return 6;
}

int worker2()
{
	int t2 = 0;
	int a2 = 10000;
	int i2 = 0;
	int b2 = 0;

	for (i2 = 0; i2 < 1000000; ++i2)
	{
		for (t2 = 0; t2 < a2; ++t2)
		{
			b2++;
		}

		printf("worker thread 2 iteration %d\n", t2);
		a2 *= 2;
	}

	return 1;
}