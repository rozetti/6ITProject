/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <string.h>

// todo crz: bounds checking

MACHINE_METHODXX(void, add_string, char const *string, int *index)
{
	int idx = _REG_FSP(_REGS(_This));

	int len = strlen(string);

	strcpy(_REG_SS(_REGS(_This)) + idx, string);

	_REG_FSP(_REGS(_This)) += len + 1;

	*index = idx;
}

MACHINE_METHODX(char const *, get_string, int index)
{
	return _REG_SS(_REGS(_This)) + index;
}