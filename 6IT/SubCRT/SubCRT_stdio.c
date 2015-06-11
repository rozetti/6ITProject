/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../SubCRT.h"

#include "SubCRT_stdio_printf.c"

int c_sprintf(char *b, const char *format, struct value_t *stack, int top);

int lib_stdio_printf(struct machine_t *machine)
{
	int string_index = TOP_INT(machine);
	
	char const *format = _REG_SS(_REGS(machine)) + string_index;
	char b[100];
	
	c_sprintf(b, format, _REG_VIP(_REGS(machine)), -1);
	machine->printf(machine, b);

	return 1;
}

_6IT_PRIVATE void _6IT_MACHINECALL(add_library_stdio)
{
	int printf_parms[] = { TYPE_CHAR | TYPE_POINTER | TYPE_CONST, TYPE_VARARG, 0 };
	_Machine->add_builtin(_Machine, TYPE_INT, "printf", lib_stdio_printf, printf_parms);
}

