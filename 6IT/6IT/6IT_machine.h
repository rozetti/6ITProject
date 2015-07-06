/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include "6IT_scheduler.h"
#include "6IT_interrupt_controller.h"

#include <time.h>

#ifdef _6IT_SUPPORT_LUA
struct lua_State; // hack crz: obviously de-hack this
#endif

#define MACHINE_EFFECTIVE_EXECUTION_START_MS(M) ((M)->execution_start_ms + (M)->pause_ms)

// todo crz: probably get this of this
enum instruction_weight
{
	LIGHT,
	HEAVY
};

struct symbol_t
{
	char identifier[MAX_IDENTIFIER_LENGTH + 1];
	int type;
	int idx;
};

struct mnemonic_t
{
	int opcode;
	char const *mnemonic;
	int stack_delta;
	int weight;
	int parm_type;
	int number_of_operands;
	int operands[5];
};

struct expression_term_t
{
	struct value_t data;
	int type;
	int number_of_args;
};

struct expression_t
{
	int source_start;
	int source_end;
	struct expression_term_t *terms;
	int first_term_index;
	int number_of_terms;
	int is_global; // hack crz
};

struct evaluator_stack_t
{
	struct value_t values[100];
	int top;
};

// todo crz: tidy this
struct frame_t
{
	struct register_t *frame_registers;
	opcode_t *entry_ip;
	opcode_t *return_ip;
	struct value_t return_value;
	int position;
	struct value_t *evaluator_stack_pointer;

	struct machine_t *machine;
	int callable_unit_idx;
	int evaluator_program_counter;
};

struct evaluator_t
{
	int(*evaluate)(struct machine_t*);
	int(*evaluate_debug)(struct machine_t*);

	int(*check_expression)(struct machine_t*, struct expression_parser_state_t*, struct expression_t const*);
	void(*print_expression)(struct machine_t*, struct expression_t const*);
	void(*print_state)(struct machine_t*, struct expression_t const*);
	struct mnemonic_t const *(*get_mnemonic)(opcode_t opcode);

	DEFINE_COMMON_ATTRIBUTES(evaluator);
};

struct processor_t
{
	void(*execute)(struct machine_t*);
	void(*execute_debug)(struct machine_t*);
	void(*execute_and_reset)(struct machine_t*);

	struct mnemonic_t const *(*get_mnemonic)(opcode_t);
	int(*get_instruction_length)(opcode_t);

	DEFINE_COMMON_ATTRIBUTES(processor);
};

struct callable_unit_t
{
	int idx;
	int return_type;
	data_type_t parameters[MAX_FUNCTION_PARAMETERS];
	char number_of_parameters;
	int is_vararg;
	int entry_point_program_counter;
	int exit_point_program_counter;

	int(*entry_point_ptr)(struct machine_t*);
	int is_lua;
	int is_resolved;
};

struct callable_unit_metadata_t
{
	char symbol[MAX_IDENTIFIER_LENGTH + 1];
	struct register_metadata_t parameter_metadata[MAX_FUNCTION_PARAMETERS];
};

struct machine_debug_config_t
{
	int active;
	int max_number_of_symbols;
	int max_number_of_breakpoints;
};

struct machine_debugger_support_t
{
	struct symbol_t *symbols;
	int free_symbol_index;

	struct breakpoint_t *breakpoints;

	int opcode_trace_on;
	int eval_trace_on;
	int line_trace_on;
	int token_trace_on;
	int call_trace_on;

	clock_t clocks[NUM_DEBUG_CLOCKS];

	struct machine_debug_config_t config;
	struct machine_t *machine;

	int count_statements_interpreted;
	int count_instructions_executed;
};

struct machine_config_t
{
	int max_number_of_tokens;
	int max_number_of_frames;
	int max_number_of_globals;
	int max_number_of_functions;
	int max_number_of_scopes;
	int max_number_of_expressions;

