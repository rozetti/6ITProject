/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6X.h"
#include "6EV/6EV_opcodes.h"

_6X_METHODXX(void, parse_relational, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct expression_emitter_t *emitter = &machine->expression_emitter;

	_This->parse_additive(_This, state, expression);

	int op = 0;
	switch (scanner->token.id)
	{
	case TOKEN_LESS_THAN: op = EVALUATOR_OPCODE_LT; break;
	case TOKEN_LESS_THAN_OR_EQUAL: op = EVALUATOR_OPCODE_LE; break;
	case TOKEN_GREATER_THAN: op = EVALUATOR_OPCODE_GT; break;
	case TOKEN_GREATER_THAN_OR_EQUAL: op = EVALUATOR_OPCODE_GE; break;
	case TOKEN_EQUALS_EQUALS: op = EVALUATOR_OPCODE_EQ; break;
	case TOKEN_BANG_EQUALS: op = EVALUATOR_OPCODE_NE; break;
	default: return;
	}

	scanner->move_next_token(scanner);
	_This->parse_additive(_This, state, expression);
	emitter->emit_binary_operator(emitter, expression, op, scanner->token.source_offset);
}

