/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

struct breakpoint_t
{
	opcode_t opcode;
};

struct debugger_t
{
	DEFINE_REQUIRED_ATTRIBUTES(debugger);

	void(*enter_processor_debugger)(struct debugger_t*, struct breakpoint_t *);
	void(*enter_evaluator_debugger)(struct debugger_t*);

	void(*print_state)(struct debugger_t*);
	void(*print_disassembly)(struct debugger_t*);
	void(*print_evaluator_state)(struct debugger_t*);
	void(*print_value_stack)(struct debugger_t*);
	void(*print_registers)(struct debugger_t*);
	void(*print_register)(struct debugger_t*, struct register_t*);
	void(*print_relative_registers)(struct debugger_t*);
	void(*print_absolute_registers)(struct debugger_t*);
	void(*print_minidump)(struct debugger_t*);
	void(*print_strings)(struct debugger_t*);

	void(*print_callable_unit_disassembly)(struct debugger_t*, struct callable_unit_t*);
	void(*print_current_instruction_disassembly)(struct debugger_t*);

	char const *(*format_execution_time)(struct debugger_t*);
};