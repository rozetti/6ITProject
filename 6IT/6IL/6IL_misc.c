/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6IL.h"

_6IT_PRIVATE void _6IT_MACHINECALL(_6IL_execute_and_reset)
{
	// todo crz: integrate this with checked build
#ifdef _6IT_CHECKED_BUILD
	int rsp = _REG_RSP(_REGS(_Machine));
#endif
	long pc = _REG_PC(_REGS(_Machine));

	_Machine->execute(_Machine);

	_6IT_ASSERT(_REG_RSP(_REGS(_Machine)) == rsp);
	_SET_PC(_REGS(_Machine), pc);
}



