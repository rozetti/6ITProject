/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

static void new_program(struct machine_t *machine)
{
	_REG_FCP(_REGS(machine)) = 0;
	_REG_FSP(_REGS(machine)) = 0;
	_REG_FUP(_REGS(machine)) = 0;
	_REG_FXP(_REGS(machine)) = 0;
	_REG_FRP(_REGS(machine)) = 0;

#ifdef _6IT_SUPPORT_LUA
	//machine_setup_lua(machine);
	//machine_pull_from_lua(machine);

	//machine->print_state(machine);
#endif
}

