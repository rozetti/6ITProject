/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6X.h"
#include "6EV/6EV_opcodes.h" 

_6X_METHODXX(void, parse_additive, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct expression_emitter_t *emitter = &machine->expression_emitter;
	struct scanner_t *scanner = _This->scanner;

	_This->parse_multiplicative(_This, state, expression);

	int op = 0;
	while (1)
	{
		switch (_This->scanner->token.id)
		{
		case TOKEN_PLUS: op = EVALUATOR_OPCODE_ADD; break;
		case TOKEN_MINUS: op = EVALUATOR_OPCODE_SUB; break;
		default: return;
		}

		_This->scanner->move_next_token(_This->scanner);
		_This->parse_multiplicative(_This, state, expression);

		if (state->use_immediates)
		{
			struct expression_term_t *final = machine->get_final_expression_term(_This->machine, expression);
			if (final->type == EVALUATOR_OPCODE_PUSH_INTEGER)
			{
				switch (op)
				{
				case EVALUATOR_OPCODE_ADD:
					final->type = EVALUATOR_OPCODE_IADD;
					break;
				case EVALUATOR_OPCODE_SUB:
					final->type = EVALUATOR_OPCODE_ISUB;
					break;
				default:
					emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
				}
			}
			else if (final->type == EVALUATOR_OPCODE_PUSH_FLOAT)
			{
				switch (op)
				{
				case EVALUATOR_OPCODE_ADD:
					final->type = EVALUATOR_OPCODE_IADD_FP32;
					break;
				case EVALUATOR_OPCODE_SUB:
					final->type = EVALUATOR_OPCODE_ISUB_FP32;
					break;
				default:
					emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
				}
			}
			else
			{
				emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
			}
		}
		else
		{
			emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
		}
	}
}

_6X_METHODXX(void, parse_multiplicative, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct expression_emitter_t *emitter = &machine->expression_emitter;
	struct scanner_t *scanner = _This->scanner;

	_This->parse_unary(_This, state, expression);

	int op = 0;
	while (1)
	{
		switch (scanner->token.id)
		{
		case TOKEN_ASTERISK: op = EVALUATOR_OPCODE_MUL; break;
		case TOKEN_FORWARD_SLASH: op = EVALUATOR_OPCODE_DIV; break;
		case TOKEN_PERCENT: op = EVALUATOR_OPCODE_MOD; break;
		default: return;
		}

		scanner->move_next_token(scanner);
		_This->parse_unary(_This, state, expression);

		if (state->use_immediates)
		{
			struct expression_term_t *final = machine->get_final_expression_term(machine, expression);

			if (final->type == EVALUATOR_OPCODE_PUSH_INTEGER)
			{
				switch (op)
				{
				case EVALUATOR_OPCODE_MUL:
					final->type = EVALUATOR_OPCODE_IMUL;
					break;
				case EVALUATOR_OPCODE_DIV:
					final->type = EVALUATOR_OPCODE_IDIV;
					break;
				case EVALUATOR_OPCODE_MOD:
					final->type = EVALUATOR_OPCODE_IMOD;
					break;
				default:
					emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
				}
			}
			else if (final->type == EVALUATOR_OPCODE_PUSH_FLOAT)
			{
				switch (op)
				{
				case EVALUATOR_OPCODE_MUL:
					final->type = EVALUATOR_OPCODE_IMUL_FP32;
					break;
				case EVALUATOR_OPCODE_DIV:
					final->type = EVALUATOR_OPCODE_IDIV_FP32;
					break;
				default:
					emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
				}
			}
			else
			{
				emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
			}
		}
		else
		{
			emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
		}
	}
}

_6X_METHODXXX(int, try_parse_cast, struct expression_parser_state_t *state, struct expression_t *expression, int *op)
{
	struct scanner_t *scanner = _This->scanner;

	struct token_t save = scanner->token;
	struct scanner_state_t save_state = scanner->state;

	if (!scanner->try_consume_symbol(scanner, TOKEN_OPEN_PAREN)) return 0;

	data_type_t type = _This->parse_data_type(_This, state);
	if (TYPE_INVALID == type)
	{
		scanner->state = save_state;
		scanner->token = save;

		return 0;
	}

	scanner->consume_symbol(scanner, TOKEN_CLOSE_PAREN);

	switch (type)
	{
	case TYPE_INT:
		*op = EVALUATOR_OPCODE_CAST_INT;
		break;
	case TYPE_FLOAT:
		*op = EVALUATOR_OPCODE_CAST_FP32;
		break;
	}

	return 1;
}

_6X_METHODXX(void, parse_unary, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct expression_emitter_t *emitter = &machine->expression_emitter;
	struct scanner_t *scanner = _This->scanner;

	int op;
	if (_This->try_parse_cast(_This, state, expression, &op))
	{
		_This->parse_brackets(_This, state, expression);
		emitter->emit_unary_operator(emitter, expression, op, scanner->token.source_offset);
	}
	else
	{
		switch (scanner->token.id)
		{
		case TOKEN_BANG: op = EVALUATOR_OPCODE_LNOT; break;
		case TOKEN_MINUS: op = EVALUATOR_OPCODE_MINUS; break;
		case TOKEN_PLUS: op = EVALUATOR_OPCODE_PLUS; break;
		default:
			_This->parse_brackets(_This, state, expression);
			return;
		}

		scanner->move_next_token(scanner);
		_This->parse_brackets(_This, state, expression);
		emitter->emit_unary_operator(emitter, expression, op, scanner->token.source_offset);
	}
}

