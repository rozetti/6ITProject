/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6EV/6EV_opcodes.h"
#include "6IL/6IL_opcodes.h"

#include <assert.h>

DOWNCOMPILER_METHOD_CONST(void, downcompile_instruction)
{
	struct machine_t *machine = _This->machine;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;
	
	struct callable_unit_t *unit = _State->unit;

	add_fixup(_This, _State, _State->pc - _State->callable_unit_PC, _REG_FCP(_REGS(machine)) - unit->entry_point_program_counter);

	_State->p = _REG_CS(_REGS(machine)) + _State->pc;
	_SET_IP(_REGS(machine), _State->p);
	opcode_t oc = _State->p[0];

	int len = machine->processor.get_instruction_length(oc);
	assert(len != 0);
	_State->current_instruction_length = len;
	_State->next_opcode = _State->p[len];

	int succeeded = 0;

	switch (OPCODE(machine))
	{
	//case H6VM_OPCODE_IFS:
	//case H6VM_OPCODE_IFB:
	//	succeeded = _6YM_downcompile_IFX(state);
	//	break;
	//case H6VM_OPCODE_FORS:
	//case H6VM_OPCODE_FORB:
	//	succeeded = _6YM_downcompile_FORX(state);
	//	break;
	case H6VM_OPCODE_EVAL:
		succeeded = downcompile_EVAL(_This, _State);
		break;
	case H6VM_OPCODE_CONDITION:
		succeeded = downcompile_CONDITION(_This, _State);
		break;
		//case H6VM_OPCODE_RETURN:
	//	succeeded = _6YM_downcompile_RETURNEVAL(state);
	//	break;
	//case H6VM_OPCODE_LOADRSP:
	//	succeeded = downcompile_LOADRSP(state);
	//	break;
	}

	if (!succeeded)
	{
		for (int i = 0; i < len; ++i)
		{
			emitter->emit_integer(emitter, _REG_CS(_REGS(machine))[_State->pc++]);
		}
	}
}

_6IT_PUBLIC void _6IT_THISCALL(downcompiler, downcompile)
{
	struct machine_t *machine = _This->machine;

	if (_This->verbosity)
	{
		machine->printf(machine, "downcompiling...\n");
	}

	struct downcompiler_state_t state;

	for (int i = 0; i < _REG_FUP(_REGS(machine)); ++i)
	{
		struct callable_unit_t *unit = _REG_US(_REGS(machine)) + i;

		if (!unit->entry_point_ptr && !unit->is_lua)
		{
			state.unit = unit;
			state.p = _REG_CS(_REGS(machine)) + unit->entry_point_program_counter;
			state.unit_p = state.p;
			state.downcompiled_callable_unit_PC = _REG_FCP(_REGS(machine));
			state.delta = 0;

			struct callable_unit_metadata_t *metadata = 0;
			if (_This->verbosity)
			{
				metadata = machine->get_callable_unit_metadata(machine, unit->idx);
				machine->printf(machine, "6YM: downcompiling callable unit '%s'\n", metadata->symbol);
			}

			downcompile_callable_unit(_This, &state);

			if (_This->verbosity)
			{
				machine->printf(machine, "6YM: downcompiled callable unit '%s'\n", metadata->symbol);
			}
		}
	}

	if (_This->verbosity >= 2)
	{
#ifdef _6IT_DEBU66ER
		machine->printf(machine, "state after downcompiling:\n");
		machine->debugger.print_state(machine);
#endif
		machine->printf(machine, "...finished downcompiling\n");
	}
}
