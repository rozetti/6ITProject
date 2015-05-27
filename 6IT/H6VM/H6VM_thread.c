/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <stdlib.h>
#include <memory.h>
#include <string.h>

_6IT_DESTRUCTOR(thread) 
{ 
	if (_REG_VS(&_This->registers))
	{
		free(_REG_VS(&_This->registers));
		_REG_VS(&_This->registers) = 0;
	}

	if (_REG_ES(&_This->registers))
	{
		free(_REG_ES(&_This->registers));
		_REG_ES(&_This->registers) = 0;
	}

	if (_REG_RS(&_This->registers))
	{
		free(_REG_RS(&_This->registers));
		_REG_RS(&_This->registers) = 0;
	}
}

_6IT_THISCALL_DIE(thread)
{
	THROW(&_This->exception, fault_code);
}

_6IT_THISCALL_GET_ERROR_STRING(thread)
{
	return 0;
}

static int construct_stack(struct thread_t *thread, int max_depth)
{
	int size = sizeof(struct frame_t) * max_depth;
	if (!(_REG_ES(&thread->registers) = (struct frame_t *)malloc(size)))
	{
		return 0;
	}

	for (int i = 0; i < max_depth; ++i)
	{
		construct_stack_frame(_REG_ES(&thread->registers) + i, 0, i);
	}

	_SET_ESP(&thread->registers, 0);

	return 1;
}

static int construct_evaluator_stack(struct thread_t *thread, int max_depth)
{
	int size = sizeof(struct value_t) * max_depth;
	if (!(_REG_VS(&thread->registers) = (struct value_t *)malloc(size)))
	{
		return 0;
	}

	for (int i = 0; i < max_depth; ++i)
	{
		construct_evaluator_stack_value(_REG_VS(&thread->registers) + i, thread->machine);
	}

	_SET_VSP(&thread->registers, -1);

	return 1;
}

static int construct_register_stack(struct thread_t *thread, int max_depth)
{
	int size = sizeof(struct register_t) * max_depth;
	if ((_REG_RS(&thread->registers) = (struct register_t *)malloc(size)))
	{
		memset(_REG_RS(&thread->registers), 0, size);
		_REG_FRP(&thread->registers) = 0;

		for (int i = 0; i < max_depth; ++i)
		{
			_REG_RS(&thread->registers)[i].idx = i;
		}

		return 1;
	}

	return 0;
}

_6IT_PUBLIC _6IT_CONSTRUCTORX(thread, thread, struct machine_config_t *config)
{
	INITIALISE_COMMON_ATTRIBUTES(thread, _Machine);

	// todo crz: handle alloc errors
	construct_stack(_This, config->max_number_of_frames);
	construct_evaluator_stack(_This, config->evaluator_stack_depth);
	construct_register_stack(_This, config->register_stack_depth);
}
