/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "SubC.h"

#include "6IL/6IL_opcodes.h"

_SUBC_METHOD_CONST(void, compile_assert)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;

	scanner->consume_token(scanner, TOKEN_OPEN_PAREN);

	struct expression_t expression;
	init_new_expression(machine, &expression);

	int ok = _State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &expression);
	if (ITSOK != ok)
	{
		THROWM(&_This->machine->exception, ok, _State->expression_parser_state->message);
	}

	scanner->consume_token(scanner, TOKEN_CLOSE_PAREN);

	emitter->emit_instruction(emitter, H6VM_OPCODE_CONDITION, expression.first_term_index);
	emitter->emit_instruction(emitter, H6VM_OPCODE_ASSERT, 0);
}

