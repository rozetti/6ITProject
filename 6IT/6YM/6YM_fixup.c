/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

DOWNCOMPILER_METHODX_CONST(int, find_new_pc, int old_pc)
{
	if (_This->verbosity)
	{
		_This->machine->printf(_This->machine, "fixup: looking for old pc: %d...", old_pc);
	}

	for (int i = 0; i < _State->number_of_fixups; ++i)
	{
		if (_State->fixups[i].old_pc == old_pc)
		{
			if (_This->verbosity)
			{
				_This->machine->printf(_This->machine, "found %d\n", _State->fixups[i].new_pc);
			}

			return _State->fixups[i].new_pc;
		}
	}

	if (_This->verbosity)
	{
		_This->machine->printf(_This->machine, "not found!\n");
	}

	return -1;
}

DOWNCOMPILER_METHODXX_CONST(void, add_fixup, int old_pc, int new_pc)
{
	if (_This->verbosity)
	{
		_This->machine->printf(_This->machine, "fixup: adding fixup %d --> %d\n", old_pc, new_pc);
	}

	_State->fixups[_State->number_of_fixups].old_pc = old_pc;
	_State->fixups[_State->number_of_fixups].new_pc = new_pc;
	++_State->number_of_fixups;
}

