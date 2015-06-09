/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6YM.h"
#include "6EV/6EV_opcodes.h"
#include "6IL/6IL_opcodes.h"

_6YM_METHOD_CONST(int, downcompile_CONDITION)
{
	struct machine_t *machine = _This->machine;

	struct expression_t expression;
	machine->build_expression(machine, &expression, OPCODE_PARM(machine));

	switch (OPCODE_INSTRUCTION(_State->next_opcode))
	{
	case H6VM_OPCODE_IFS:
	case H6VM_OPCODE_IFB:
		if (_6YM_METHOD_NAME(downcompile_CONDITION_IFX)(_This, _State, &expression)) return 1;
		break;
	}

	return 0;
}

