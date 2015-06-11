/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6IL.h"

_6IT_PRIVATE struct mnemonic_t h6vm_mnemonic[] =
{
	H6VM_OPCODE_STOP, "STOP", H6VM_STACK_NONE, LIGHT, H6VM_INT16, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_INVALID, "INVALID", H6VM_STACK_NONE, LIGHT, 0, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_BREAK, "BREAK", H6VM_STACK_NONE, LIGHT, 0, 0, { 0, 0, 0, 0, 0 },

	// crz: skinny

	T6IL_MOVSI, "MOVSI", H6VM_STACK_NONE, LIGHT, H6VM_REG_SPECIAL, 1, { H6VM_INT32, 0, 0, 0, 0 },
	T6IL_MOVSF, "MOVSF", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_SPECIAL, H6VM_FP32, 0, 0, 0 },
	T6IL_MOVRI, "MOVRI", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_REL, H6VM_INT32, 0, 0, 0 },
	T6IL_MOVRF, "MOVRF", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_REL, H6VM_FP32, 0, 0, 0 },
	T6IL_MOVAI, "MOVAI", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_ABS, H6VM_INT32, 0, 0, 0 },
	T6IL_MOVAF, "MOVAF", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_ABS, H6VM_FP32, 0, 0, 0 },

	T6IL_MOVSR, "MOVSR", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_SPECIAL, H6VM_REG_REL, 0, 0, 0 },
	T6IL_MOVSA, "MOVSA", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_SPECIAL, H6VM_REG_ABS, 0, 0, 0 },
	T6IL_MOVSS, "MOVSS", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_SPECIAL, H6VM_REG_SPECIAL, 0, 0, 0 },

	T6IL_MOVRS, "MOVRS", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_REL, H6VM_REG_SPECIAL, 0, 0, 0 },
	T6IL_MOVAS, "MOVAS", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_REG_ABS, H6VM_REG_SPECIAL, 0, 0, 0 },

	T6IL_INCR, "INCR", H6VM_STACK_NONE, LIGHT, H6VM_REG_REL, 0, { 0, 0, 0, 0, 0 },
	T6IL_INCA, "INCA", H6VM_STACK_NONE, LIGHT, H6VM_REG_ABS, 0, { 0, 0, 0, 0, 0 },
	T6IL_INC, "INC", H6VM_STACK_NONE, LIGHT, H6VM_INT32, 0, { 0, 0, 0, 0, 0 },
	T6IL_DECR, "DECR", H6VM_STACK_NONE, LIGHT, H6VM_REG_REL, 0, { 0, 0, 0, 0, 0 },
	T6IL_DECA, "DECA", H6VM_STACK_NONE, LIGHT, H6VM_REG_ABS, 0, { 0, 0, 0, 0, 0 },
	T6IL_DEC, "DEC", H6VM_STACK_NONE, LIGHT, H6VM_INT32, 0, { 0, 0, 0, 0, 0 },

	T6IL_ADDR, "ADDR", H6VM_STACK_NONE, LIGHT, H6VM_REG_REL, 1, { H6VM_INT32, 0, 0, 0, 0 },
	T6IL_ADDA, "ADDA", H6VM_STACK_NONE, LIGHT, H6VM_REG_ABS, 1, { H6VM_INT32, 0, 0, 0, 0 },

	T6IL_RETURNIIFREQI, "RETURNIIFREQI", H6VM_STACK_NONE, LIGHT, H6VM_REG_REL, 1, { H6VM_INT16_INT16, 0, 0, 0, 0 },
	T6IL_JUMPIFREQI, "JUMPIFREQI", H6VM_STACK_NONE, LIGHT, H6VM_REG_REL, 1, { H6VM_INT16_FPC16, 0, 0, 0, 0 },
	T6IL_JUMPIFRNEI, "JUMPIFRNEI", H6VM_STACK_NONE, LIGHT, H6VM_REG_REL, 1, { H6VM_INT16_FPC16, 0, 0, 0, 0 },

	T6IL_JUMPIFEQI, "JUMPIFEQI", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 1, { H6VM_INT32, 0, 0, 0, 0 },
	T6IL_JUMPIFNEI, "JUMPIFNEI", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 1, { H6VM_INT32, 0, 0, 0, 0 },
	T6IL_JUMPIFEQ, "JUMPIFEQ", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	T6IL_JUMPIFNE, "JUMPIFNE", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	T6IL_JUMPIFLT, "JUMPIFLT", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	T6IL_JUMPIFGT, "JUMPIFGT", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	T6IL_JUMPIFLE, "JUMPIFLE", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	T6IL_JUMPIFGE, "JUMPIFGE", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },

	T6IL_FORRI, "FORRI", LIGHT, H6VM_STACK_NONE, H6VM_REG_REL, 2, { H6VM_INT32, H6VM_IPC, 0, 0, 0 },
	T6IL_NEXTRI, "NEXTRI", LIGHT, H6VM_STACK_NONE, H6VM_REG_REL, 2, { H6VM_INT32, H6VM_INT16_IPC16, 0, 0, 0 },
	T6IL_NEXTRINCI, "NEXTRINCI", LIGHT, H6VM_STACK_NONE, H6VM_REG_REL, 2, { H6VM_INT32, H6VM_IPC, 0, 0, 0 },
	T6IL_NEXTRDECI, "NEXTRDECI", LIGHT, H6VM_STACK_NONE, H6VM_REG_REL, 2, { H6VM_INT32, H6VM_IPC, 0, 0, 0 },

	T6IL_RETURNINT, "RETURNINT16", LIGHT, H6VM_STACK_NONE, H6VM_INT32, 0, { 0, 0, 0, 0, 0 },

	// crz: code

	H6VM_OPCODE_IFS, "IFS", H6VM_STACK_NONE, HEAVY, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_IFB, "IFB", H6VM_STACK_NONE, HEAVY, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_IFSS, "IFSS", H6VM_STACK_NONE, HEAVY, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_IFSB, "IFSB", H6VM_STACK_NONE, HEAVY, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_IFBS, "IFBS", H6VM_STACK_NONE, HEAVY, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_IFBB, "IFBB", H6VM_STACK_NONE, HEAVY, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_IFBB, "IFBB", H6VM_STACK_NONE, HEAVY, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_ELSE, "ELSE", H6VM_STACK_NONE, HEAVY, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },

	H6VM_OPCODE_JUMP, "JUMP", H6VM_STACK_NONE, LIGHT, 0, 1, { H6VM_FPC, 0, 0, 0, 0 },
	H6VM_OPCODE_JUMPIF, "JUMPIF", H6VM_STACK_NONE, LIGHT, 0, 1, { H6VM_FPC, 0, 0, 0, 0 },
	H6VM_OPCODE_LJUMP, "JUMP", H6VM_STACK_NONE, LIGHT, 0, 1, { H6VM_PC, 0, 0, 0, 0 },
	H6VM_OPCODE_LJUMPIF, "JUMPIF", H6VM_STACK_NONE, LIGHT, 0, 2, { H6VM_EXPR, H6VM_PC, 0, 0, 0 },

	H6VM_OPCODE_FORS, "FORS", H6VM_STACK_NONE, LIGHT, H6VM_FPC, 0, { 0, 0 ,0, 0, 0 },
	H6VM_OPCODE_FORB, "FORB", LIGHT, H6VM_STACK_NONE, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_NEXT, "NEXT", LIGHT, H6VM_STACK_NONE, H6VM_FPC, 0, { 0, 0, 0, 0, 0 },

	H6VM_OPCODE_WHILES, "WHILES", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_FPC, 0, 0, 0, 0 },
	H6VM_OPCODE_WHILEB, "WHILEB", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_FPC, 0, 0, 0, 0 },

	H6VM_OPCODE_DOS, "DOS", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_PC, 0, 0, 0, 0 },
	H6VM_OPCODE_DOB, "DOB", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_PC, 0, 0, 0, 0 },

	H6VM_OPCODE_LOOP_BREAK, "LOOPBREAK", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_PC, 0, 0, 0, 0 },
	H6VM_OPCODE_LOOP_CONTINUE, "LOOPCONT", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_PC, 0, 0, 0, 0 },

	H6VM_OPCODE_EVAL, "EVAL", LIGHT, H6VM_STACK_PUSH1, H6VM_EXPR, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_EXEC, "EXEC", LIGHT, H6VM_STACK_NONE, H6VM_EXPR, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_CONDITION, "CONDITION", LIGHT, H6VM_STACK_NONE, H6VM_EXPR, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_DROP, "DROP", LIGHT, H6VM_STACK_POP1, 0, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_ASSERT, "ASSERT", LIGHT, H6VM_STACK_NONE, 0, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_RETURN_POP, "RETURNPOP", LIGHT, H6VM_STACK_POP1, 0, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_RETURN_FREG, "RETURNREL", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_REG_REL, 0, 0, 0, 0 },
	H6VM_OPCODE_RETURN_SREG, "RETURNABS", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_REG_ABS, 0, 0, 0, 0 },
	H6VM_OPCODE_RETURN, "RETURN", LIGHT, H6VM_STACK_NONE, 0, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_RETURN_INTEGER, "RETURNI", LIGHT, H6VM_STACK_NONE, H6VM_INT16, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_RETURN_FLOAT, "RETURNF", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_FP32, 0, 0, 0, 0 },
	H6VM_OPCODE_RETURN_STRING, "RETURNS", LIGHT, H6VM_STACK_NONE, 0, 1, { H6VM_STR, 0, 0, 0, 0 },
	H6VM_OPCODE_RETURN_EVALUATION, "RETURNEVAL", LIGHT, H6VM_STACK_NONE, H6VM_EXPR, 0, { 0, 0, 0, 0, 0 },

	// crz: register stack operations

	H6VM_OPCODE_PUSHRSP, "PUSHRSP", LIGHT, H6VM_STACK_NONE, 0, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_POPRSP, "POPRSP", LIGHT, H6VM_STACK_NONE, 0, 0, { 0, 0, 0, 0, 0 },
	H6VM_OPCODE_LOADRSP, "LOADRSP", LIGHT, H6VM_STACK_NONE, H6VM_INT32, 0, { 0, 0, 0, 0, 0 },

	H6VM_OPCODE_NOP, "NOP", LIGHT, H6VM_STACK_NONE, 0, 0, { 0, 0, 0, 0, 0 },

	0, 0, LIGHT, 0, 0, 0, { 0, 0, 0, 0, 0 }
};

_6IT_PRIVATE struct mnemonic_t const *_6IL_get_mnemonic(opcode_t opcode)
{
	int i = 0;

	while (h6vm_mnemonic[i].mnemonic)
	{
		if (h6vm_mnemonic[i].opcode == opcode)
		{
			return &h6vm_mnemonic[i];
		}

		++i;
	}

	return 0;
}

_6IT_PRIVATE int _6IL_get_instruction_length(opcode_t opcode)
{
	struct mnemonic_t const *mn = _6IL_get_mnemonic(OPCODE_INSTRUCTION(opcode));
	return 1 + mn->number_of_operands;
}

