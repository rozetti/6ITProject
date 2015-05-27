/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <stdio.h>

DEBUGGER_METHOD(char const *, format_execution_time)
{
	static char buffer[10];

	int ms;
	_Bios.get_current_time_ms(&ms);

	int d = ms - MACHINE_EFFECTIVE_EXECUTION_START_MS(_This->machine);

#ifdef _6IT_WIN32_SAFE_CRT
	sprintf_s(buffer, sizeof(buffer), "%.4d:%.4d", d / 1000, d % 1000);
#else
	sprintf(buffer, "%.4d:%.4d", d / 1000, d % 1000);
#endif

	return buffer;
}