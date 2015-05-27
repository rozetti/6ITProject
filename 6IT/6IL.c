/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include "6IL/6IL_opcodes.c"
#include "6IL/6IL_misc.c"

#ifdef _6IT_DEBU66ER
#define PROCESSOR_WITH_DEBUGGER
#include "6IL/6IL_processor.c"
#undef PROCESSOR_WITH_DEBUGGER
#endif

#include "6IL/6IL_processor.c"

_6IT_PRIVATE void _6IT_MACHINECALL(execute_debug_no_debugger)
{
	THROWM(&_Machine->exception, 0, "no debugger");
}

_6IT_DESTRUCTOR(processor) { }

_6IT_PUBLIC _6IT_CONSTRUCTOR(processor, 6IL)
{
	_This->destruct = processor_destruct;

	_This->execute = execute;
#ifdef _6IT_DEBU66ER
	_This->execute_debug = execute_debug;
#else
	_This->execute_debug = execute_debug_no_debugger;
#endif
	_This->execute_and_reset = execute_and_reset;
	_This->get_mnemonic = get_mnemonic;
	_This->get_instruction_length = get_instruction_length;
}

