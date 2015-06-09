/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6YM.h"
#include "6EV/6EV_opcodes.h"
#include "6IL/6IL_opcodes.h"

_6YM_METHODX_CONST(int, downcompile_IFREQI, struct expression_t *expression)
{
	struct instruction_emitter_t *emitter = &_This->machine->instruction_emitter;
	
	int jump_pc = OPCODE_IMMEDIATE(_State->next_opcode);

	++jump_pc; // crz: skip the redundant LOADRSP (as there is no expression evaluation now)
	
	emitter->emit_instruction(emitter, T6IL_JUMPIFRNEI, expression->terms[0].data.as_integer);
	emitter->emit_integer(emitter, MAKE_OPERAND(expression->terms[1].data.as_integer, jump_pc));

	_State->pc += _State->current_instruction_length;
	_State->pc += _This->machine->processor.get_instruction_length(H6VM_OPCODE_IFS);

	return 1;
}

_6YM_METHODX_CONST(int, downcompile_CONDITION_IFX, struct expression_t *expression)
{
	// crz: if register == integer
	if (expression->number_of_terms == 3 &&
		expression->terms[0].type == EVALUATOR_OPCODE_PUSH_FREG &&
		expression->terms[1].type == EVALUATOR_OPCODE_PUSH_INTEGER &&
		expression->terms[2].type == EVALUATOR_OPCODE_EQ)
	{
		return _6YM_METHOD_NAME(downcompile_IFREQI)(_This, _State, expression);
	}

	return 0;
}

