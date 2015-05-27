/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

EXPRESSION_PARSER_METHODXX(void, parse_brackets, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;

	if (scanner->token.id == TOKEN_OPEN_PAREN)
	{
		do // todo crz: not too sure exactly how the spec wants this to work
		{
			scanner->move_next_token(scanner);
			_This->parse_assignment(_This, state, expression);
		} while (scanner->try_consume_token(scanner, TOKEN_COMMA));

		if (!scanner->try_consume_token(scanner, TOKEN_CLOSE_PAREN))
		{
			THROW(&state->exception, ERROR_6X_CLOSING_PARENTHESIS_EXPECTED);
		}
	}
	else
	{
		_This->parse_primitive(_This, state, expression);
	}
}

