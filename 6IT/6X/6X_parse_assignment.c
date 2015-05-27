/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6EV/6EV_opcodes.h"

EXPRESSION_PARSER_METHODXX(void, parse_assignment, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct expression_emitter_t *emitter = &machine->expression_emitter;

	struct token_t save = scanner->token;

	if (scanner->token.type == TOKEN_TYPE_IDENTIFIER)
	{
		struct register_t const *var = machine->find_register(machine, scanner->token.token);
		if (var)
		{
			scanner->move_next_token(scanner);

			int op = EVALUATOR_OPCODE_INVALID;
			int idx = 0;
			if (var->local_idx < 0)
			{
				switch (scanner->token.id)
				{
				case TOKEN_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_SREG; break;
				case TOKEN_PLUS_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_SREG_ADD; break;
				case TOKEN_MINUS_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_SREG_SUB; break;
				case TOKEN_ASTERISK_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_SREG_MUL; break;
				case TOKEN_FORWARD_SLASH_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_SREG_DIV; break;
				case TOKEN_PERCENT_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_SREG_MOD; break;
				}
				idx = var->idx;
			}
			else
			{
				switch (scanner->token.id)
				{
				case TOKEN_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_FREG; break;
				case TOKEN_PLUS_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_FREG_ADD; break;
				case TOKEN_MINUS_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_FREG_SUB; break;
				case TOKEN_ASTERISK_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_FREG_MUL; break;
				case TOKEN_FORWARD_SLASH_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_FREG_DIV; break;
				case TOKEN_PERCENT_EQUALS: op = EVALUATOR_OPCODE_ASSIGN_FREG_MOD; break;
				}
				idx = var->local_idx;
			}

			if (EVALUATOR_OPCODE_INVALID == op)
			{
				scanner->set_token(scanner, save);
				_This->parse_unary_post_assignment(_This, state, expression);
				return;
			}

			scanner->move_next_token(scanner);
			_This->parse_assignment(_This, state, expression);

			if (var->local_idx < 0)
			{
				emitter->emit_static_register_assignment(emitter, expression, var, op, scanner->token.source_offset);
			}
			else
			{
				emitter->emit_frame_register_assignment(emitter, expression, var, op, scanner->token.source_offset);
			}
		}
	}

	_This->parse_unary_post_assignment(_This, state, expression);
}

EXPRESSION_PARSER_METHODXX(void, parse_unary_post_assignment, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct expression_emitter_t *emitter = &machine->expression_emitter;

	struct token_t save = scanner->token;

	if (scanner->token.type == TOKEN_TYPE_IDENTIFIER)
	{
		struct register_t const *var = machine->find_register(machine, scanner->token.token);
		if (var)
		{
			scanner->move_next_token(scanner);

			int op = EVALUATOR_OPCODE_INVALID;
			if (var->local_idx < 0)
			{
				switch (scanner->token.id)
				{
				case TOKEN_PLUSPLUS: op = EVALUATOR_OPCODE_SREG_POSTINC; break;
				case TOKEN_MINUSMINUS: op = EVALUATOR_OPCODE_SREG_POSTDEC; break;
				default:
					scanner->set_token(scanner, save);
					_This->parse_relational(_This, state, expression);
					return;
				}
				emitter->emit_static_register_assignment(emitter, expression, var, op, scanner->token.source_offset);
			}
			else
			{
				switch (scanner->token.id)
				{
				case TOKEN_PLUSPLUS: op = EVALUATOR_OPCODE_FREG_POSTINC; break;
				case TOKEN_MINUSMINUS: op = EVALUATOR_OPCODE_FREG_POSTDEC; break;
				default:
					scanner->set_token(scanner, save);
					_This->parse_relational(_This, state, expression);
					return;
				}
				emitter->emit_frame_register_assignment(emitter, expression, var, op, scanner->token.source_offset);
			}

			scanner->move_next_token(scanner);
		}
	}

	_This->parse_relational(_This, state, expression);
}

EXPRESSION_PARSER_METHODXX(void, parse_unary_pre_assignment, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = state->parser->machine;
	struct scanner_t *scanner = state->parser->scanner;
	struct expression_emitter_t *emitter = &machine->expression_emitter;

	if (scanner->token.id == TOKEN_PLUSPLUS)
	{
		scanner->move_next_token(scanner);

		if (scanner->token.type != TOKEN_TYPE_IDENTIFIER)
		{
		//	machine->die(machine, 0, "identifier expected");
			THROW(&machine->exception, 1);
		}

		struct register_t const *var = machine->find_register(machine, scanner->token.token);
		if (!var)
		{
			THROW(&machine->exception, 1);
//			machine->die(machine, 0, "unknown identifier");
		}

		if (var->local_idx < 0)
		{
			emitter->emit_static_register_assignment(emitter, expression, var, EVALUATOR_OPCODE_SREG_PREINC, scanner->token.source_offset);
		}
		else
		{
			emitter->emit_frame_register_assignment(emitter, expression, var, EVALUATOR_OPCODE_FREG_PREINC, scanner->token.source_offset);
		}

		scanner->move_next_token(scanner);
	}
	else if (scanner->token.id == TOKEN_MINUSMINUS)
	{
		scanner->move_next_token(scanner);

		if (scanner->token.type != TOKEN_TYPE_IDENTIFIER)
		{
			THROW(&machine->exception, 1);
//			machine->die(machine, 0, "identifier expected");
		}

		struct register_t const *var = machine->find_register(machine, scanner->token.token);
		if (!var)
		{
			THROW(&machine->exception, 1);
//			machine->die(machine, 0, "unknown identifier");
		}

		if (var->local_idx < 0)
		{
			emitter->emit_static_register_assignment(emitter, expression, var, EVALUATOR_OPCODE_SREG_PREDEC, scanner->token.source_offset);
		}
		else
		{
			emitter->emit_frame_register_assignment(emitter, expression, var, EVALUATOR_OPCODE_FREG_PREDEC, scanner->token.source_offset);
		}
		scanner->move_next_token(scanner);
	}
}

