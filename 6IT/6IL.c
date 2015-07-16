/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#ifdef _6IT_AMALGAMATE_SOURCE
#include "6IL/6IL_opcodes.c"
#include "6IL/6IL_misc.c"

#include "6IL/6IL_processor.c"

#ifdef _6IT_DEBU66ER
#define PROCESSOR_WITH_DEBUGGER
#include "6IL/6IL_processor.c"
#undef PROCESSOR_WITH_DEBUGGER
#endif

#endif

_6IT_PRIVATE void _6IT_MACHINECALL(_6IL_execute_debug_no_debugger)
{
	THROWM(&_Machine->exception, 0, "no debugger");
}

_6IT_DESTRUCTOR(processor, 6IL) { }

_6IT_PUBLIC _6IT_CONSTRUCTOR(processor, 6IL)
{
    _This->verbosity = 0;
	_This->destruct = _6IL_METHOD_NAME(destruct);

	_This->execute = _6IL_METHOD_NAME(execute);
#ifdef _6IT_DEBU66ER
	_This->execute_debug = _6IL_METHOD_NAME(execute_debug);
#else
	_This->execute_debug = _6IL_METHOD_NAME(execute_debug_no_debugger);
#endif
	_This->execute_and_reset = _6IL_METHOD_NAME(execute_and_reset);
	_This->get_mnemonic = _6IL_METHOD_NAME(get_mnemonic);
	_This->get_instruction_length = _6IL_METHOD_NAME(get_instruction_length);
}

