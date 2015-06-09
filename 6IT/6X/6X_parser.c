/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6X.h"
#include "6EV/6EV_opcodes.h"

#include <memory.h>

_6X_METHODXX(int, parse_expression, struct expression_parser_state_t *state, struct expression_t *expression)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct expression_emitter_t *emitter = &machine->expression_emitter;

	int vsp = _REG_VSP(_REGS(machine));

	int ex = 0;
	if (ex = CATCH(&state->exception))
	{
		_SET_VSP(_REGS(machine), vsp);
		return state->exception.fault_code;
	}

	data_type_t dt = _This->parse_data_type(_This, state);
	if (dt)
	{
		scanner->check_token_type(scanner, TOKEN_TYPE_IDENTIFIER);

		if (expression->is_global)
		{
			struct register_t *var = machine->allocate_free_static_register(machine, dt);
			state->parser->emit_static_register_allocation(state->parser, expression, var->idx, scanner->token.source_offset);
			machine->register_set_symbol(machine, var->idx, scanner->token.token);
		}
		else
		{
			state->parser->emit_frame_register_allocation(state->parser, expression, dt, scanner->token.source_offset);
			struct register_t *var = machine->allocate_frame_register(machine, dt);
			machine->register_set_symbol(machine, var->idx, scanner->token.token);
			emitter->emit_opcode(emitter, expression, EVALUATOR_OPCODE_DROP, scanner->token.source_offset);
		}
	}

	_This->parse_assignment(_This, state, expression);
	emitter->emit_finished(emitter, expression, scanner->token.source_offset);

	// todo crz: replace return value with an array of errors
	int ok = machine->evaluator.check_expression(machine, state, expression);

	_SET_VSP(_REGS(machine), vsp);

	if ((ITSOK == ok) && !state->is_temporary)
	{
		_REG_FXP(_REGS(machine)) += expression->number_of_terms;
	}

	return ok;
}

