/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <string.h>

static void machine_resolve_externals(struct machine_t *machine)
{
#ifdef _6IT_SUPPORT_LUA
	machine->resolve_lua_externals(machine);
#endif
}

static struct callable_unit_t *machine_find_callable_unit_by_entry_point(struct machine_t *machine, int entry)
{
	for (int i = 0; i < _REG_FUP(_REGS(machine)); ++i)
	{
		if (CALLABLE_UNIT(machine, i)->entry_point_program_counter == entry)
		{
			return CALLABLE_UNIT(machine, i);
		}
	}

	return 0;
}

static struct callable_unit_t *machine_get_callable_unit(struct machine_t *machine, int idx)
{
	return CALLABLE_UNIT(machine, idx);
}

static struct callable_unit_metadata_t *machine_get_callable_unit_metadata(struct machine_t *machine, int idx)
{
	return machine->callable_unit_metadata + idx;
}

static struct callable_unit_t *machine_find_callable_unit(struct machine_t *machine, char *name)
{
	for (int i = 0; i < _REG_FUP(_REGS(machine)); ++i)
	{
		char const *f = machine->get_callable_unit_symbol(machine, i);
		if (!strcmp(name, f))
		{
			return _REG_US(_REGS(machine)) + i;
		}
	}

	return 0;
}

static struct callable_unit_t *add_callable_unit_inner(struct machine_t *machine, int return_type, char const *name)
{
	if (_REG_FUP(_REGS(machine)) == machine->config.max_number_of_functions)
	{
		machine->die(machine, 2, "too many functions");
	}

	struct callable_unit_t *cu = _REG_US(_REGS(machine)) + _REG_FUP(_REGS(machine));

	cu->idx = _REG_FUP(_REGS(machine));
	cu->return_type = return_type;
	machine->set_callable_unit_symbol(machine, cu->idx, name);
	cu->number_of_parameters = 0;
	cu->is_vararg = 0;
	cu->is_resolved = 0;
	cu->is_lua = 0;

	++_REG_FUP(_REGS(machine));

	return cu;
}

static struct callable_unit_t *machine_add_callable_unit(struct machine_t *machine, int return_type, char const *name)
{
	struct callable_unit_t *cu = add_callable_unit_inner(machine, return_type, name);

	cu->entry_point_ptr = 0;
	
	if (DEBUG_SHOW_ADDED_SYMBOL(machine))
	{
		machine->printf(machine, "H6VM: added callable unit: %d:%s\n", cu->idx, name);
	}

	return cu;
}

static struct callable_unit_t *machine_add_builtin(struct machine_t *machine, int return_type, char const *name, int(*entry_point_ptr)(struct machine_t *), int *parms)
{
	struct callable_unit_t *cu = add_callable_unit_inner(machine, return_type, name);
	cu->entry_point_ptr = entry_point_ptr;
	cu->is_resolved = 1;

	if (parms)
	{
		int i = 0;
		while (parms[i])
		{
			//struct register_t *p = &(cu->parameters)[i];
			//p->value.type = parms[i];
//			if (p->value.type == TYPE_VARARG)

			cu->parameters[i] = parms[i];
			if (parms[i] == TYPE_VARARG)
			{
				cu->is_vararg = 1;
			}

			++i;
			++cu->number_of_parameters; // todo crz: possibly do away with this
		}
	}

	if (DEBUG_SHOW_ADDED_SYMBOL(machine))
	{
		machine->printf(machine, "H6VM: added builtin: %d:%s\n", cu->idx, name);
	}

	return cu;
}

static char const *machine_get_callable_unit_symbol(struct machine_t *machine, int idx)
{
	return machine->callable_unit_metadata[idx].symbol;
}

static void machine_set_callable_unit_symbol(struct machine_t *machine, int idx, char const *name)
{
	strcpy(machine->callable_unit_metadata[idx].symbol, name);
}

static char const *machine_get_callable_unit_parameter_symbol(struct machine_t *machine, int callable_unit_idx, int idx)
{
	return machine->callable_unit_metadata[callable_unit_idx].parameter_metadata[idx].symbol;
}

static void machine_set_callable_unit_parameter_symbol(struct machine_t *machine, int callable_unit_idx, int idx, char const *name)
{
	strcpy(machine->callable_unit_metadata[callable_unit_idx].parameter_metadata[idx].symbol, name);
}
