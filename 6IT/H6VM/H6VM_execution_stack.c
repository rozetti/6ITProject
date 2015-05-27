/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

_6IT_PUBLIC _6IT_CONSTRUCTORX(frame, stack_frame, int position)
{
	memset(_This, 0, sizeof(struct frame_t));

	_This->machine = _Machine;
	_This->position = position;
	_This->return_value.as_integer = 0;
	_This->callable_unit_idx = 0;

	_This->evaluator_program_counter = -1;
}

