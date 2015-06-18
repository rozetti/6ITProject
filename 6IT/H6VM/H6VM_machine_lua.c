/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#ifdef _6IT_SUPPORT_LUA

#include "../H6VM.h"
#include <string.h>

//void machine_push_to_lua(struct machine_t *machine);

_H6VM_METHOD(void, setup_lua)
{
	if (!_This->lua) return;

	// todo crz: before and after check to grab only stuff defined in the .lua file

	lua_pushglobaltable(_This->lua);
	lua_pushnil(_This->lua);

	while (lua_next(_This->lua, -2))
	{
		const char* name = lua_tostring(_This->lua, -2);

		// todo crz: put this back, or get rid of

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

		lua_pop(_This->lua, 1);
	}

	lua_pop(_This->lua, 1);
}

_H6VM_METHOD(void, pull_from_lua)
{
	lua_State *L = _This->lua;

	for (int i = 0; i < _This->config.max_number_of_globals; ++i)
	{
		struct register_t *var = _STATIC_REGISTER(_This, i);

		if (var->is_lua)
		{
			char const *symbol = _This->register_get_symbol(_This, var->idx);

			lua_getglobal(L, symbol);
			assert(lua_isnumber(L, -1));
			lua_Number v = lua_tonumber(_This->lua, -1);

			switch (var->value.type)
			{
			case TYPE_INT:
				var->value.as_integer = (int)v;
				break;
			case TYPE_FLOAT:
				var->value.as_float = (float)v;
				break;
			default:
				strcpy(_This->exception.message, "Lua excternal has incompatible type");
				THROW(&_This->exception, 1);
			}
		}
	}
}

_H6VM_METHOD(void, push_to_lua)
{
	for (int i = 0; i < _This->config.max_number_of_globals; ++i)
	{
		struct register_t *var = _STATIC_REGISTER(_This, i);

		if (var->is_lua)
		{
			char const *symbol = _This->register_get_symbol(_This, var->idx);

			lua_pushnumber(_This->lua, var->value.as_float);
			lua_setglobal(_This->lua, symbol);
		}
	}
}

_H6VM_METHODX(int, lua_has_variable, char const *symbol)
{
	int yes = 0;

#ifdef _6IT_SUPPORT_LUA
	lua_getglobal(_This->lua, symbol);

	if (lua_isnumber(_This->lua, -1)) yes = 1;

	lua_pop(_This->lua, 1);
#endif

	return yes;
}

_H6VM_METHODX(int, lua_has_function, char const *symbol)
{
	int yes = 0;

#ifdef _6IT_SUPPORT_LUA
	lua_getglobal(_This->lua, symbol);

	if (lua_isfunction(_This->lua, -1)) yes = 1;

	lua_pop(_This->lua, 1);
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

_H6VM_METHOD(void, resolve_lua_externals)
{
	if (!_This->lua) return;

	for (int i = 0; i < _REG_FUP(_REGS(_This)); ++i)
	{
		struct callable_unit_t *cu = GET_CALLABLE_UNIT(_This, i);

		if (!cu->is_resolved)
		{
			char const *symbol = _This->get_callable_unit_symbol(_This, i);

			if (_H6VM_METHOD_NAME(lua_has_function)(_This, symbol))
			{
				cu->is_resolved = 1;
				cu->is_lua = 1;
			}
		}
	}

	for (int i = 0; i < _This->config.max_number_of_globals; ++i)
	{
		struct register_t *var = _STATIC_REGISTER(_This, i);

		if (var->is_external)
		{
			char const *symbol = _This->register_get_symbol(_This, var->idx);
			if (_H6VM_METHOD_NAME(lua_has_variable)(_This, symbol))
			{
				var->is_lua = 1;
			}
		}
	}

	_This->pull_from_lua(_This);
}

#endif