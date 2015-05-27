/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <string.h>

struct register_t *machine_allocate_frame_register(struct machine_t *machine, int type)
{
	struct register_t *reg = _REG_RS(_REGS(machine)) + _REG_RSP(_REGS(machine));

	reg->value.as_integer = 0;
	reg->value.type = type;
	reg->is_lua = 0;

	reg->local_idx = reg->idx - FRAME_BASE_REGISTER_INDEX(machine, _FRAME(machine));
	
	_INC_RSP(_REGS(machine));

	return reg;
}

struct register_t *machine_allocate_static_register(struct machine_t *machine, int idx)
{
	if (idx > machine->config.max_number_of_globals)
	{
		machine->die(machine, 0, "too many absolute registers");
	}

	struct register_t *reg = _REG_RS(_REGS(machine)) + idx;

	reg->local_idx = -1;
	reg->value.as_integer = 0;
	reg->value.type = TYPE_INVALID;
	reg->idx = idx;
	reg->is_lua = 0;

	return reg;
}

// todo crz: work out if the type checker needs the type here
struct register_t *machine_allocate_free_static_register(struct machine_t *machine, int type)
{
	int idx = _REG_FRP(_REGS(machine));
	if (idx > machine->config.max_number_of_globals)
	{
		machine->die(machine, 0, "too many absolute registers");
	}

	struct register_t *reg = _REG_RS(_REGS(machine)) + idx;

	reg->value.as_integer = 0;
	reg->value.type = type;
	reg->idx = idx;
	reg->local_idx = -1;
	reg->is_lua = 0;

	++_REG_FRP(_REGS(machine));

	return reg;
}

#include <string.h>

// todo crz: replace these with macros
struct register_t *machine_get_register_absolute(struct machine_t *machine, int idx)
{
#ifdef _DEBUG
	if (idx >= _REG_RSP(_REGS(machine)))
	{
		machine->die(machine, 0, "register index out of range");
	}
#endif

	struct register_t *reg = _REG_RS(_REGS(machine)) + idx;

	return reg;
}

struct register_metadata_t *machine_get_register_metadata(struct machine_t *machine, int idx)
{
	return machine->register_metadata + idx;
}

struct register_t *machine_find_frame_register(struct machine_t *machine, char const *symbol)
{
	int last = _REG_RSP(_REGS(machine)) - 1;
	int first = FRAME_BASE_REGISTER_INDEX(machine, _FRAME(machine));

	for (int i = last; i >= first; i--)
	{
		struct register_t *reg = machine_get_register_absolute(machine, i);
		char const *name = machine->register_get_symbol(machine, i);
		if (!strcmp(name, symbol))
		{
			return reg;
		}
	}

	return 0;
}

struct register_t *machine_find_static_register(struct machine_t *machine, char const *symbol)
{
	for (int i = 0; i < _REG_FRP(_REGS(machine)); i++)
	{
		struct register_t *reg = _STATIC_REGISTER(machine, i);
		char const *name = machine->register_get_symbol(machine, reg->idx);
		if (!strcmp(name, symbol))
		{
			return reg;
		}
	}

	return 0;
}

struct register_t *machine_find_register(struct machine_t *machine, char const *symbol)
{
	struct register_t *reg = machine->find_frame_register(machine, symbol);
	if (!reg)
	{
		reg = machine->find_static_register(machine, symbol);
	}

	return reg;
}

char const *machine_register_get_symbol(struct machine_t *machine, int idx)
{
	return machine->register_metadata[idx].symbol;
}

void machine_register_set_symbol(struct machine_t *machine, int idx, char const *name)
{
	strcpy(machine->register_metadata[idx].symbol, name);
}

