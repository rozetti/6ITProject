/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include "6IT_macros.h"

struct fixup_t
{
	int old_pc;
	int new_pc;
};

struct downcompiler_t
{
	DEFINE_COMMON_ATTRIBUTES(downcompiler);

	void _6IT_THISCALL(downcompiler, (*downcompile));
};

struct downcompiler_state_t
{
	struct callable_unit_t *unit;
	opcode_t *p;
	opcode_t *unit_p;
	int pc;
	int callable_unit_PC;
	int downcompiled_callable_unit_PC;
	int delta;
	struct mnemonic_t *current_mnemonic;
	int current_instruction_length;
	opcode_t next_opcode;

	struct fixup_t fixups[1000]; // hack crz
	int number_of_fixups;
};
