/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include <assert.h>

int return_parm(int a)
{
	return a;
}

int return_square_of_parm(int b)
{
	return b * b;
}

void test_operators()
{
	int a = 0;

	// assignment
	a = 1;
	assert(a == 1);
	// addition
	assert(1 + 1 == 2);
	assert(2 == 1 + 1);

	// subtraction
	assert(1 == 2 - 1);
	assert(2 - 1 == 1);

	// unary plus
	assert(+1 == 1);

	// unary minus
	assert(-1 == -1);

	// multiplication
	assert(1 * 1 == 1);
	assert(1 == 1 * 1);
	assert(2 * 2 == 4);
	assert(4 == 2 * 2);

	assert(2 * 2 == 2 * 2);
	assert(2 * 2 == 1 * 4);

	assert(1 * 1 == 1);
	assert(-1 * -1 == 1);
	assert(-1 * 1 == -1);
	assert(1 * -1 == -1);

	// division
	assert(4 / 2 == 2);
	assert(2 == 4 / 2);

	// modulo
	assert(4 % 3 == 1);
	assert(1 == 4 % 3);

	// pre-increment
	a = 1;
	assert(++a == 2);

	// post-increment
	a = 1;
	assert(a++ == 1);
	assert(a == 2);

	// pre-decrement
	a = 2;
	assert(--a == 1);

	// post-decrement
	a = 1;
	assert(a-- == 1);
	assert(a == 0);

	// equality
	assert(1 == 1);

	// inequality
	assert(1 != 0);
	assert(0 != 1);

	// greater
	assert(1 > 0);

	// less
	assert(0 < 1);

	// greater or equal
	assert(1 >= 0);
	assert(1 >= 1);

	// less or equal
	assert(0 <= 1);
	assert(1 <= 1);

	// logical not
	a = 0;
	//assert(!a);

	// logical and
	//assert(1 && 1);

	// logical or
	//assert(1 || 0);

	// bitwise not
	// bitwise and
	// bitwise or
	// bitwise xor
	// bitwise left shift
	// bitwise right shift

	// addition assignment
	a = 1;
	a += 2;
	assert(a == 3);

	// subtraction assignment
	a = 3;
	a -= 2;
	assert(a == 1);

	// multiplication assignment
	a = 2;
	a *= 3;
	assert(a == 6);

	// division assignment
	a = 6;
	a /= 3;
	assert(a == 2);

	// modulo assignment
	a = 4;
	a %= 3;
	assert(a == 1);

	// bitwise and assignment
	// bitwise or assignment
	// bitwise xor assignment
	// bitwise left shift assignment
	// bitwise right shift assignment

	// precedence

	assert(1 + 2 * 3 == 7);
	assert(3 * 2 + 1 == 7);

	// brackets

	assert((1 + 2) * 3 == 9);
	assert(3 * (2 + 1) == 9);

	assert((2 == 1) == 0);
	assert((2 == 2) == 1);
	assert(0 == (1 == 0));
	assert(1 == (2 == 2));

	// other

	assert((1 + 2) * 3 == 3 * 3);
	assert(3 * (2 + 1) == (3 * 3));

	assert(1 + (1 + 1) == 3);
	assert((1 + 1) + 1 == 3);

	printf("ret %d\n", return_parm(-1));
	printf("sq %d\n", return_square_of_parm(-1));

	//assert(return_parm(1) == 1);
	//assert(return_parm(0) == 0);
	//assert(return_parm(-1) == -1);

	//assert(return_square_of_parm(1) == 1);
	//assert(return_square_of_parm(-1) == 1);
	//assert(return_square_of_parm(0) == 0);
	//assert(return_square_of_parm(2) == 4);
}

int main()
{
	test_operators();

	return 42;
}
