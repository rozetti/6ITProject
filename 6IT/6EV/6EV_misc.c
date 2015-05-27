/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

_6IT_PRIVATE struct mnemonic_t const *get_mnemonic(opcode_t opcode)
{
	int i = 0;

	while (evaluator_mnemonics[i].mnemonic)
	{
		if (evaluator_mnemonics[i].opcode == opcode)
		{
			return &evaluator_mnemonics[i];
		}

		++i;
	}

	return 0;
}