	int data_segment_size;
	int string_segment_size;
	int code_segment_size;
	int expression_segment_size;

	int register_stack_depth;
	int evaluator_stack_depth;

	struct machine_debug_config_t debug;
};

struct thread_t
{
	int id;

	struct machine_registers_t registers;
	struct machine_registers_t saved_reg;

	DEFINE_COMMON_ATTRIBUTES(thread);
};

#define _THREAD(M) (M)->threads[(M)->current_thread_idx]

struct instruction_emitter_t
{
	struct machine_t *machine;

	int(*emit_instruction)(struct instruction_emitter_t*, int opcode, int parm);
	int(*emit_PC)(struct instruction_emitter_t*, int pc);
	int(*emit_integer)(struct instruction_emitter_t*, int n);
	int(*emit_float)(struct instruction_emitter_t*, float f);
	int(*emit_expression_index)(struct instruction_emitter_t*, int index);
	int(*emit_placeholder)(struct instruction_emitter_t*);
	void(*set_code)(struct instruction_emitter_t*, int pc, int code);
};

struct expression_emitter_t
{
	struct machine_t *machine;

	void(*append_term)(struct expression_emitter_t*, struct expression_t*, struct expression_term_t*, int source_position);
	void(*emit_finished)(struct expression_emitter_t*, struct expression_t*, int source_position);
	void(*emit_binary_operator)(struct expression_emitter_t*, struct expression_t*, int op, int source_position);
	void(*emit_opcode)(struct expression_emitter_t*, struct expression_t*, int op, int source_position);
	void(*emit_unary_operator)(struct expression_emitter_t*, struct expression_t*, int op, int source_position);
	void(*emit_frame_register_assignment)(struct expression_emitter_t*, struct expression_t*, struct register_t const*, int op, int source_position);
	void(*emit_static_register_assignment)(struct expression_emitter_t*, struct expression_t*, struct register_t const*, int op, int source_position);
};

struct machine_t
{
	void(*execute)(struct machine_t*);
	int(*evaluate)(struct machine_t*);
	struct machine_registers_t registers;

	int tocks;

	struct thread_t primary_thread;
	struct thread_t *threads[10]; // crz: magic number
	int number_of_threads;
	int current_thread_idx;

	int execution_start_ms;
	int pause_ms;

	int single_step_flag;
	int condition_flag;
	int return_flag;

	struct evaluator_t evaluator;
	struct processor_t processor;
#ifdef _6IT_DEBU66ER
	struct debugger_t debugger;
#endif
	struct downcompiler_t downcompiler;
	struct expression_emitter_t expression_emitter;
	struct instruction_emitter_t instruction_emitter;

#ifdef _6IT_SUPPORT_INTERRUPTS
	struct interrupt_controller_t interrupt_controller;
	int interrupt_levels[_NUMBER_OF_INTERRUPT_LEVELS];
	atomic_flag_t interrupt_flag;
#endif

#ifdef _6IT_SUPPORT_THREADS
	struct scheduler_t scheduler;
#endif

#ifdef _6IT_SUPPORT_LUA
	struct lua_State *lua;
#endif
	struct register_metadata_t *register_metadata;
	struct callable_unit_metadata_t *callable_unit_metadata;

	// todo crz: put these back
	//_This->disassemble_instruction = disassemble_instruction;
	char const *(*format_primitive_value)(struct machine_t *machine, struct value_t *value);
	char const *(*format_machine_code)(struct machine_t *machine, int type, int code);
//	char const *(*type_description)(struct machine_t *machine, data_type_t type);
	MACHINE_METHODX(char const *, (*type_description), data_type_t type);
	MACHINE_METHODXX(char const *, (*disassemble_instruction), int pc, int callable_unit_PC);

	void(*printf)(struct machine_t*, char const *format, ...);
	// crz: register methods

