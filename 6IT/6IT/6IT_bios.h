/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include "6IT.h"

struct bios_t
{
	int initialised;

	void(*print_string)(char const *);
	void(*printf)(char const *format, ...);

	void(*read_line)(char *buffer, int max_length);

	void(*get_current_time_ms)(int *milliseconds);
};

extern struct bios_t _Bios;