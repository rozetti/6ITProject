/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6IL/6IL_opcodes.h"

// crz: this is probably slower than leaving it alone,
//int _6YM_downcompile_LOADRSP(struct downcompiler_state_t *state)
//{
//		MAKE_IP(state->p);
//
//	state->machine->emit_instruction(state->machine, T6IL_MOVSI, H6VM_REG_RSP);
//	state->machine->emit_integer(state->machine, OPERAND1);
//
//	state->pc += state->machine->processor.get_instruction_length(OPCODE);
//	return 1;
//}
//
