/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

DOWNCOMPILER_METHOD_CONST(void, downcompile_instruction);

#include "6YM/6YM_fixup.c"

#include "6YM/6YM_downcompile_RETURNX.c"
#include "6YM/6YM_downcompile_IF.c"
#include "6YM/6YM_downcompile_EVAL.c"
#include "6YM/6YM_downcompile_FORX.c"
#include "6YM/6YM_downcompile_CONDITION.c"

#include "6YM/6YM_downcompile_smalls.c"
#include "6YM/6YM_downcompile_callable_unit.c"
#include "6YM/6YM_downcompiler.c"

_6IT_DESTRUCTOR(downcompiler) { }

_6IT_THISCALL_DIE(downcompiler)
{
	THROW((&_This->exception), fault_code);
}

_6IT_THISCALL_GET_ERROR_STRING(downcompiler)
{
	return 0;
}

_6IT_PUBLIC _6IT_CONSTRUCTOR(downcompiler, 6YM)
{
	INITIALISE_COMMON_ATTRIBUTES(downcompiler, _Machine);

	_This->machine = _Machine;

	_This->downcompile = downcompile;
}