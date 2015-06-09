/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6EV.h"

_6IT_PRIVATE void _6IT_MACHINECALLX(_6EV_METHOD_NAME(print_expression_term), struct expression_term_t *term)
{
	struct callable_unit_t *func;
	char const *name;

	struct mnemonic_t const *mnemonic = _Machine->evaluator.get_mnemonic(term->type);
	if (mnemonic)
	{
		_Machine->printf(_Machine, "%.2d:%s", term->type, mnemonic->mnemonic);
	}

	switch (term->type)
	{
	case EVALUATOR_OPCODE_INVALID:
		THROW(&_Machine->exception, 1);
		break;
	case EVALUATOR_OPCODE_PUSH_FREG:
		_Machine->printf(_Machine, "[+%d]", term->data.as_integer);
		break;

	case EVALUATOR_OPCODE_PUSH_SREG:
		_Machine->printf(_Machine, "[%d]", term->data.as_integer);
		break;

	case EVALUATOR_OPCODE_PUSH_INTEGER:
	case EVALUATOR_OPCODE_ALLOC_FREG:
	case EVALUATOR_OPCODE_ALLOC_SREG:
		_Machine->printf(_Machine, ":%d", term->data.as_integer);
		break;

	case EVALUATOR_OPCODE_PUSH_FLOAT:
		_Machine->printf(_Machine, ":%f", term->data.as_float);
		break;

	case EVALUATOR_OPCODE_PUSH_STRING:
		_Machine->printf(_Machine, ":%d", term->data.as_integer);
		break;

	case EVALUATOR_OPCODE_IADD:
	case EVALUATOR_OPCODE_ISUB:
	case EVALUATOR_OPCODE_IMUL:
	case EVALUATOR_OPCODE_IDIV:
	case EVALUATOR_OPCODE_IMOD:
		_Machine->printf(_Machine, "(%d)", term->data.as_integer);
		break;

	case EVALUATOR_OPCODE_IADD_FP32:
	case EVALUATOR_OPCODE_ISUB_FP32:
	case EVALUATOR_OPCODE_IMUL_FP32:
	case EVALUATOR_OPCODE_IDIV_FP32:
		_Machine->printf(_Machine, "(%f)", term->data.as_float);
		break;

	case EVALUATOR_OPCODE_ASSIGN_FREG:
	case EVALUATOR_OPCODE_FREG_PREINC:
	case EVALUATOR_OPCODE_FREG_POSTINC:
	case EVALUATOR_OPCODE_FREG_PREDEC:
	case EVALUATOR_OPCODE_FREG_POSTDEC:
		_Machine->printf(_Machine, "[+%d]", term->data.as_integer);
		break;

	case EVALUATOR_OPCODE_ASSIGN_SREG:
	case EVALUATOR_OPCODE_SREG_PREINC:
	case EVALUATOR_OPCODE_SREG_POSTINC:
	case EVALUATOR_OPCODE_SREG_PREDEC:
	case EVALUATOR_OPCODE_SREG_POSTDEC:
		_Machine->printf(_Machine, "[%d]", term->data.as_integer);
		break;

	case EVALUATOR_OPCODE_CALL:
		func = _Machine->find_callable_unit_by_entry_point(_Machine, term->data.as_integer);
		name = _Machine->get_callable_unit_symbol(_Machine, func->idx);
		_Machine->printf(_Machine, ":%s", name);
		break;
	case EVALUATOR_OPCODE_BUILTIN:
		func = _Machine->get_callable_unit(_Machine, term->data.as_integer);
		name = _Machine->get_callable_unit_symbol(_Machine, func->idx);
		_Machine->printf(_Machine, ":%s", name);
		break;
	case EVALUATOR_OPCODE_LUACALL:
		func = _Machine->get_callable_unit(_Machine, term->data.as_integer);
		name = _Machine->get_callable_unit_symbol(_Machine, func->idx);
		_Machine->printf(_Machine, ":%s", name);
		break;
	case EVALUATOR_OPCODE_UNRESOLVED_EXPERNAL_CALL:
		func = _Machine->get_callable_unit(_Machine, term->data.as_integer);
		name = _Machine->get_callable_unit_symbol(_Machine, func->idx);
		_Machine->printf(_Machine, ":%s", name);
		break;
	}
}

_6IT_PRIVATE void _6IT_MACHINECALLX(_6EV_METHOD_NAME(print_expression), struct expression_t const *expression)
{
	_Machine->printf(_Machine, "\t{%d}: ", expression->first_term_index);

	for (int i = 0; i < expression->number_of_terms; ++i)
	{
		struct expression_term_t *term = expression->terms + i;
		_6EV_METHOD_NAME(print_expression_term)(_Machine, term);
		_Machine->printf(_Machine, " ");
	}

	_Machine->printf(_Machine, "\n");
}

_6IT_PRIVATE void _6IT_MACHINECALLX(_6EV_METHOD_NAME(print_state), struct expression_t const *expression)
{
	for (int i = 0; i <= expression->number_of_terms; ++i)
	{
		if (i == _REG_VPC(_REGS(_Machine)))
		{
			_Machine->printf(_Machine, "--> ");
		}
		else
		{
			_Machine->printf(_Machine, "    ");
		}

		struct expression_term_t *term = _REG_XS(_REGS(_Machine)) + expression->first_term_index + i;

		_6EV_METHOD_NAME(print_expression_term)(_Machine, term);
		_Machine->printf(_Machine, "\n");
	}
}

