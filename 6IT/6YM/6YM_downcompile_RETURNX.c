/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6EV/6EV_opcodes.h"
#include "6IL/6IL_opcodes.h"

DOWNCOMPILER_METHODX_CONST(int, downcompile_EVAL_RETURN, struct expression_t *expression)
{
	struct instruction_emitter_t *emitter = &_This->machine->instruction_emitter;
	
	if (expression->number_of_terms == 1)
	{
		if (expression->terms[0].type == EVALUATOR_OPCODE_PUSH_INTEGER)
		{
			emitter->emit_instruction(emitter, T6IL_RETURNINT, expression->terms[0].data.as_integer);
			_State->pc += _State->current_instruction_length;
			_State->pc += _This->machine->processor.get_instruction_length(H6VM_OPCODE_RETURN);
			return 1;
		}
	}

	return 0;
}

DOWNCOMPILER_METHOD_CONST(int, downcompile_RETURNEVAL)
{
	struct machine_t *machine = _This->machine;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;

	struct expression_t expression;
	machine->build_expression(machine, &expression, OPCODE_PARM(machine));

	if (expression.number_of_terms == 1)
	{
		if (expression.terms[0].type == EVALUATOR_OPCODE_PUSH_INTEGER)
		{
			emitter->emit_instruction(emitter, T6IL_RETURNINT, expression.terms[0].data.as_integer);
			_State->pc += machine->processor.get_instruction_length(OPCODE(machine));
			return 1;
		}
	}

	return 0;
}

