/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../SubC.h"

#include <string.h>

_6IT_PRIVATE char const *type_base_description(data_type_t type)
{
	if (TYPE_VARARG == type)
	{
		return "...";
	}

	switch (type & TYPE_BASE_MASK)
	{
	case TYPE_VOID: return "void";
	case TYPE_CHAR: return "char";
	case TYPE_INT: return "int";
	case TYPE_LONG: return "long";
	case TYPE_FLOAT: return "float";
	case TYPE_DOUBLE: return "double";
	case TYPE_STRUCT: return "struct";
	case TYPE_UNION: return "union";
	}

	return "INVALID";
}

_6IT_PRIVATE char const *type_description(data_type_t type)
{
	static char buff[30];
	buff[0] = 0;

	if (type & TYPE_UNSIGNED)
	{
		strcat(buff, "unsigned ");
	}

	strcat(buff, type_base_description(type));

	if (type & TYPE_CONST)
	{
		strcat(buff, " const");
	}

	if (type & TYPE_POINTER)
	{
		strcat(buff, " *");
	}
	else if (type & TYPE_POINTER2)
	{
		strcat(buff, " **");
	}

	return buff;
}

_6IT_PRIVATE struct machine_register_t *_6IT_MACHINECALLXX(push_local_variable, data_type_t type, char const *name)
{
	struct machine_register_t *var = _Machine->allocate_frame_register(_Machine, type);
	_Machine->register_set_symbol(_Machine, var->idx, name);

	return var;
}

_6IT_PRIVATE struct machine_register_t *_6IT_MACHINECALLXX(add_global_variable, data_type_t type, char const *name)
{
	struct machine_register_t *var = _Machine->allocate_free_static_register(_Machine, type);
	_Machine->register_set_symbol(_Machine, var->idx, name);

	return var;
}
