/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include "6IOS/6IOS_input.c"
#include "6IOS/6IOS_print.c"
#include "6IOS/6IOS_time.c"

_6IT_PUBLIC _6IT_CONSTRUCTOR(bios, 6IOS)
{
	// todo crz: better singleton
	if (_This->initialised) return;
	_This->initialised = 1;

	_This->print_string = bios_print_string;
	_This->printf = bios_printf;

	_This->read_line = bios_read_line;

	_This->get_current_time_ms = get_current_time_ms;
}
