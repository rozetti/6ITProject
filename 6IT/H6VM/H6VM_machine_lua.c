/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#ifdef _6IT_SUPPORT_LUA

#include "6IT.h"
#include <string.h>

void machine_push_to_lua(struct machine_t *machine);

static void machine_setup_lua(struct machine_t *machine)
{
	if (!machine->lua) return;

	// todo crz: before and after check to grab only stuff defined in the .lua file

	lua_pushglobaltable(machine->lua);
	lua_pushnil(machine->lua);

	while (lua_next(machine->lua, -2)) 
	{
		const char* name = lua_tostring(machine->lua, -2);

		//if (lua_isnumber(machine->lua, -1))
		//{
		//	struct register_t *var = machine->allocate_free_static_register(machine, TYPE_FLOAT);
		//	machine->register_set_symbol(machine, var->idx, name);
		//	var->value.type = TYPE_FLOAT;
		//	var->value.as_float = (float)lua_tonumber(machine->lua, -1);
		//	var->is_lua = 1;
		//}

		//if (lua_isfunction(machine->lua, -1))
		//{
		//	struct callable_unit_t *function = machine->add_callable_unit(machine, TYPE_FLOAT, name);
		//	function->number_of_parameters = 0;
		//	function->is_lua = 1;
		//	function->entry_point_ptr = 0;
		//	function->entry_point_program_counter = 0;
		//	function->exit_point_program_counter = 0;
		//	function->is_vararg = 1;
		//}

		lua_pop(machine->lua, 1);
	}

	lua_pop(machine->lua, 1);
}

static void machine_pull_from_lua(struct machine_t *machine)
{
	lua_State *L = machine->lua;

	for (int i = 0; i < machine->config.max_number_of_globals; ++i)
	{
		struct register_t *var = _STATIC_REGISTER(machine, i);

		if (var->is_lua)
		{
			char const *symbol = machine->register_get_symbol(machine, var->idx);

			lua_getglobal(L, symbol);
			assert(lua_isnumber(L, -1));
			lua_Number v = lua_tonumber(machine->lua, -1);

			switch (var->value.type)
			{
			case TYPE_INT:
				var->value.as_integer = (int)v;
				break;
			case TYPE_FLOAT:
				var->value.as_float = (float)v;
				break;
			default:
				strcpy(machine->exception.message, "Lua excternal has incompatible type");
				THROW(&machine->exception, 1);
			}
		}
	}
}

static void machine_push_to_lua(struct machine_t *machine)
{
	for (int i = 0; i < machine->config.max_number_of_globals; ++i)
	{
		struct register_t *var = _STATIC_REGISTER(machine, i);

		if (var->is_lua)
		{
			char const *symbol = machine->register_get_symbol(machine, var->idx);

			lua_pushnumber(machine->lua, var->value.as_float);
			lua_setglobal(machine->lua, symbol);
		}
	}
}

static int machine_lua_has_variable(struct machine_t *machine, char const *symbol)
{
	int yes = 0;

#ifdef _6IT_SUPPORT_LUA
	lua_getglobal(machine->lua, symbol);

	if (lua_isnumber(machine->lua, -1)) yes = 1;

	lua_pop(machine->lua, 1);
#endif

	return yes;
}

static int machine_lua_has_function(struct machine_t *machine, char const *symbol)
{
	int yes = 0;

#ifdef _6IT_SUPPORT_LUA
	lua_getglobal(machine->lua, symbol);

	if (lua_isfunction(machine->lua, -1)) yes = 1;

	lua_pop(machine->lua, 1);
#endif

	return yes;
}

//#ifdef _6IT_SUPPORT_LUA
//if (!reg)
//{
//	if (machine_lua_has_variable(machine, symbol))
//	{
//		struct register_t *var = machine->allocate_free_static_register(machine, dt);
//		expression_emit_absolute_register_allocation(machine, expression, var->idx, scanner->token.source_offset);
//		machine->register_set_symbol(machine, var->idx, scanner->token.token);
//	}
//}
//#endif

static void machine_resolve_lua_externals(struct machine_t *machine)
{
	if (!machine->lua) return;

	for (int i = 0; i < _REG_FUP(_REGS(machine)); ++i)
	{
		struct callable_unit_t *cu = CALLABLE_UNIT(machine, i);

		if (!cu->is_resolved)
		{
			char const *symbol = machine->get_callable_unit_symbol(machine, i);

			if (machine_lua_has_function(machine, symbol))
			{
				cu->is_resolved = 1;
				cu->is_lua = 1;
			}
		}
	}

	for (int i = 0; i < machine->config.max_number_of_globals; ++i)
	{
		struct register_t *var = _STATIC_REGISTER(machine, i);

		if (var->is_external)
		{
			char const *symbol = machine->register_get_symbol(machine, var->idx);
			if (machine_lua_has_variable(machine, symbol))
			{
				var->is_lua = 1;
			}
		}
	}

	machine->pull_from_lua(machine);
}

#endif