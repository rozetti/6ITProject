/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#ifdef _6IT_DEBU66ER

#ifdef _6IT_AMALGAMATE_SOURCE
#include "6UG/6UG_debugger.c"
#include "6UG/6UG_print.c"
#include "6UG/6UG_minidump.c"
#include "6UG/6UG_time.c"
#endif

_6IT_PUBLIC _6IT_CONSTRUCTOR(debugger, 6UG)
{
	INITIALISE_REQUIRED_ATTRIBUTES(debugger, _Machine);

	_This->enter_evaluator_debugger = _6UG_METHOD_NAME(enter_evaluator_debugger);
	_This->enter_processor_debugger = _6UG_METHOD_NAME(enter_processor_debugger);

	_This->print_absolute_registers = _6UG_METHOD_NAME(print_absolute_registers);
	_This->print_relative_registers = _6UG_METHOD_NAME(print_relative_registers);
	_This->print_callable_unit_disassembly = _6UG_METHOD_NAME(print_callable_unit_disassembly);
	_This->print_current_instruction_disassembly = _6UG_METHOD_NAME(print_current_instruction_disassembly);
	_This->print_evaluator_state = _6UG_METHOD_NAME(print_evaluator_state);
	_This->print_minidump = _6UG_METHOD_NAME(print_minidump);
	_This->print_registers = _6UG_METHOD_NAME(print_registers);
	_This->print_register = _6UG_METHOD_NAME(print_register);
	_This->print_state = _6UG_METHOD_NAME(print_state);
	_This->print_disassembly = _6UG_METHOD_NAME(print_disassembly);
	_This->print_strings = _6UG_METHOD_NAME(print_strings);
	_This->print_value_stack = _6UG_METHOD_NAME(print_value_stack);

	_This->format_execution_time = _6UG_METHOD_NAME(format_execution_time);
}

#endif
