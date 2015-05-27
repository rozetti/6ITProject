/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include "6IT.h"

struct compiler_t
{
	DEFINE_COMMON_ATTRIBUTES(compiler);
	DEFINE_POLYMORPHIC_ATTRIBUTE(scanner, scanner);

	struct scanner_t *scanner;

	int _6IT_THISCALL(compiler, (*compile));
};

struct compiler_state_t
{
	int loop_PC;
	int base_RSP;
	int callable_unit_PC;

	struct frame_t *frame;
	struct expression_parser_state_t *expression_parser_state;

	int global_expressions[100];
	int number_of_global_expressions;

	int number_of_errors;
	int number_of_warnings;

	int fatal;
};



