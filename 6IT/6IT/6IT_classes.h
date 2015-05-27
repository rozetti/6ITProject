/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

// todo crz: investigate portability of varidic macros

#ifdef _6IT_PRIVATE
#undef _6IT_PRIVATE
#endif
#define _6IT_PRIVATE 

#define _6IT_PUBLIC

#define _6IT_CONSTRUCTOR(T, NAME) void construct_##NAME(struct T##_t* _This, struct machine_t *_Machine)
#define _6IT_CONSTRUCTORX(T, NAME, PARM) void construct_##NAME(struct T##_t* _This, struct machine_t *_Machine, PARM)
#define _6IT_CONSTRUCTORXX(T, NAME, P1, P2) void construct_##NAME(struct T##_t* _This, struct machine_t *_Machine, P1, P2)

#define _6IT_DESTRUCTOR(T) _6IT_PRIVATE void T##_destruct(struct T##_t* _This)
#define _6IT_THISCALL_DIE(T) _6IT_PRIVATE void T##_die(struct T##_t* _This, int fault_code, char const *message)
#define _6IT_THISCALL_GET_ERROR_STRING(T) _6IT_PRIVATE char const *T##_get_error_string(struct T##_t* _This, int error)

#define _6IT_THISCALL(T, NAME) NAME(struct T##_t *_This)
#define _6IT_THISCALLX(T, NAME, P1) NAME(struct T##_t *_This, P1)
#define _6IT_THISCALLXX(T, NAME, P1, P2) NAME(struct T##_t *_This, P1, P2)
#define _6IT_THISCALLXXX(T, NAME, P1, P2, P3) NAME(struct T##_t *_This, P1, P2, P3)
#define _6IT_THISCALLXXXX(T, NAME, P1, P2, P3, P4) NAME(struct T##_t *_This, P1, P2, P3, P4)

#define _6IT_THISCALL_CONST(T, NAME) NAME(struct T##_t const *_This)
#define _6IT_THISCALLX_CONST(T, NAME, P1) NAME(struct T##_t const *_This, P1)
#define _6IT_THISCALLXX_CONST(T, NAME, P1, P2) NAME(struct T##_t const *_This, P1, P2)
#define _6IT_THISCALLXXX_CONST(T, NAME, P1, P2, P3) NAME(struct T##_t const *_This, P1, P2, P3)
#define _6IT_THISCALLXXXX_CONST(T, NAME, P1, P2, P3, P4) NAME(struct T##_t const *_This, P1, P2, P3, P4)

#define _6IT_STATECALL(T, NAME) NAME(struct T##_t *_This, struct T##_state_t *_State)
#define _6IT_STATECALLX(T, NAME, P1) NAME(struct T##_t *_This, struct T##_state_t *_State, P1)
#define _6IT_STATECALLXX(T, NAME, P1, P2) NAME(struct T##_t *_This, struct T##_state_t *_State, P1, P2)
#define _6IT_STATECALLXXX(T, NAME, P1, P2, P3) NAME(struct T##_t *_This, struct T##_state_t *_State, P1, P2, P3)
#define _6IT_STATECALLXXXX(T, NAME, P1, P2, P3, P4) NAME(struct T##_t *_This, struct T##_state_t *_State, P1, P2, P3, P4)

#define _6IT_STATECALL_CONST(T, NAME) NAME(struct T##_t const *_This, struct T##_state_t *_State)
#define _6IT_STATECALLX_CONST(T, NAME, P1) NAME(struct T##_t const *_This, struct T##_state_t *_State, P1)
#define _6IT_STATECALLXX_CONST(T, NAME, P1, P2) NAME(struct T##_t const *_This, struct T##_state_t *_State, P1, P2)
#define _6IT_STATECALLXXX_CONST(T, NAME, P1, P2, P3) NAME(struct T##_t const *_This, struct T##_state_t *_State, P1, P2, P3)
#define _6IT_STATECALLXXXX_CONST(T, NAME, P1, P2, P3, P4) NAME(struct T##_t const *_This, struct T##_state_t *_State, P1, P2, P3, P4)

#define _6IT_MACHINECALL(NAME) NAME(struct machine_t *_Machine)
#define _6IT_MACHINECALLX(NAME, PARM) NAME(struct machine_t *_Machine, PARM)
#define _6IT_MACHINECALLXX(NAME, P1, P2) NAME(struct machine_t *_Machine, P1, P2)
#define _6IT_MACHINECALLXXX(NAME, P1, P2, P3) NAME(struct machine_t *_Machine, P1, P2, P3)
#define _6IT_MACHINECALLXXXX(NAME, P1, P2, P3, P4) NAME(struct machine_t *_Machine, P1, P2, P3, P4)

#define MACHINECALL_METHOD(R, NAME) _6IT_PRIVATE R _6IT_MACHINECALL(NAME)
#define MACHINECALL_METHODX(R, NAME, PARM) _6IT_PRIVATE R _6IT_MACHINECALLX(NAME, PARM)
#define MACHINECALL_METHODXX(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_MACHINECALLXX(NAME, P1, P2)
#define MACHINECALL_METHODXXX(R, NAME, P1, P2, P3) _6IT_PRIVATE R _6IT_MACHINECALLXXX(NAME, P1, P2, P3)
#define MACHINECALL_METHODXXXX(R, NAME, P1, P2, P3, P4) _6IT_PRIVATE R _6IT_MACHINECALLXXXX(NAME, P1, P2, P3, P4)

