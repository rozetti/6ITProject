/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

DEBUGGER_METHOD(void, print_minidump)
{
	struct machine_t *machine = _This->machine;

	machine->printf(machine, "PC:  %.3d  ", _REG_PC(_REGS(machine)));
	machine->printf(machine, "ESP: %.3d  ", _REG_ESP(_REGS(machine)));
	machine->printf(machine, "RSP: %.3d  ", _REG_RSP(_REGS(machine)));
	machine->printf(machine, "VSP: %.3d  ", _REG_VSP(_REGS(machine)));
	machine->printf(machine, "\n", _REG_VSP(_REGS(machine)));

	struct frame_t *frame = _FRAME(machine);
	if (frame)
	{
		int cu = frame->callable_unit_idx;

		machine->printf(machine, "frame: %d, callable unit: %d:%s, bp: %d, locals: %i\n",
			frame->position, cu, machine->get_callable_unit_symbol(machine, cu),
			FRAME_BASE_REGISTER_INDEX(machine, frame),
			0); // todo crz
	}
	else
	{
		machine->printf(machine, "no frame\n");
	}
}
