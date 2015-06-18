/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../H6VM.h"

// hack crz: this is all a bit inconsistent and hacky

_H6VM_METHODX(void, bind_compiler, struct compiler_t *compiler)
{
	compiler->machine = _This;
	_This->bind_scanner(_This, compiler->scanner);
}

_H6VM_METHODX(void, bind_scanner, struct scanner_t *scanner)
{
	scanner->machine = _This;
	scanner->environment = _This->environment;
};

_H6VM_METHODX(void, bind_processor, struct processor_t *processor)
{
#ifdef _6IT_DEBU66ER
	_This->execute = processor->execute_debug;
#else
	_This->execute = processor->execute;
#endif
};

_H6VM_METHODX(void, bind_evaluator, struct evaluator_t *evaluator)
{
	_This->evaluate = evaluator->evaluate;
};

_H6VM_METHODX(void, bind_downcompiler, struct downcompiler_t *downcompiler)
{
};

_H6VM_METHODX(void, bind_environment, struct environment_t *environment)
{
	_This->environment = environment;
}

// todo crz: probably switch to using only the environment
#ifdef _6IT_SUPPORT_LUA
_H6VM_METHODX(void, bind_lua, struct lua_State *state)
{
	_This->lua = state;
}
#endif

_6IT_PRIVATE int _H6VM_METHOD_NAME(vcall)(struct machine_t *machine, char const *symbol, void *return_value, ...)
{
	struct callable_unit_t *cu = machine->find_callable_unit(machine, symbol);
	if (!cu)
	{
		return 0;
	}

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

	CALL_FUNCTION(machine, symbol);

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

