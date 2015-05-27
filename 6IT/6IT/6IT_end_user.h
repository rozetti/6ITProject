/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

struct calculator_t
{
	DEFINE_COMMON_ATTRIBUTES(calculator);

	int(*evaluate)(struct calculator_t*);

	char *buffer;
	int buffer_length;
	int buffer_offset;

	struct scanner_t scanner;
	struct expression_parser_t parser;
	struct expression_parser_state_t state;
	struct expression_t expression;
};

