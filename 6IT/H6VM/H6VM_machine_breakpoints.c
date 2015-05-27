/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <memory.h>

MACHINE_METHOD(int, find_free_breakpoint)
{
	for (int i = 0; i < _This->debug->config.max_number_of_breakpoints; ++i)
	{
		struct breakpoint_t *bp = _This->debug->breakpoints + i;

		if (0 == bp->opcode)
		{
			return i;
		}
	}

	return 0;
}

MACHINE_METHODX(int, set_breakpoint, int pc)
{
	opcode_t *ip = _REG_CS(_REGS(_This)) + pc;
	opcode_t oc = *ip;

	int idx = _This->find_free_breakpoint(_This);
	struct breakpoint_t *bp = _This->debug->breakpoints + idx;
	bp->opcode = oc;

	*ip = MAKE_OPCODE(MACHINE_OPCODE_BREAK, idx);
}
