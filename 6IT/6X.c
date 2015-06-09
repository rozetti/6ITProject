/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6X.h"

#ifdef _6IT_AMALGAMATE_SOURCE
#include "6X/6X_emitter.c"
#include "6X/6X_parser.c"
#include "6X/6X_parse_arithmetic.c"
#include "6X/6X_parse_assignment.c"
#include "6X/6X_parse_brackets.c"
#include "6X/6X_parse_data_type.c"
#include "6X/6X_parse_identifier.c"
#include "6X/6X_parse_primitive.c"
#include "6X/6X_parse_relational.c"
#endif

_6IT_DESTRUCTOR(expression_parser, 6X) { }

_6IT_THISCALL_DIE(expression_parser, 6X)
{
	THROW(&_This->exception, fault_code);
}

_6IT_THISCALL_GET_ERROR_STRING(expression_parser, 6X)
{
	switch (error)
	{
	case ERROR_6X_CLOSING_PARENTHESIS_EXPECTED:
		return "closing parenthesis expected";
	case ERROR_6X_UNDEFINED_IDENTIFIER:
		return "undefined identifier";
	}

	return 0;
}

_6IT_PUBLIC _6IT_CONSTRUCTORX(expression_parser, 6X, struct scanner_t *scanner)
{
	INITIALISE_COMMON_ATTRIBUTES(expression_parser, 6X, _Machine);

	_This->scanner = scanner;

	_This->parse_data_type = _6X_METHOD_NAME(parse_data_type);

	_This->emit_string = _6X_METHOD_NAME(emit_string);
	_This->emit_variable = _6X_METHOD_NAME(emit_variable);
	_This->emit_integer = _6X_METHOD_NAME(emit_integer);
	_This->emit_float = _6X_METHOD_NAME(emit_float);
	_This->emit_call = _6X_METHOD_NAME(emit_call);
	_This->emit_frame_register_allocation = _6X_METHOD_NAME(emit_frame_register_allocation);
	_This->emit_static_register_allocation = _6X_METHOD_NAME(emit_static_register_allocation);

	_This->parse_expression = _6X_METHOD_NAME(parse_expression);
	_This->parse_assignment = _6X_METHOD_NAME(parse_assignment);
	_This->parse_unary_post_assignment = _6X_METHOD_NAME(parse_unary_post_assignment);
	_This->parse_unary_pre_assignment = _6X_METHOD_NAME(parse_unary_pre_assignment);
	_This->parse_additive = _6X_METHOD_NAME(parse_additive);
	_This->parse_multiplicative = _6X_METHOD_NAME(parse_multiplicative);
	_This->parse_unary = _6X_METHOD_NAME(parse_unary);
	_This->parse_brackets = _6X_METHOD_NAME(parse_brackets);
	_This->parse_data_type = _6X_METHOD_NAME(parse_data_type);
	_This->parse_identifier = _6X_METHOD_NAME(parse_identifier);
	_This->parse_char_literal = _6X_METHOD_NAME(parse_char_literal);
	_This->parse_numeric_literal = _6X_METHOD_NAME(parse_numeric_literal);
	_This->parse_string_literal = _6X_METHOD_NAME(parse_string_literal);
	_This->parse_primitive = _6X_METHOD_NAME(parse_primitive);
	_This->parse_relational = _6X_METHOD_NAME(parse_relational);

	_This->try_parse_cast = _6X_METHOD_NAME(try_parse_cast);
}


