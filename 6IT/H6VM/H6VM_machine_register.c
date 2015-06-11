/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../H6VM.h"

#include <string.h>

_H6VM_METHODX(struct register_t *, allocate_frame_register, int type)
{
	struct register_t *reg = _REG_RS(_REGS(_This)) + _REG_RSP(_REGS(_This));

	reg->value.as_integer = 0;
	reg->value.type = type;
	reg->is_lua = 0;

	reg->local_idx = reg->idx - FRAME_BASE_REGISTER_INDEX(_This, _FRAME(_This));
	
	_INC_RSP(_REGS(_This));

	return reg;
}

// todo crz: does this get used (without taking a type??)
_H6VM_METHODX(struct register_t *, allocate_static_register, int idx)
{
	if (idx > _This->config.max_number_of_globals)
	{
		_This->die(_This, 0, "too many absolute registers");
	}

	struct register_t *reg = _REG_RS(_REGS(_This)) + idx;

	reg->local_idx = -1;
	reg->value.as_integer = 0;
	reg->value.type = TYPE_INVALID;
	reg->idx = idx;
	reg->is_lua = 0;

	return reg;
}

// todo crz: work out if the type checker needs the type here
_H6VM_METHODX(struct register_t *, allocate_free_static_register, int type)
{
	int idx = _REG_FRP(_REGS(_This));
	if (idx > _This->config.max_number_of_globals)
	{
		_This->die(_This, 0, "too many absolute registers");
	}

	struct register_t *reg = _REG_RS(_REGS(_This)) + idx;

	reg->value.as_integer = 0;
	reg->value.type = type;
	reg->idx = idx;
	reg->local_idx = -1;
	reg->is_lua = 0;

	++_REG_FRP(_REGS(_This));

	return reg;
}

_H6VM_METHODX(struct register_t *, get_static_register, int idx)
{
#ifdef _DEBUG
	if (idx >= _REG_RSP(_REGS(_This)))
	{
		_This->die(_This, 0, "register index out of range");
	}
#endif

	struct register_t *reg = _REG_RS(_REGS(_This)) + idx;

	return reg;
}

_H6VM_METHODX(struct register_metadata_t *, get_register_metadata, int idx)
{
	return _This->register_metadata + idx;
}

_H6VM_METHODX(struct register_t *, find_frame_register, char const *symbol)
{
	int last = _REG_RSP(_REGS(_This)) - 1;
	int first = FRAME_BASE_REGISTER_INDEX(_This, _FRAME(_This));

	for (int i = last; i >= first; i--)
	{
		struct register_t *reg = _H6VM_METHOD_NAME(get_static_register)(_This, i);
		char const *name = _This->register_get_symbol(_This, i);
		if (!strcmp(name, symbol))
		{
			return reg;
		}
	}

	return 0;
}

_H6VM_METHODX(struct register_t *, find_static_register, char const *symbol)
{
	for (int i = 0; i < _REG_FRP(_REGS(_This)); i++)
	{
		struct register_t *reg = _STATIC_REGISTER(_This, i);
		char const *name = _This->register_get_symbol(_This, reg->idx);
		if (!strcmp(name, symbol))
		{
			return reg;
		}
	}

	return 0;
}

_H6VM_METHODX(struct register_t *, find_register, char const *symbol)
{
	struct register_t *reg = _This->find_frame_register(_This, symbol);
	if (!reg)
	{
		reg = _This->find_static_register(_This, symbol);
	}

	return reg;
}

_H6VM_METHODX(char const *, register_get_symbol, int idx)
{
	return _This->register_metadata[idx].symbol;
}

_H6VM_METHODXX(void, register_set_symbol, int idx, char const *symbol)
{
	strcpy(_This->register_metadata[idx].symbol, symbol);
}

