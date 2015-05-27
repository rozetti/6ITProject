/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

SCHEDULER_METHOD_CONST(void, next)
{
	if (1 == _This->machine->number_of_threads) return;

	int id = _This->machine->current_thread_idx + 1;
	if (id == _This->machine->number_of_threads)
	{
		id = 0;
	}

	_This->machine->switch_context(_This->machine, id);
}

_6IT_PUBLIC _6IT_CONSTRUCTOR(scheduler, scheduler)
{
	_This->machine = _Machine;

	_This->next = next;
}
