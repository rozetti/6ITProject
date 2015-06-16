/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6X.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

_6X_METHODXX(void, parse_numeric_literal, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct scanner_t *scanner = state->parser->scanner;

	char const *p = strchr(scanner->token.token, '.');

	if (p || state->numbers_are_floats)
	{
		double d = strtof(scanner->token.token, 0);
		float f = (float)d;
		_This->emit_float(_This, expression, f, scanner->token.source_offset);
	}
	else
	{
		int v = atoi(scanner->token.token);
		_This->emit_integer(_This, expression, v, scanner->token.source_offset);
	}

	scanner->move_next_token(scanner);
}

_6X_METHODXX(void, parse_char_literal, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct scanner_t *scanner = state->parser->scanner;

	// todo crz: remove this assert and handle escape sequences
	assert(strlen(scanner->token.token) == 1);

	if (state->numbers_are_floats)
	{
		double d = (double)scanner->token.token[0];
		float f = (float)d;
		_This->emit_float(_This, expression, f, scanner->token.source_offset);
	}
	else
	{
		int v = scanner->token.token[0];
		_This->emit_integer(_This, expression, v, scanner->token.source_offset);
	}

	scanner->move_next_token(scanner);
}

_6X_METHODXX(void, parse_string_literal, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct scanner_t *scanner = _This->scanner;

	_This->emit_string(_This, expression, scanner->token.id, scanner->token.source_offset);

	scanner->move_next_token(scanner);
}

_6X_METHODXX(void, parse_primitive, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct scanner_t *scanner = _This->scanner;

	if (scanner->token.type == TOKEN_TYPE_IDENTIFIER)
	{
		_This->parse_identifier(_This, state, expression);
	}
	else if (scanner->token.type == TOKEN_TYPE_NUMBER)
	{
		_This->parse_numeric_literal(_This, state, expression);
	}
	else if (scanner->token.type == TOKEN_TYPE_CHAR)
	{
		_This->parse_char_literal(_This, state, expression);
	}
	else if (scanner->token.type == TOKEN_TYPE_STRING)
	{
		_This->parse_string_literal(_This, state, expression);
	}
	else
	{
		_This->parse_unary_pre_assignment(_This, state, expression);
	}
}

