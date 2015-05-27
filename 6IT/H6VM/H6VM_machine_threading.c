/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

void machine_create_thread(struct machine_t *machine, struct thread_t *thread, struct callable_unit_t *entry)
{
	thread->registers = machine->registers;

	construct_thread(thread, machine, &machine->config);
	machine->threads[machine->number_of_threads] = thread;
	thread->id = machine->number_of_threads++;
	thread->saved_reg = thread->registers;

	thread->registers.frame->frame_registers = _REG_RS(&thread->registers) + _REG_RSP(&thread->registers);

	if (entry) // hack crz
	{
		thread->registers.frame->entry_ip = _REG_CS(&thread->registers) + entry->entry_point_program_counter;
		thread->registers.frame->callable_unit_idx = entry->idx;
		_SET_PC(&thread->registers, entry->entry_point_program_counter);
	}
}

void machine_switch_context(struct machine_t *machine, int id)
{
	if (id == machine->current_thread_idx)
	{
		return;
	}

	if (-1 != machine->current_thread_idx)
	{
		machine->threads[machine->current_thread_idx]->registers = machine->registers;
	}

	machine->current_thread_idx = id;
	machine->registers = machine->threads[machine->current_thread_idx]->registers;
}