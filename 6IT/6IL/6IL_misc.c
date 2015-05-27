/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

_6IT_PRIVATE void _6IT_MACHINECALL(execute_and_reset)
{
	int rsp = _REG_RSP(_REGS(_Machine));
	int pc = _REG_PC(_REGS(_Machine));

	_Machine->execute(_Machine);

	assert(_REG_RSP(_REGS(_Machine)) == rsp);
	_SET_PC(_REGS(_Machine), pc);
}

_6IT_PRIVATE struct mnemonic_t const *get_mnemonic(opcode_t opcode)
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

_6IT_PRIVATE int get_instruction_length(opcode_t opcode)
{
	struct mnemonic_t const *mn = get_mnemonic(OPCODE_INSTRUCTION(opcode));
	return 1 + mn->number_of_operands;
}


