/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6YM.h"

_6YM_METHOD_CONST(void, downcompile_instruction);

#ifdef _6IT_AMALGAMATE_SOURCE
#include "6YM/6YM_fixup.c"

#include "6YM/6YM_downcompile_RETURNX.c"
#include "6YM/6YM_downcompile_IF.c"
#include "6YM/6YM_downcompile_EVAL.c"
#include "6YM/6YM_downcompile_FORX.c"
#include "6YM/6YM_downcompile_CONDITION.c"

#include "6YM/6YM_downcompile_smalls.c"
#include "6YM/6YM_downcompile_callable_unit.c"
#include "6YM/6YM_downcompiler.c"
#endif

_6IT_DESTRUCTOR(downcompiler, 6YM) { }

_6IT_THISCALL_DIE(downcompiler, 6YM)
{
	THROW((&_This->exception), fault_code);
}

_6IT_THISCALL_GET_ERROR_STRING(downcompiler, 6YM)
{
	return 0;
}

_6IT_PUBLIC _6IT_CONSTRUCTOR(downcompiler, 6YM)
{
	INITIALISE_COMMON_ATTRIBUTES(downcompiler, 6YM, _Machine);

	_This->machine = _Machine;

	_This->downcompile = _6YM_downcompile;
}