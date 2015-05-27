/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

COMPILER_METHOD_CONST(void, compile_program)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct expression_parser_state_t *expression_parser_state = _State->expression_parser_state;
	struct expression_parser_t *expression_parser = expression_parser_state->parser;

	_REG_RSP(_REGS(machine)) = machine->config.max_number_of_globals;

	struct token_t save = scanner->token;

	while ((scanner->token.type != TOKEN_TYPE_EOF) && (!_State->fatal))
	{
		struct token_t before = scanner->token;
		struct scanner_state_t scanner_save = scanner->state;

		if (scanner->try_consume_string(scanner, "extern"))
		{
			data_type_t dt = expression_parser->parse_data_type(expression_parser, expression_parser_state);

			struct register_t *var = machine->find_register(machine, scanner->token.token);
			if (!var)
			{
				var = machine->allocate_free_static_register(machine, dt);
				machine->register_set_symbol(machine, var->idx, scanner->token.token);
				var->is_external = 1;
			}

			scanner->move_next_token(scanner);
			scanner->consume_token(scanner, TOKEN_SEMI_COLON);
			continue;
		}

		data_type_t dt = expression_parser->parse_data_type(expression_parser, expression_parser_state);
		if (dt)
		{
			scanner->move_next_token(scanner);
			if (scanner->is_token(scanner, TOKEN_OPEN_PAREN))
			{
				scanner->state = scanner_save;
				scanner->token = before;
				_State->base_RSP = _REG_RSP(_REGS(machine));
				compile_function(_This, _State);
			}
			else
			{
				scanner->state = scanner_save;
				scanner->token = before;
				compile_global_expression(_This, _State);
			}

			continue;
		}
		else
		{
			if (scanner->is_token(scanner, TOKEN_SEMI_COLON))
			{
				scanner->move_next_token(scanner);
			}
			else if (scanner->is_token(scanner, TOKEN_HASH))
			{
				scanner->move_next_line(scanner);
			}
		}

		if (scanner->token.type == TOKEN_TYPE_EOF) // hack crz
		{
			break;
		}
	}

	emit_static_initialiser(_This, _State);

	machine->resolve_externals(machine);

	scanner->set_token(scanner, save);
}
