/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6YM.h"
#include "6EV/6EV_opcodes.h"
#include "6IL/6IL_opcodes.h"

#include <stdio.h>

_6YM_METHOD_CONST(void, downcompile_callable_unit)
{
	struct machine_t *machine = _This->machine;
	struct callable_unit_t *unit = _State->unit;

	_State->callable_unit_PC = unit->entry_point_program_counter;

	_State->number_of_fixups = 0;

	int pc = _State->unit->entry_point_program_counter;
	unit->entry_point_program_counter = _REG_FCP(_REGS(machine));

	_State->pc = pc;
	while (_State->pc <= _State->unit->exit_point_program_counter)
	{
		_6YM_METHOD_NAME(downcompile_instruction)(_This, _State);
	}

	unit->exit_point_program_counter = _REG_FCP(_REGS(machine)) - 1;

#ifdef _6IT_DEBU66ER
	if (_This->verbosity)
	{
		machine->printf(machine, "downcompiled callable unit:\n");
		machine->debugger.print_callable_unit_disassembly(&_This->machine->debugger, unit);
	}
#endif

	pc = _State->unit->entry_point_program_counter;
	while (pc <= _State->unit->exit_point_program_counter)
	{
		_SET_PC(_REGS(machine), pc);
		opcode_t *_ip = _REG_IP(_REGS(machine));
		int oc = OPCODE(machine);

		struct mnemonic_t const *mn = machine->processor.get_mnemonic(oc);

		if (mn->parm_type == H6VM_FPC)
		{
			int new_pc = _6YM_METHOD_NAME(find_new_pc)(_This, _State, OPCODE_PARM(machine));
			if (-1 == new_pc)
			{
				sprintf(machine->exception.message, "fixup not found for old PC '%d'", OPCODE_PARM(machine));
				THROW(&machine->exception, 1);
			}

			SET_OPCODE_PARM(oc, new_pc);
			*_ip = oc;
		}

		for (int i = 0; i < mn->number_of_operands; ++i)
		{
			if (mn->operands[i] == H6VM_FPC)
			{
				int old_pc = _ip[i + 1];
				int new_pc = _6YM_METHOD_NAME(find_new_pc)(_This, _State, old_pc);
				if (-1 == new_pc)
				{
					sprintf(machine->exception.message, "bad fixup from address '%d'", old_pc);
					THROW(&machine->exception, 1);
				}

				_ip[i + 1] = new_pc;
			}

			if (mn->operands[i] == H6VM_INT16_FPC16)
			{
				int old_pc = OPERAND_16L(_ip[i + 1]);
				int old_int = OPERAND_16H(_ip[i + 1]);

				int new_pc = _6YM_METHOD_NAME(find_new_pc)(_This, _State, old_pc);
				if (-1 == new_pc)
				{
					sprintf(machine->exception.message, "bad fixup from address '%d'", old_pc);
					THROW(&machine->exception, 1);
				}

				_ip[i + 1] = MAKE_OPERAND(old_int, new_pc);
			}
		}

		pc += machine->processor.get_instruction_length(oc);
	}
}

