/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <stdio.h>

_6IT_PRIVATE void bios_read_line(char *buffer, int max_length)
{
	fgets(buffer, max_length, stdin);
}
