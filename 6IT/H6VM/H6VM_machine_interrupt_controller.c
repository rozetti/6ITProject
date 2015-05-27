/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#ifdef _6IT_SUPPORT_INTERRUPTS

static void request(struct interrupt_controller_t *This, int level)
{
	This->machine->interrupt_levels[level] = 1;

	_SET_IRQF(This->machine);
}


void construct_interrupt_controller(struct interrupt_controller_t *This, struct machine_t *machine)
{
	This->machine = machine;

	This->request = request;
}

#endif