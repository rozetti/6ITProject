/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

#define _IS_COMMAND(C, S) (!_strcmpi(command, #C) || !_strcmpi(command, #S))
#define _IS_PARM(C, S) (!_strcmpi(parms[0], #C) || !_strcmpi(parms[0], #S))
#define _HAS_PARM  (number_of_parms != 0)

DEBUGGER_METHOD(void, state_changed)
{
	struct machine_t *machine = _This->machine;

	if (!memcmp(&_THREAD(machine)->registers, &_THREAD(machine)->saved_reg, sizeof(struct machine_registers_t))) return;

	_This->print_minidump(_This);

	_THREAD(machine)->saved_reg = _THREAD(machine)->registers;
}

_6IT_PRIVATE void help(struct machine_t *machine)
{
	machine->printf(machine, "commands: (s)tep, (b)reak, (l)ist, state, ev, val, reg, (c)ontinue, (r)un\n");
}

DEBUGGER_METHODX(void, enter_processor_debugger, struct breakpoint_t *bp)
{
	struct machine_t *machine = _This->machine;

	int entry_ms;
	_Bios.get_current_time_ms(&entry_ms);

	machine->printf(machine, "entering debugger at %s\n\t%s\n", 
		_This->format_execution_time(_This),
		machine->disassemble_instruction(machine, _REG_PC(_REGS(machine)), -1));

	_This->print_minidump(_This);

	char temp[100];
	char *parms[10];
	char buffer[100];

	while (1)
	{
		help(machine);
		machine->printf(machine, "> ");
		machine->environment->input_line(buffer, sizeof(buffer));

		strcpy(temp, buffer);
		char *command = strtok(temp, " \r\n");
		if (!command)
		{
			continue;
		}

		int number_of_parms = 0;
		while (parms[number_of_parms] = strtok(0, " \r\n"))
		{
			++number_of_parms;
		}

		if (_IS_COMMAND(help, h))
		{
			help(machine);
		}
		else if (_IS_COMMAND(list, l))
		{
			_This->print_disassembly(_This);
		}
		else if (_IS_COMMAND(info, i))
		{
			if (_HAS_PARM)
			{
				if (_IS_PARM(break, b))
				{
					machine->printf(machine, "list breakpoints\n");
				}
				else
				{
					machine->printf(machine, "bad info parm\n");
				}
			}
			else
			{
				machine->printf(machine, "info need a parm\n");
			}
		}
		else if (_IS_COMMAND(step, s))
		{
			machine->interrupt_controller.request(&machine->interrupt_controller, INTERRUPT_SINGLE_STEP);
			break;
		}
		else if (_IS_COMMAND(break, b))
		{
			if (_HAS_PARM)
			{
				machine->printf(machine, "breakpoint: %s\n", parms[0]);
				int bp = atoi(parms[0]);
				machine->set_breakpoint(machine, bp);
			}
			else
			{
				machine->printf(machine, "break need a parm\n");
			}
		}
		else if (!_strcmpi(command, "state"))
		{
			_This->print_state(_This);
		}
		else if (!_strcmpi(command, "ev"))
		{
			_This->print_evaluator_state(_This);
		}
		else if (!_strcmpi(command, "val"))
		{
			_This->print_value_stack(_This);
		}
		else if (!_strcmpi(command, "reg"))
		{
			_This->print_registers(_This);
		}
		else if (!_strcmpi(command, "continue") || !_strcmpi(command, "c"))
		{
			break;
		}
		// todo crz: this
		//else if (!_strcmpi(command, "run") || !_strcmpi(command, "r"))
		//{
		//	REG_PC(machine) = 0;
		//	_REG_RSP(_REGS(machine)) = machine->config.max_number_of_globals;

		//	machine->single_step_flag = 0;
		//	return;
		//}
		else
		{
			machine->printf(machine, "unknown command\n");
		}
	}

	int exit_ms;
	_Bios.get_current_time_ms(&exit_ms);

	machine->pause_ms += exit_ms - entry_ms;
}

// todo crz: this
DEBUGGER_METHOD(void, enter_evaluator_debugger)
{
	//if (!machine->single_step_flag) return;

	//machine_print_evaluator_state(machine);
}

