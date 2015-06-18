/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../H6VM.h"

#include <string.h>

_H6VM_METHOD(void, resolve_externals)
{
#ifdef _6IT_SUPPORT_LUA
	_This->resolve_lua_externals(_This);
#endif
}

_H6VM_METHODX(struct callable_unit_t *, find_callable_unit_by_entry_point, int entry)
{
	for (int i = 0; i < _REG_FUP(_REGS(_This)); ++i)
	{
		if (GET_CALLABLE_UNIT(_This, i)->entry_point_program_counter == entry)
		{
			return GET_CALLABLE_UNIT(_This, i);
		}
	}

	return 0;
}

_H6VM_METHODX(struct callable_unit_t *, get_callable_unit, int idx)
{
	return GET_CALLABLE_UNIT(_This, idx);
}

_H6VM_METHODX(struct callable_unit_metadata_t *, get_callable_unit_metadata, int idx)
{
	return _This->callable_unit_metadata + idx;
}

_H6VM_METHODX(struct callable_unit_t *, find_callable_unit, char const *name)
{
	for (int i = 0; i < _REG_FUP(_REGS(_This)); ++i)
	{
		char const *f = _This->get_callable_unit_symbol(_This, i);
		if (!strcmp(name, f))
		{
			return _REG_US(_REGS(_This)) + i;
		}
	}

	return 0;
}

_H6VM_METHODXX(struct callable_unit_t *, add_callable_unit_inner, int return_type, char const *name)
{
	if (_REG_FUP(_REGS(_This)) == _This->config.max_number_of_functions)
	{
		_This->die(_This, 2, "too many functions");
	}

	struct callable_unit_t *cu = _REG_US(_REGS(_This)) + _REG_FUP(_REGS(_This));

	cu->idx = _REG_FUP(_REGS(_This));
	cu->return_type = return_type;
	_This->set_callable_unit_symbol(_This, cu->idx, name);
	cu->number_of_parameters = 0;
	cu->is_vararg = 0;
	cu->is_resolved = 0;
	cu->is_lua = 0;

	++_REG_FUP(_REGS(_This));

	return cu;
}

_H6VM_METHODXX(struct callable_unit_t *, add_callable_unit, int return_type, char const *name)
{
	struct callable_unit_t *cu = _H6VM_METHOD_NAME(add_callable_unit_inner)(_This, return_type, name);

	cu->entry_point_ptr = 0;
	
	if (DEBUG_SHOW_ADDED_SYMBOL(_This))
	{
		_This->printf(_This, "H6VM: added callable unit: %d:%s\n", cu->idx, name);
	}

	return cu;
}

_H6VM_METHODXXXX(struct callable_unit_t *, add_builtin, int return_type, char const *name, int(*entry_point_ptr)(struct machine_t *), int *parms)
{
	struct callable_unit_t *cu = _H6VM_METHOD_NAME(add_callable_unit_inner)(_This, return_type, name);
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

	if (DEBUG_SHOW_ADDED_SYMBOL(_This))
	{
		_This->printf(_This, "H6VM: added builtin: %d:%s\n", cu->idx, name);
	}

	return cu;
}

// todo crz: overflow safety

_H6VM_METHODX(char const *, get_callable_unit_symbol, int idx)
{
	return _This->callable_unit_metadata[idx].symbol;
}

_H6VM_METHODXX(void, set_callable_unit_symbol, int idx, char const *symbol)
{
	strcpy(_This->callable_unit_metadata[idx].symbol, symbol);
}

_H6VM_METHODXX(char const *, get_callable_unit_parameter_symbol, int callable_unit_idx, int idx)
{
	return _This->callable_unit_metadata[callable_unit_idx].parameter_metadata[idx].symbol;
}

_H6VM_METHODXXX(void, set_callable_unit_parameter_symbol, int callable_unit_idx, int idx, char const *symbol)
{
	strcpy(_This->callable_unit_metadata[callable_unit_idx].parameter_metadata[idx].symbol, symbol);
}

_6IT_PRIVATE int _H6VM_METHOD_NAME(vcall)(struct machine_t *machine, struct callable_unit_t const *cu, void *return_value, ...)
{
	va_list args;
	va_start(args, return_value);

	for (int n = 0; n < cu->number_of_parameters; ++n)
	{
		data_type_t type = cu->parameters[n];

		_FRAME_REGISTER(_FRAME(machine), n)->value.type = type;
		switch (type)
		{
		case TYPE_INT:
			_FRAME_REGISTER(_FRAME(machine), n)->value.as_integer = va_arg(args, int);
			_INC_RSP(_REGS(machine));
			break;
		case TYPE_FLOAT:
			_FRAME_REGISTER(_FRAME(machine), n)->value.as_float = (float)va_arg(args, double);
			_INC_RSP(_REGS(machine));
			break;
		case TYPE_CHAR_CONST_PTR:
			_FRAME_REGISTER(_FRAME(machine), n)->value.as_char_const_ptr = va_arg(args, char const *);
			_INC_RSP(_REGS(machine));
			break;
		default:
			return 0;
		}
	}

	va_end(args);

	_SET_PC(_REGS(machine), cu->entry_point_program_counter);
	machine->execute(machine);

	switch (cu->return_type)
	{
	case TYPE_INT:
		*(int *)return_value = POP_INT(machine);
		break;
	case TYPE_FLOAT:
		*(float *)return_value = POP_FLOAT(machine);
		break;
	case TYPE_VOID:
		break;
	default:
		return 0;
	}

	return 1;
}

