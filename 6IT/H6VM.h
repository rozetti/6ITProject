/*****************************************************************************
The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org.
******************************************************************************
The MIT License (MIT)

Copyright(c) 2015 Conrad Rozetti

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

#pragma once

#include "6IT.h"

#define DEBUG_SHOW_ADDED_SYMBOL(M) ((M)->verbosity >= 1)

// todo crz: put these somewhere nice
int construct_machine_debugger_support(struct machine_debugger_support_t *debug, struct machine_debug_config_t config);
void destruct_machine_debug(struct machine_debugger_support_t *debug);
int machine_debug_is_active(struct machine_t *machine);

//void debug_printf(char const *format, ...);

int machine_first_chance_exception_handler(struct exception_t *);
int machine_check_state(struct machine_t *machine);
void machine_minidump(struct machine_t *machine);

void machine_debug_reset_clock(struct frame_t *frame, int n);
int machine_debug_read_clock(struct frame_t *frame, int n);
int machine_debug_print_clock(struct frame_t *frame, int n);

enum
{
	ERROR_H6VM_ERRORS = MACHINE_ERRORS,
	ERROR_H6VM_UNKNOWN_TYPE_ON_VALUE_STACK
};

enum symbol_type_enum
{
	SYMBOL_TYPE_GLOBAL_VARIABLE,
	SYMBOL_TYPE_LOCAL_VARIABLE,
	SYMBOL_TYPE_FUNCTION
};

enum h6vm_registers
{
	H6VM_REG_PC, // crz: program counter (change to IP?)

	H6VM_REG_RS, // crz: register stack segment
	H6VM_REG_BP, // crz: register stack base pointer (points to first relative register for current execution stack frame)
	H6VM_REG_RSP, // crz: register stack pointer
	H6VM_REG_FRP, // crz: free absolute register pointer
	H6VM_REG_ES, // crz: executeion stack segment
	H6VM_REG_ESP, // crz: execution stack pointer
	H6VM_REG_XS, // crz: expression segemnt
	H6VM_REG_FXP, // crz: free expression pointer
	H6VM_REG_VS, // crz: evaluator stack segment
	H6VM_REG_VSP, // crz: evaluator stack pointer
	H6VM_REG_CS, // crz: code segment
	H6VM_REG_FCP, // crz: free code pointer
	H6VM_REG_DS, // crz: data segment
	H6VM_REG_FDP, // crz: free data pointer
	H6VM_REG_US, // crz: callable unit segment
	H6VM_REG_FUP, // crz: free callable unit pointer

	H6VM_REG_ACC, // crz: accumulator
	H6VM_REG_CMP, // crz: comparand

	H6VM_NUM_REG
};

enum operator_type
{
	OPERATOR_TYPE_UNARY,
	OPERATOR_TYPE_BINARY,
	OPERATOR_TYPE_TERNARY,
	OPERATOR_TYPE_SPECIAL
};

