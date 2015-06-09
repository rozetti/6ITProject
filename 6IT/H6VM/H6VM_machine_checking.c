/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

// todo crz: much much more of this
_H6VM_METHOD(int, check_state)
{
	int vsp = _REG_VSP(_REGS(_This));
	int rsp = _REG_RSP(_REGS(_This));
	int esp = _REG_ESP(_REGS(_This));

	if (-1 > vsp)
	{
		_This->printf(_This, "Error: VSP is %d\n", vsp);
		return 0;
	}

	if (rsp < _This->config.max_number_of_globals)
	{
		_This->printf(_This, "Error: RSP is %d\n", rsp);
		return 0;
	}

	if (0 > esp)
	{
		_This->printf(_This, "Error: ESP is %d\n", esp);
		return 0;
	}

	return 1;
}
