/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#ifdef _6IT_DEBU66ER

#include "6UG/6UG_debugger.c"
#include "6UG/6UG_print.c"
#include "6UG/6UG_minidump.c"
#include "6UG/6UG_time.c"

_6IT_PUBLIC _6IT_CONSTRUCTOR(debugger, 6UG)
{
	INITIALISE_REQUIRED_ATTRIBUTES(debugger, _Machine);

	_This->enter_evaluator_debugger = enter_evaluator_debugger;
	_This->enter_processor_debugger = enter_processor_debugger;

	_This->print_absolute_registers = print_absolute_registers;
	_This->print_relative_registers = print_relative_registers;
	_This->print_callable_unit_disassembly = print_callable_unit_disassembly;
	_This->print_current_instruction_disassembly = print_current_instruction_disassembly;
	_This->print_evaluator_state = print_evaluator_state;
	_This->print_minidump = print_minidump;
	_This->print_registers = print_registers;
	_This->print_register = print_register;
	_This->print_state = print_state;
	_This->print_disassembly = print_disassembly;
	_This->print_strings = print_strings;
	_This->print_value_stack = print_value_stack;

	_This->format_execution_time = format_execution_time;
}

#endif
