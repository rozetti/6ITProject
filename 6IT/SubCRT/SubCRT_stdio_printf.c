/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

// todo crz: get rid of this

#include <stdarg.h>

#include "6IT.h"

static void printchar(char **str, int c)
{
	**str = c;
	++(*str);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for (; width > 0; --width) {
			printchar(out, padchar);
			++pc;
		}
	}
	for (; *string; ++string) {
		printchar(out, *string);
		++pc;
	}
	for (; width > 0; --width) {
		printchar(out, padchar);
		++pc;
	}

	return pc;
}

#define PRINT_BUF_LEN 30

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN - 1;
	*s = '\0';

	while (u) {
		t = u % b;
		if (t >= 10)
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if (width && (pad & PAD_ZERO)) {
			printchar(out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints(out, s, width, pad);
}

static int print_f(char **out, float f, int base, int sg, int width, int pad, int letbase)
{
	int pc = 0;

	int i = (int)f;
	int fractional_part = (int)((f - i) * 100000);
	
	pc = printi(out, i, base, sg, width, pad, letbase);
	pc += prints(out, ".", 1, 0);
	pc += printi(out, fractional_part, base, sg, 5, PAD_ZERO, letbase);

	return pc;
}

int c_sprintf(char *b, const char *format, struct value_t *stack, int top)
{
	register int width, pad;
	register int pc = 0;
	char scr[2];

	char **out = &b;

	for (; *format != 0; ++format) 
	{
		if (*format == '%') 
		{
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			
			if (*format == '-') 
			{
				++format;
				pad = PAD_RIGHT;
			}
			
			while (*format == '0') 
			{
				++format;
				pad |= PAD_ZERO;
			}
			
			for (; *format >= '0' && *format <= '9'; ++format) 
			{
				width *= 10;
				width += *format - '0';
			}
			
			if (*format == 's') 
			{
				char const *s = stack[top--].as_char_const_ptr;
				pc += prints(out, s ? s : "(null)", width, pad);
				continue;
			}
			
			if (*format == 'f')
			{
				float f = stack[top--].as_float;

				pc += print_f(out, f, 10, 1, width, pad, 'a');
				continue;
			}

			if (*format == 'd')
			{
				int i = stack[top--].as_integer;
				
				pc += printi(out, i, 10, 1, width, pad, 'a');
				continue;
			}
			
			if (*format == 'x') 
			{
				int i = stack[top--].as_integer;
				pc += printi(out, i, 16, 0, width, pad, 'a');
				continue;
			}
			
			if (*format == 'X') 
			{
				int i = stack[top--].as_integer;
				pc += printi(out, i, 16, 0, width, pad, 'A');
				continue;
			}
			
			if (*format == 'u') 
			{
				int i = stack[top--].as_integer;
				pc += printi(out, i, 10, 0, width, pad, 'a');
				continue;
			}
			
			if (*format == 'c') 
			{
				int i = stack[top--].as_integer;
				scr[0] = (char)i;
				scr[1] = '\0';
				pc += prints(out, scr, width, pad);
				continue;
			}
		}
		else if (*format == '\\')
		{
			++format;
			if (*format == 'n')
			{
				printchar(out, '\n');
				++pc;
			}
		}
		else
		{
		out:
			printchar(out, *format);
			++pc;
		}
	}

	**out = 0;

	return pc;
}