	struct register_t *(*allocate_frame_register)(struct machine_t*, int type);
	struct register_t *(*allocate_free_static_register)(struct machine_t*, int type);
	struct register_t *(*allocate_static_register)(struct machine_t*, int idx);
	struct register_t *(*find_frame_register)(struct machine_t*, char const *symbol);
	struct register_t *(*find_static_register)(struct machine_t*, char const *symbol);
	struct register_t *(*find_register)(struct machine_t*, char const *symbol);
	char const *(*register_get_symbol)(struct machine_t*, int idx);
	void(*register_set_symbol)(struct machine_t*, int idx, char const *name);
	struct register_metadata_t *(*get_register_metadata)(struct machine_t*, int idx);

	// crz: callable unit methods
	struct callable_unit_t *(*get_callable_unit)(struct machine_t*, int idx);
	struct callable_unit_metadata_t *(*get_callable_unit_metadata)(struct machine_t*, int idx);
	struct callable_unit_t *(*find_callable_unit)(struct machine_t*, char const *name);
	struct callable_unit_t *(*find_callable_unit_by_entry_point)(struct machine_t*, int entry);
	struct callable_unit_t *(*add_callable_unit)(struct machine_t*, int return_type, char const *name);
	struct callable_unit_t *(*add_builtin)(struct machine_t*, int return_type, char const *name, int(*entry_point_ptr)(struct machine_t*), int *parms);
	char const *(*get_callable_unit_symbol)(struct machine_t*, int idx);
	void(*set_callable_unit_symbol)(struct machine_t*, int idx, char const *name);
	char const *(*get_callable_unit_parameter_symbol)(struct machine_t*, int callable_unit_idx, int parm_idx);
	void(*set_callable_unit_parameter_symbol)(struct machine_t*, int callable_unit_idx, int idx, char const *name);

	int(*vcall)(struct machine_t *machine, struct callable_unit_t const *, void *return_value, ...);

	void(*build_expression)(struct machine_t*, struct expression_t*, int ptr);
	int(*first_chance_exception_handler)(struct exception_t*);

	// crz: binding
	void(*bind_processor)(struct machine_t*, struct processor_t*);
	void(*bind_evaluator)(struct machine_t*, struct evaluator_t*);
	void(*bind_downcompiler)(struct machine_t*, struct downcompiler_t*);
	void(*bind_scanner)(struct machine_t*, struct scanner_t*);
	void(*bind_compiler)(struct machine_t*, struct compiler_t*);
	void(*bind_environment)(struct machine_t*, struct environment_t*);
	
	struct expression_term_t *(*get_final_expression_term)(struct machine_t*, struct expression_t*);

	void(*new_program)(struct machine_t*);
	void(*resolve_externals)(struct machine_t*);
	void(*state_changed)(struct machine_t*);

	// crz: lua support
#ifdef _6IT_SUPPORT_LUA
	void(*bind_lua)(struct machine_t*, struct lua_State*);
	void(*resolve_lua_externals)(struct machine_t*);
	void(*pull_from_lua)(struct machine_t*);
	void(*push_to_lua)(struct machine_t*);
#endif

	MACHINE_METHOD(int, (*check_state));

	// crz: strings
	MACHINE_METHODXX(void, (*add_string), char const *string, int *index);
	MACHINE_METHODX(char const *, (*get_string), int index);

	// crz: expressions
	MACHINE_METHODXX(int, (*get_referenced_expressions), int pc, int *expressions);
		
	// crz: threading
	void(*create_thread)(struct machine_t*, struct thread_t*, struct callable_unit_t*);
	void(*switch_context)(struct machine_t*, int id);

	// crz: debugger
	MACHINE_METHOD(int, (*find_free_breakpoint));
	MACHINE_METHODX(void, (*set_breakpoint), int pc);

	// crz: console
	//MACHINE_METHOD(char const *, (*read_line));

	struct machine_debugger_support_t *debug;
	struct machine_config_t config;

	int ready;
	int fault_code;

	DEFINE_COMMON_ATTRIBUTES(machine);
};
