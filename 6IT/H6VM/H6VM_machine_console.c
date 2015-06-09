/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "H6VM.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static void _H6VM_printf(struct machine_t *machine, char const *format, ...)
{
	static char b[2000];

	va_list args;
	va_start(args, format);
	_6IT_VSPRINTF_STATIC_BUFFER(b, format, args);
	va_end(args);

	if (machine->environment)
	{
		machine->environment->output_string(b);
	}
	else
	{
		printf("%s", b);
	}
}

_H6VM_METHODXX(void, read_line, char *s, int max_length)
{
	_This->environment->input_line(s, max_length);
}