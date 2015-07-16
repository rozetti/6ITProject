/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6X.h"

#include <stdio.h>

_6X_METHODXX(void, parse_identifier, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;

	struct callable_unit_t *function = machine->find_callable_unit(machine, scanner->token.token);
	if (function)
	{
		scanner->move_next_token(scanner);

		scanner->consume_symbol(scanner, TOKEN_OPEN_PAREN);

		int number_of_args = 0;
		if (!scanner->try_consume_symbol(scanner, TOKEN_CLOSE_PAREN))
		{
			do
			{
				_This->parse_assignment(_This, state, expression);
				++number_of_args;
				if (number_of_args > function->number_of_parameters)
				{
					if (!function->is_vararg)
					{
						THROW(&state->parser->machine->exception, 1);
					}
				}
			} while (scanner->try_consume_token(scanner, TOKEN_COMMA));

			scanner->consume_token(scanner, TOKEN_CLOSE_PAREN);
		}

		_This->emit_call(_This, expression, function, number_of_args, scanner->token.source_offset);
		return;
	}

	struct machine_register_t const *var = machine->find_register(machine, scanner->token.token);
	if (var)
	{
		_This->emit_variable(_This, expression, var, scanner->token.source_offset);
		scanner->move_next_token(scanner);
		return;
	}

//	machine->print_state(machine);

	sprintf(state->message, "undefined identifier: %s", scanner->token.token);
	THROW(&state->exception, ERROR_6X_UNDEFINED_IDENTIFIER);
}

