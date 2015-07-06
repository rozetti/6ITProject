/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../H6VM.h"

_H6VM_METHODXX(void, create_thread, struct thread_t *thread, struct callable_unit_t *entry)
{
	thread->registers = _This->registers;

	construct_H6VM_THREAD(thread, _This, &_This->config);
	_This->threads[_This->number_of_threads] = thread;
	thread->id = _This->number_of_threads++;
	thread->saved_reg = thread->registers;

	thread->registers.frame->frame_registers = _REG_RS(&thread->registers) + _REG_RSP(&thread->registers);

	if (entry) // hack crz
	{
		thread->registers.frame->entry_ip = _REG_CS(&thread->registers) + entry->entry_point_program_counter;
		thread->registers.frame->callable_unit_idx = entry->idx;
		_SET_PC(&thread->registers, entry->entry_point_program_counter);
	}
}

_H6VM_METHODX(void, switch_context, int id)
{
    // hack crz
    if (_This->current_thread_idx > 100)
    {
        _This->current_thread_idx = -1;
    }

	if (id == _This->current_thread_idx)
	{
		return;
	}

	if (-1 != _This->current_thread_idx)
	{
		_This->threads[_This->current_thread_idx]->registers = _This->registers;
	}

	_This->current_thread_idx = id;
	_This->registers = _This->threads[_This->current_thread_idx]->registers;
}
