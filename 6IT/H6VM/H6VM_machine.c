/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../H6VM.h"

#include "H6VM_machine_strings.c"
#include "H6VM_machine_disassembly.c"
#include "H6VM_machine_breakpoints.c"
#include "H6VM_machine_bindings.c"
#include "H6VM_machine_program.c"
#include "H6VM_machine_console.c"
#include "H6VM_machine_callable_unit.c"
#include "H6VM_machine_expression.c"
#include "H6VM_machine_lua.c"
#include "H6VM_machine_interrupt_controller.c"
#include "H6VM_machine_register.c"
#include "H6VM_machine_threading.c"
#include "H6VM_machine_debugger_support.c"
#include "H6VM_machine_checking.c"

#include <stdlib.h>
#include <memory.h>
#include <string.h>

struct value_t _int_value;

_6IT_THISCALL_DIE(machine, H6VM)
{
#ifdef _6IT_DEBU66ER
	_This->debugger.print_state(&_This->debugger);
#endif

	_This->printf(_This, "\n\nvirtual machine fault: %s\n", message);

	THROW(&_This->exception, fault_code);
}

void machine_die_immediately(struct machine_t *machine, int fault_code, char const *message)
{
	machine->printf(machine, "\n\nvirtual machine unrecoverable fault: %s\n", message);

	THROW(&machine->exception, fault_code);
}

_6IT_THISCALL_GET_ERROR_STRING(machine, H6VM)
{
	switch (error)
	{
	case ERROR_H6VM_UNKNOWN_TYPE_ON_VALUE_STACK:
		return "Unknown type on value stack";
	}

	return 0;
}

void construct_default_H6VM_config(struct machine_config_t *config)
{
	config->code_segment_size = 10000;
	config->data_segment_size = 5000;
	config->string_segment_size = 5000;
	config->evaluator_stack_depth = 10000;
	config->expression_segment_size = 10000;
	config->max_number_of_frames = 50;
	config->max_number_of_functions = 100;
	config->max_number_of_scopes = 1000;

	config->max_number_of_globals = 100;
	config->register_stack_depth = 200;

	config->debug.active = 1;
	config->debug.max_number_of_symbols = 1000;
	config->debug.max_number_of_breakpoints = 10;
}

static int construct_functions_segment(struct machine_t *machine)
{
	int size = sizeof(struct callable_unit_t) * machine->config.max_number_of_functions;
	if (!(_REG_US(_REGS(machine)) = (struct callable_unit_t *)malloc(size)))
	{
		machine->die(machine, 1, "could not construct functions segment");
	}

	memset(_REG_US(_REGS(machine)), 0, size);

	_REG_FUP(_REGS(machine)) = 0;

	return 1;
}

static int construct_expression_segment(struct machine_t *machine)
{
	int size = sizeof(struct expression_term_t) * machine->config.expression_segment_size;
	if (!(_REG_XS(_REGS(machine)) = (struct expression_term_t *)malloc(size)))
	{
		machine->die(machine, 1, "could not construct expressions segment");
	}

	memset(_REG_XS(_REGS(machine)), 0, size);
	_REG_FXP(_REGS(machine)) = 0;

	return 1;
}

static int construct_code_segment(struct machine_t *machine)
{
	int size = sizeof(opcode_t) * machine->config.code_segment_size;
	if (!(_REG_CS(_REGS(machine)) = (opcode_t *)malloc(size)))
	{
		machine->die(machine, 1, "could not construct code segment");
	}

	memset(_REG_CS(_REGS(machine)), 0, size);

	_REG_FCP(_REGS(machine)) = 0;
	_SET_PC(_REGS(machine), 0);

	return 1;
}

static int construct_data_segment(struct machine_t *machine)
{
	int size = machine->config.data_segment_size;
	if (!(_REG_DS(_REGS(machine)) = (void *)calloc(1, size)))
	{
		machine->die(machine, 1, "could not construct data segment");
	}

	_REG_FDP(_REGS(machine)) = 0;

	return 1;
}

static int construct_string_segment(struct machine_t *machine)
{
	int size = machine->config.string_segment_size;
	if (!(_REG_SS(_REGS(machine)) = (char *)calloc(1, size)))
	{
		machine->die(machine, 1, "could not construct string segment");
	}

	_REG_FSP(_REGS(machine)) = 0;

	return 1;
}

static int construct_register_metadata(struct machine_t *machine)
{
	int size = sizeof(struct register_metadata_t) * machine->config.register_stack_depth;
	if (!(machine->register_metadata = (struct register_metadata_t *)malloc(size)))
	{
		machine->die(machine, 1, "could not construct register metadata");
	}

	memset(machine->register_metadata, 0, size);

	return 1;
}

