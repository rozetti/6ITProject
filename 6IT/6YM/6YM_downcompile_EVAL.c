/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6EV/6EV_opcodes.h"
#include "6IL/6IL_opcodes.h"

DOWNCOMPILER_METHOD_CONST(int, downcompile_EVAL)
{
	struct machine_t *machine = _This->machine;

	struct expression_t expression;
	machine->build_expression(machine, &expression, OPCODE_PARM(machine));

	switch (OPCODE_INSTRUCTION(_State->next_opcode))
	{
	case H6VM_OPCODE_RETURN:
		if (downcompile_EVAL_RETURN(_This, _State, &expression)) return 1;
		break;
	}

	//if (expression.number_of_terms == 1)
	//{
	//	if (expression.terms[0].type == EVALUATOR_OPCODE_FREG_PREINC)
	//	{
	//		state->machine->emit_instruction(state->machine, T6IL_INCR, expression.terms[0].data.as_integer);
	//		state->pc += state->machine->processor.get_instruction_length(OPCODE);
	//		return 1;
	//	}

	//	if (expression.terms[0].type == EVALUATOR_OPCODE_PUSH_INTEGER)
	//	{
	//		state->machine->emit_instruction(state->machine, EVALUATOR_OPCODE_PUSH_INTEGER, expression.terms[0].data.as_integer);
	//		state->pc += state->machine->processor.get_instruction_length(OPCODE);
	//		return 1;
	//	}
	//}

	return 0;
}

