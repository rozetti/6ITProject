/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../H6VM.h"

#include <time.h>
#include <string.h>
#include <stdio.h>

int _H6VM_METHOD_NAME(first_chance_exception_handler)(struct exception_t *exception)
{
	if (exception->message[0] != 0)
	{
		_Bios.printf("machine exception handler caught exception %d (%s)\n", 
			exception->fault_code, exception->message);
	}
	else
	{
		_Bios.printf("machine exception handler caught exception %d\n", exception->fault_code);
	}

 	return 0;
}

// todo crz: point these at machine, not frame
void machine_debug_reset_clock(struct frame_t *frame, int n)
{
	if (!frame->machine->debug) return;

	frame->machine->debug->clocks[n] = clock();

	frame->machine->printf(frame->machine, "clock reset\n");
}

int machine_debug_read_clock(struct frame_t *frame, int n)
{
	if (!frame->machine->debug) return 0;

	clock_t elapsed = clock() - frame->machine->debug->clocks[n];

	return (int)(elapsed * 1000.0 / CLOCKS_PER_SEC);
}

int machine_debug_print_clock(struct frame_t *frame, int n)
{
	if (!frame->machine->debug) return 0;

	int milliseconds = machine_debug_read_clock(frame, n);
	
	frame->machine->printf(frame->machine, "clock %d elapsed time: %d seconds, %d milliseconds\n", 
		n, milliseconds / 1000, milliseconds % 1000);

	return milliseconds;
}

