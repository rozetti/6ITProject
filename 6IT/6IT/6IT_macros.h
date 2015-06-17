/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include <string.h>

#define H6VM_CODELESS 0xC0DE7E55
#define H6VM_DEADCODE 0xDEADC0DE

#define ITSOK 0

#define SCANNER_ERRORS 1000
#define COMPILER_ERRORS 2000
#define EXPRESSION_PARSER_ERRORS 3000
#define DOWNCOMPILER_ERRORS 4000
#define EVALUATOR_ERRORS 5000
#define PROCESSOR_ERRORS 6000
#define MACHINE_ERRORS 7000

// todo crz: put somewhere else
#define CALLABLE_UNIT(M, idx) (_REG_US((_REGS(M))) + (idx))

#ifdef _6IT_DEBU66ER
#define THROW(ex, i) \
	(ex)->fault_code = (i); \
	if (!(ex)->machine->first_chance_exception_handler(ex)) longjmp((ex)->restore_state, (i))

#define THROWM(ex, i, m) \
	(ex)->fault_code = (i); \
	strncpy((ex)->message, (m), EXCEPTION_MESSAGE_LENGTH); \
	if (!(ex)->machine->first_chance_exception_handler(ex)) longjmp((ex)->restore_state, (i))
#else
#define THROW(ex, i) \
	(ex)->fault_code = (i); \
	(ex)->message[0] = 0; \
	longjmp((ex)->restore_state, (i))
#define THROWM(ex, i, m) \
	(ex)->fault_code = (i); \
	strncpy((ex)->message, (m), EXCEPTION_MESSAGE_LENGTH); \
	longjmp((ex)->restore_state, (i))
#endif

#define CATCH(ex) \
	((ex)->fault_code = 0, \
	setjmp((ex)->restore_state))

#define DEFINE_REQUIRED_ATTRIBUTES(T) \
	struct machine_t *machine

#define DEFINE_COMMON_ATTRIBUTES(T) \
	struct T##_t *base; \
	void(*die)(struct T##_t *, int fault_code, char const *message); \
	char const *(*get_error_string)(struct T##_t *, int); \
	void(*destruct)(struct T##_t *); \
	int verbosity; \
	struct exception_t exception; \
	struct environment_t *environment; \
	DEFINE_REQUIRED_ATTRIBUTES(T)

#define INITIALISE_REQUIRED_ATTRIBUTES(T, M) \
	_This->machine = (M)

#define INITIALISE_COMMON_ATTRIBUTES(T, NS, M) \
	_This->base = _This; \
	_This->destruct = _##NS##_destruct; \
	_This->die = _##NS##_die; \
	_This->get_error_string = _##NS##_get_error_string; \
	_This->verbosity = 0; \
	_This->environment = 0; \
	_This->exception.fault_code = ITSOK; \
	_This->exception.machine = (M); \
	INITIALISE_REQUIRED_ATTRIBUTES(T, M)

#define DEFINE_POLYMORPHIC_ATTRIBUTE(CLASS, NAME) \
	struct CLASS##_t NAME##_base; \
	struct CLASS##_t NAME##_derived

#define INITIALISE_POLYMORPHIC_ATTRIBUTE(NAME) DERIVED_OBJECT(NAME) = BASE_OBJECT(NAME)

#define BASE_OBJECT(NAME) NAME##_base
#define DERIVED_OBJECT(NAME) NAME##_derived

#define MAX_IDENTIFIER_LENGTH 31 
#define NUM_DEBUG_CLOCKS 10
#define MAX_FUNCTION_PARAMETERS 10
#define MAX_TOKEN_LENGTH 31

// todo crz: put theses somewhere
#define SET_OPCODE_TRACE(M, on) if ((M)->debug) {(M)->debug->opcode_trace_on = (on) != 0; }
#define OPCODE_TRACE(M) ((M)->debug ? (M)->debug->opcode_trace_on : 0)

#define SET_EVALUATOR_TRACE(M, on) if ((M)->debug) {(M)->debug->eval_trace_on = (on) != 0; }
#define EVALUATOR_TRACE(M) ((M)->debug ? (M)->debug->eval_trace_on : 0)

#define SET_LINE_TRACE(M, on) if ((M)->debug) {(M)->debug->line_trace_on = (on) != 0; }
#define LINE_TRACE(M) ((M)->debug ? (M)->debug->line_trace_on : 0)

#define SET_TOKEN_TRACE(M, on) if ((M)->debug) {(M)->debug->token_trace_on = (on) != 0; }
#define TOKEN_TRACE(M) ((M)->debug ? (M)->debug->token_trace_on : 0)

// todo crz: probably move binding stuff somewhere nice
#define SET_STATIC_REGISTER_INT(M, name, i) \
(M)->find_static_register((M), "binding_target")->value.as_integer = (i)

#define GET_STATIC_REGISTER_INT(M, name) \
((M)->find_static_register((M), "binding_target")->value.as_integer)