static int construct_function_metadata(struct machine_t *machine)
{
	int size = sizeof(struct callable_unit_metadata_t) * machine->config.max_number_of_functions;
	if (!(machine->callable_unit_metadata = (struct callable_unit_metadata_t *)malloc(size)))
	{
		machine->die(machine, 1, "could not construct function metadata");
	}

	memset(machine->callable_unit_metadata, 0, size);

	return 1;
}

static int construct_debug(struct machine_t *machine)
{
	machine->debug = 0;
	if (!machine->config.debug.active) return 1;

#ifndef _6IT_DEBU66ER
	machine->config.debug.active = 0;
	return 1;
#endif

	if (!(machine->debug = (struct machine_debugger_support_t *)malloc(sizeof(struct machine_debugger_support_t))))
	{
		return 0;
	}

	machine->debug->machine = machine;

	construct_machine_debugger_support(machine->debug, machine->config.debug);

	return 1;
}

static int try_construct_segments(struct machine_t *machine)
{
	if (!construct_functions_segment(machine)) return 0;
	if (!construct_expression_segment(machine)) return 0;
	if (!construct_data_segment(machine)) return 0;
	if (!construct_string_segment(machine)) return 0;
	if (!construct_code_segment(machine)) return 0;
	if (!construct_register_metadata(machine)) return 0;
	if (!construct_function_metadata(machine)) return 0;
	if (!construct_debug(machine)) return 0;

	return 1;
}

_6IT_DESTRUCTOR(machine, H6VM)
{
	_This->ready = 0;

	if (_REG_US(_REGS(_This)))
	{
		free(_REG_US(_REGS(_This)));
		_REG_US(_REGS(_This)) = 0;
	}

	if (_REG_XS(_REGS(_This)))
	{
		free(_REG_XS(_REGS(_This)));
		_REG_XS(_REGS(_This)) = 0;
	}

	if (_REG_CS(_REGS(_This)))
	{
		free(_REG_CS(_REGS(_This)));
		_REG_CS(_REGS(_This)) = 0;
	}

	if (_REG_DS(_REGS(_This)))
	{
		free(_REG_DS(_REGS(_This)));
		_REG_DS(_REGS(_This)) = 0;
	}

	if (_This->callable_unit_metadata)
	{
		free(_This->callable_unit_metadata);
		_This->callable_unit_metadata = 0;
	}

	if (_This->register_metadata)
	{
		free(_This->register_metadata);
		_This->register_metadata = 0;
	}

	if (_REG_SS(_REGS(_This)))
	{
		free(_REG_SS(_REGS(_This)));
		_REG_SS(_REGS(_This)) = 0;
	}

	if (_This->debug)
	{
		destruct_machine_debug(_This->debug);
		free(_This->debug);
		_This->debug = 0;
	}
}

// todo crz: tidy this

