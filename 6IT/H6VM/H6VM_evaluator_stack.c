/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../H6VM.h"

_6IT_PUBLIC _6IT_CONSTRUCTOR(value, evaluator_stack_value)
{
	_This->as_integer = 0;
	_This->type = TYPE_INVALID;
}
