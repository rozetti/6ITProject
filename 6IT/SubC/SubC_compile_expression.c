/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include "SubC.h"
#include "6IL/6IL_opcodes.h"
#include "6EV/6EV_opcodes.h"

_SUBC_METHOD_CONST(void, compile_global_expression)
{
	struct expression_t expression;
	init_new_expression(_This->machine, &expression);
	expression.is_global = 1;
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &expression);

	_State->global_expressions[_State->number_of_global_expressions++] = expression.first_term_index;
}

_SUBC_METHOD_CONST(void, emit_static_initialiser)
{
	struct instruction_emitter_t *emitter = &_This->machine->instruction_emitter;

	struct callable_unit_t *cu = _This->machine->add_callable_unit(_This->machine, TYPE_VOID, "static");

	if (_SUBC_REPORT_PROGRESS(_This))
	{
		_This->machine->printf(_This->machine, "SubC: creating static initialiser\n");
	}

	cu->number_of_parameters = 0;
	cu->entry_point_program_counter = _REG_FCP(_REGS(_This->machine));
	cu->is_resolved = 1;

	for (int i = 0; i < _State->number_of_global_expressions; ++i)
	{
		emitter->emit_instruction(emitter, H6VM_OPCODE_EVAL, _State->global_expressions[i]);
	}

	emitter->emit_instruction(emitter, H6VM_OPCODE_RETURN, 0);

	cu->exit_point_program_counter = _REG_FCP(_REGS(_This->machine)) - 1;
}

_SUBC_METHOD_CONST(void, compile_expression)
{
	struct instruction_emitter_t *emitter = &_This->machine->instruction_emitter;

	struct expression_t expression;
	init_new_expression(_This->machine, &expression);
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &expression);

	emitter->emit_instruction(emitter, H6VM_OPCODE_EXEC, expression.first_term_index);
}