_6IT_PUBLIC _6IT_CONSTRUCTORX(machine, H6VM, struct machine_config_t config)
{
	INITIALISE_COMMON_ATTRIBUTES(machine, H6VM, _Machine);

	_This->config = config;

	construct_expression_emitter(&_This->expression_emitter, _This);
	construct_instruction_emitter(&_This->instruction_emitter, _This);
#ifdef _6IT_DEBU66ER
	construct_6UG(&_This->debugger, _This);
#endif
	construct_6IOS(&_Bios, 0); // hack crz: this should only be a singleton

	_This->bind_downcompiler = _H6VM_METHOD_NAME(bind_downcompiler);
	_This->bind_evaluator = _H6VM_METHOD_NAME(bind_evaluator);
	_This->bind_processor = _H6VM_METHOD_NAME(bind_processor);
	_This->bind_scanner = _H6VM_METHOD_NAME(bind_scanner);
	_This->bind_compiler = _H6VM_METHOD_NAME(bind_compiler);
	_This->bind_environment = _H6VM_METHOD_NAME(bind_environment);

	_This->printf = _H6VM_METHOD_NAME(printf);

	// crz: debugger support

	_This->first_chance_exception_handler = _H6VM_METHOD_NAME(first_chance_exception_handler);
	_This->check_state = _H6VM_METHOD_NAME(check_state);
	_This->format_primitive_value = _H6VM_METHOD_NAME(format_primitive_value);
	_This->format_machine_code = _H6VM_METHOD_NAME(format_machine_code);
	_This->get_referenced_expressions = _H6VM_METHOD_NAME(get_referenced_expressions);
	_This->disassemble_instruction = _H6VM_METHOD_NAME(disassemble_instruction);
	_This->type_description = _H6VM_METHOD_NAME(type_description);
	_This->set_breakpoint = _H6VM_METHOD_NAME(set_breakpoint);
	_This->find_free_breakpoint = _H6VM_METHOD_NAME(find_free_breakpoint);

	// crz: allocation

	_This->allocate_free_static_register = _H6VM_METHOD_NAME(allocate_free_static_register);
	_This->allocate_static_register = _H6VM_METHOD_NAME(allocate_static_register);
	_This->allocate_frame_register = _H6VM_METHOD_NAME(allocate_frame_register);

	// crz: callable units

	_This->get_callable_unit = _H6VM_METHOD_NAME(get_callable_unit);
	_This->add_callable_unit = _H6VM_METHOD_NAME(add_callable_unit);
	_This->add_builtin = _H6VM_METHOD_NAME(add_builtin);

	// crz: expressions

	_This->build_expression = _H6VM_METHOD_NAME(build_expression);
	_This->get_final_expression_term = _H6VM_METHOD_NAME(get_final_expression_term);

	// crz: symbols

	_This->find_static_register = _H6VM_METHOD_NAME(find_static_register);
	_This->find_frame_register = _H6VM_METHOD_NAME(find_frame_register);
	_This->find_register = _H6VM_METHOD_NAME(find_register);
	_This->get_register_metadata = _H6VM_METHOD_NAME(get_register_metadata);
	_This->register_set_symbol = _H6VM_METHOD_NAME(register_set_symbol);
	_This->register_get_symbol = _H6VM_METHOD_NAME(register_get_symbol);

	_This->get_callable_unit_metadata = _H6VM_METHOD_NAME(get_callable_unit_metadata);
	_This->find_callable_unit = _H6VM_METHOD_NAME(find_callable_unit);
	_This->find_callable_unit_by_entry_point = _H6VM_METHOD_NAME(find_callable_unit_by_entry_point);
	_This->get_callable_unit_symbol = _H6VM_METHOD_NAME(get_callable_unit_symbol);
	_This->set_callable_unit_symbol = _H6VM_METHOD_NAME(set_callable_unit_symbol);
	_This->get_callable_unit_parameter_symbol = _H6VM_METHOD_NAME(get_callable_unit_parameter_symbol);
	_This->set_callable_unit_parameter_symbol = _H6VM_METHOD_NAME(set_callable_unit_parameter_symbol);

	// crz: program

	_This->new_program = _H6VM_METHOD_NAME(new_program);
	_This->resolve_externals = _H6VM_METHOD_NAME(resolve_externals);

#ifdef _6IT_SUPPORT_LUA
	_This->bind_lua = _H6VM_METHOD_NAME(bind_lua);
	_This->resolve_lua_externals = _H6VM_METHOD_NAME(resolve_lua_externals);
	_This->push_to_lua = _H6VM_METHOD_NAME(push_to_lua);
	_This->pull_from_lua = _H6VM_METHOD_NAME(pull_from_lua);
#endif

	// crz: strings

	_This->add_string = _H6VM_METHOD_NAME(add_string);
	_This->get_string = _H6VM_METHOD_NAME(get_string);

	// crz: theeading

	_This->create_thread = _H6VM_METHOD_NAME(create_thread);
	_This->switch_context = _H6VM_METHOD_NAME(switch_context);

#ifdef _6IT_SUPPORT_INTERRUPTS
	construct_interrupt_controller(&_This->interrupt_controller, _This);
#endif

#ifdef _6IT_SUPPORT_THREADS
	construct_scheduler(&_This->scheduler, _This);
#endif
	_This->current_thread_idx = -1;
	_This->create_thread(_This, &_This->primary_thread, 0);
	_This->switch_context(_This, _This->primary_thread.id);

	if (!try_construct_segments(_This))
	{
		_This->destruct(_This);
		// todo crz: throw here
		return;
	}

	_SET_RSP(_REGS(_This), _This->config.max_number_of_globals);
	_FRAME(_This) = _REG_ES(_REGS(_This));
	_SET_FRAME_REGISTERS(_This, _REG_RS(_REGS(_This)) + _REG_RSP(_REGS(_This)));

	_This->condition_flag = 1;
	_This->return_flag = 0;
	_FRAME(_This) = _REG_ES(_REGS(_This));

	_REG_VPC(_REGS(_This)) = -1;

	_int_value.type = TYPE_INT; // crz: ??
	_This->tocks = 0;
}
