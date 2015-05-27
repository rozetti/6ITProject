/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

// todo crz: much much more of this
int machine_check_state(struct machine_t *machine)
{
	int vsp = _REG_VSP(_REGS(machine));
	int rsp = _REG_RSP(_REGS(machine));
	int esp = _REG_ESP(_REGS(machine));

	if (-1 > vsp)
	{
		machine->printf(machine, "Error: VSP is %d\n", vsp);
		return 0;
	}

	if (rsp < machine->config.max_number_of_globals)
	{
		machine->printf(machine, "Error: RSP is %d\n", rsp);
		return 0;
	}

	if (0 > esp)
	{
		machine->printf(machine, "Error: ESP is %d\n", esp);
		return 0;
	}

	return 1;
}