#define MACHINE_METHOD(R, NAME) _6IT_PRIVATE R _6IT_THISCALL(machine, NAME)
#define MACHINE_METHODX(R, NAME, P1) _6IT_PRIVATE R _6IT_THISCALLX(machine, NAME, P1)
#define MACHINE_METHODXX(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_THISCALLXX(machine, NAME, P1, P2)
#define MACHINE_METHODXXX(R, NAME, P1, P2, P3) _6IT_PRIVATE R _6IT_THISCALLXXX(machine, NAME, P1, P2, P3)

#define DEBUGGER_METHOD(R, NAME) _6IT_PRIVATE R _6IT_THISCALL(debugger, NAME)
#define DEBUGGER_METHODX(R, NAME, PARM) _6IT_PRIVATE R _6IT_THISCALLX(debugger, NAME, PARM)
#define DEBUGGER_METHODXX(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_THISCALLXX(debugger, NAME, P1, P2)

#define EVALUATOR_METHOD(NAME) _6IT_THISCALL(evaluator, NAME)
#define EVALUATOR_METHODX(NAME, PARM) _6IT_THISCALLX(evaluator, NAME, PARM)
#define EVALUATOR_METHODXX(NAME, P1, P2) _6IT_THISCALLXX(evaluator, NAME, P1, P2)

#define PROCESSOR_METHOD(NAME) _6IT_THISCALL(processor, NAME)
#define PROCESSOR_METHODX(NAME, PARM) _6IT_THISCALLX(processor, NAME, PARM)
#define PROCESSOR_METHODXX(NAME, P1, P2) _6IT_THISCALLXX(processor, NAME, P1, P2)

#define SCANNER_METHOD_CONST(R, NAME) _6IT_PRIVATE R _6IT_THISCALL_CONST(scanner, NAME)
#define SCANNER_METHODX_CONST(R, NAME, PARM) _6IT_PRIVATE R _6IT_THISCALLX_CONST(scanner, NAME, PARM)

#define SCANNER_METHOD(R, NAME) _6IT_PRIVATE R _6IT_THISCALL(scanner, NAME)
#define SCANNER_METHODX(R, NAME, PARM) _6IT_PRIVATE R _6IT_THISCALLX(scanner, NAME, PARM)
#define SCANNER_METHODXX(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_THISCALLXX(scanner, NAME, P1, P2)

#define EXPRESSION_PARSER_METHOD(R, NAME) _6IT_PRIVATE R _6IT_THISCALL(expression_parser, NAME)
#define EXPRESSION_PARSER_METHODX(R, NAME, PARM) _6IT_PRIVATE R _6IT_THISCALLX(expression_parser, NAME, PARM)
#define EXPRESSION_PARSER_METHODXX(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_THISCALLXX(expression_parser, NAME, P1, P2)
#define EXPRESSION_PARSER_METHODXXX(R, NAME, P1, P2, P3) _6IT_PRIVATE R _6IT_THISCALLXXX(expression_parser, NAME, P1, P2, P3)
#define EXPRESSION_PARSER_METHODXXXX(R, NAME, P1, P2, P3, P4) _6IT_PRIVATE R _6IT_THISCALLXXXX(expression_parser, NAME, P1, P2, P3, P4)

#define EXPRESSION_EMITTER_METHOD(R, NAME) _6IT_PRIVATE R _6IT_THISCALL(expression_emitter, NAME)
#define EXPRESSION_EMITTER_METHODX(R, NAME, PARM) _6IT_PRIVATE R _6IT_THISCALLX(expression_emitter, NAME, PARM)
#define EXPRESSION_EMITTER_METHODXX(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_THISCALLXX(expression_emitter, NAME, P1, P2)
#define EXPRESSION_EMITTER_METHODXXX(R, NAME, P1, P2, P3) _6IT_PRIVATE R _6IT_THISCALLXXX(expression_emitter, NAME, P1, P2, P3)
#define EXPRESSION_EMITTER_METHODXXXX(R, NAME, P1, P2, P3, P4) _6IT_PRIVATE R _6IT_THISCALLXXXX(expression_emitter, NAME, P1, P2, P3, P4)

#define INSTRUCTION_EMITTER_METHOD(R, NAME) _6IT_PRIVATE R _6IT_THISCALL(instruction_emitter, NAME)
#define INSTRUCTION_EMITTER_METHODX(R, NAME, PARM) _6IT_PRIVATE R _6IT_THISCALLX(instruction_emitter, NAME, PARM)
#define INSTRUCTION_EMITTER_METHODXX(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_THISCALLXX(instruction_emitter, NAME, P1, P2)
#define INSTRUCTION_EMITTER_METHODXXX(R, NAME, P1, P2, P3) _6IT_PRIVATE R _6IT_THISCALLXXX(instruction_emitter, NAME, P1, P2, P3)

#define COMPILER_METHOD_CONST(R, NAME) _6IT_PRIVATE R _6IT_STATECALL_CONST(compiler, NAME)
#define COMPILER_METHODX_CONST(R, NAME, P1) _6IT_PRIVATE R _6IT_STATECALLX_CONST(compiler, NAME, P1)
#define COMPILER_METHODXX_CONST(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_STATECALLXX_CONST(compiler, NAME, P1, P2)

#define DOWNCOMPILER_METHOD_CONST(R, NAME) _6IT_PRIVATE R _6IT_STATECALL_CONST(downcompiler, NAME)
#define DOWNCOMPILER_METHODX_CONST(R, NAME, P1) _6IT_PRIVATE R _6IT_STATECALLX_CONST(downcompiler, NAME, P1)
#define DOWNCOMPILER_METHODXX_CONST(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_STATECALLXX_CONST(downcompiler, NAME, P1, P2)

#define SCHEDULER_METHOD_CONST(R, NAME) _6IT_PRIVATE R _6IT_THISCALL_CONST(scheduler, NAME)
