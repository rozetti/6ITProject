/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

struct expression_parser_t
{
	DEFINE_COMMON_ATTRIBUTES(expression_parser);

	struct scanner_t *scanner;

	void(*emit_string)(struct expression_parser_t*, struct expression_t*, int index, int source_position);
	void(*emit_variable)(struct expression_parser_t*, struct expression_t*, struct register_t const *var, int source_position);
	void(*emit_integer)(struct expression_parser_t*, struct expression_t*, int value, int source_position);
	void(*emit_float)(struct expression_parser_t*, struct expression_t*, float value, int source_position);
	void(*emit_call)(struct expression_parser_t*, struct expression_t*, struct callable_unit_t*, int number_of_args, int source_position);
	void(*emit_static_register_allocation)(struct expression_parser_t*, struct expression_t*, int type, int source_position);
	void(*emit_frame_register_allocation)(struct expression_parser_t*, struct expression_t*, int type, int source_position);

	int(*parse_expression)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);

	void(*parse_assignment)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_additive)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_relational)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_multiplicative)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_unary_pre_assignment)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_unary_post_assignment)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_unary)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_brackets)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_identifier)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_primitive)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_numeric_literal)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_char_literal)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);
	void(*parse_string_literal)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*);

	int(*try_parse_cast)(struct expression_parser_t*, struct expression_parser_state_t*, struct expression_t*, int *op);
	data_type_t(*parse_data_type)(struct expression_parser_t*, struct expression_parser_state_t*);
};

struct expression_parser_state_t
{
	struct expression_parser_t *parser;
	char message[100];
	struct exception_t exception;
	int is_temporary;

	int use_immediates;
	int numbers_are_floats;
};

