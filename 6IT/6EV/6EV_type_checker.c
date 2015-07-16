/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6EV.h"

#include <assert.h>
#include <string.h>

// todo crz: this needs a lot of work
_6IT_PRIVATE int _6IT_MACHINECALLXX(_6EV_check_expression, struct expression_parser_state_t *state, struct expression_t const *expression)
{
	int pointer_save = _REG_VSP(_REGS(_Machine));
	struct machine_register_t *var;
	struct value_t *stack = _REG_VS(_REGS(_Machine));
	int current = 0;
	struct expression_term_t *term = expression->terms;
	int type1;
	int type2;
	struct callable_unit_t *cu;

	do
	{
		switch (term->type)
		{
		case EVALUATOR_OPCODE_LT:
		case EVALUATOR_OPCODE_LE:
		case EVALUATOR_OPCODE_GT:
		case EVALUATOR_OPCODE_GE:
		case EVALUATOR_OPCODE_EQ:
		case EVALUATOR_OPCODE_NE:
		case EVALUATOR_OPCODE_ADD:
		case EVALUATOR_OPCODE_SUB:
		case EVALUATOR_OPCODE_MUL:
		case EVALUATOR_OPCODE_DIV:
		case EVALUATOR_OPCODE_MOD:
			type1 = stack[_REG_VSP(_REGS(_Machine))].type;
			type2 = stack[_REG_VSP(_REGS(_Machine)) - 1].type;
			if (type1 != type2)
			{
				strcpy(state->message, "binary operator operands must be of the same type");
				return 1;
			}

			DROP(_Machine);

			if (type1 == TYPE_FLOAT)
			{
				switch (term->type)
				{
				case EVALUATOR_OPCODE_ADD:
					term->type = EVALUATOR_OPCODE_ADD_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = type1;
					break;
				case EVALUATOR_OPCODE_SUB:
					term->type = EVALUATOR_OPCODE_SUB_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = type1;
					break;
				case EVALUATOR_OPCODE_MUL:
					term->type = EVALUATOR_OPCODE_MUL_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = type1;
					break;
				case EVALUATOR_OPCODE_DIV:
					term->type = EVALUATOR_OPCODE_DIV_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = type1;
					break;
				case EVALUATOR_OPCODE_EQ:
					term->type = EVALUATOR_OPCODE_EQ_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = TYPE_INT;
					break;
				case EVALUATOR_OPCODE_NE:
					term->type = EVALUATOR_OPCODE_NE_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = TYPE_INT;
					break;
				case EVALUATOR_OPCODE_LT:
					term->type = EVALUATOR_OPCODE_LT_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = TYPE_INT;
					break;
				case EVALUATOR_OPCODE_LE:
					term->type = EVALUATOR_OPCODE_LE_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = TYPE_INT;
					break;
				case EVALUATOR_OPCODE_GT:
					term->type = EVALUATOR_OPCODE_GT_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = TYPE_INT;
					break;
				case EVALUATOR_OPCODE_GE:
					term->type = EVALUATOR_OPCODE_GE_FP32;
					stack[_REG_VSP(_REGS(_Machine))].type = TYPE_INT;
					break;
				default:
					strcpy(state->message, "binary operator does not support FP32 operands");
					return 1;
				}
			}

			break;

		case EVALUATOR_OPCODE_CAST_INT:
			type1 = stack[_REG_VSP(_REGS(_Machine))].type;
			switch (type1)
			{
			case TYPE_INT:
				return ERROR_6EV_CASTINT_OPERAND_MUST_BE_NON_INT;
			case TYPE_FLOAT:
				term->type = EVALUATOR_OPCODE_CAST_FP32_INT;
				stack[_REG_VSP(_REGS(_Machine))].type = TYPE_INT;
				break;
			default:
				return ERROR_6EV_INVALID_CAST_TO_INT;
			}
			break;
		case EVALUATOR_OPCODE_CAST_FP32:
			type1 = stack[_REG_VSP(_REGS(_Machine))].type;
			switch (type1)
			{
			case TYPE_FLOAT:
				return ERROR_6EV_CASTFP32_OPERAND_MUST_BE_NON_FP32;
			case TYPE_INT:
				term->type = EVALUATOR_OPCODE_CAST_INT_FP32;
				stack[_REG_VSP(_REGS(_Machine))].type = TYPE_FLOAT;
				break;
			default:
				return ERROR_6EV_INVALID_CAST_TO_FP32;
			}
			break;

		case EVALUATOR_OPCODE_IADD:
		case EVALUATOR_OPCODE_ISUB:
		case EVALUATOR_OPCODE_IMUL:
		case EVALUATOR_OPCODE_IDIV:
		case EVALUATOR_OPCODE_IMOD:
		case EVALUATOR_OPCODE_MINUS:
		case EVALUATOR_OPCODE_PLUS:
			type1 = stack[_REG_VSP(_REGS(_Machine))].type;
			if (type1 != TYPE_INT)
			{
				return ERROR_6EV_INT_UNARY_OPERATOR_OPERAND_WRONG_TYPE;
			}
			break;

		case EVALUATOR_OPCODE_IADD_FP32:
		case EVALUATOR_OPCODE_ISUB_FP32:
		case EVALUATOR_OPCODE_IMUL_FP32:
		case EVALUATOR_OPCODE_IDIV_FP32:
		case EVALUATOR_OPCODE_MINUS_FP32:
		case EVALUATOR_OPCODE_PLUS_FP32:
			type1 = stack[_REG_VSP(_REGS(_Machine))].type;
			if (type1 != TYPE_FLOAT)
			{
				return ERROR_6EV_FP32_UNARY_OPERATOR_OPERAND_WRONG_TYPE;
			}
			break;

		case EVALUATOR_OPCODE_ASSIGN_SREG:
		case EVALUATOR_OPCODE_ASSIGN_SREG_ADD:
		case EVALUATOR_OPCODE_ASSIGN_SREG_SUB:
		case EVALUATOR_OPCODE_ASSIGN_SREG_MUL:
		case EVALUATOR_OPCODE_ASSIGN_SREG_DIV:
		case EVALUATOR_OPCODE_ASSIGN_SREG_MOD:
			var = _STATIC_REGISTER(_Machine, term->data.as_integer);
			type1 = stack[_REG_VSP(_REGS(_Machine))].type;
			DROP(_Machine);
			if (type1 != var->value.type)
			{
				return ERROR_6EV_REGISTER_ASSIGNMENT_FROM_WRONG_TYPE;
			}

			if (type1 == TYPE_FLOAT)
			{
				switch (term->type)
				{
				case EVALUATOR_OPCODE_ASSIGN_SREG_ADD:
					term->type = EVALUATOR_OPCODE_ASSIGN_SREG_ADD_FP32;
					break;
				case EVALUATOR_OPCODE_ASSIGN_SREG_SUB:
					term->type = EVALUATOR_OPCODE_ASSIGN_SREG_SUB_FP32;
					break;
				case EVALUATOR_OPCODE_ASSIGN_SREG_MUL:
					term->type = EVALUATOR_OPCODE_ASSIGN_SREG_MUL_FP32;
					break;
				case EVALUATOR_OPCODE_ASSIGN_SREG_DIV:
					term->type = EVALUATOR_OPCODE_ASSIGN_SREG_DIV_FP32;
					break;
				}
			}
			break;

		case EVALUATOR_OPCODE_ALLOC_FREG:
		case EVALUATOR_OPCODE_ALLOC_SREG:
			PUSH_INT(_Machine, 0);
			break;

		case EVALUATOR_OPCODE_DROP:
			DROP(_Machine);
			break;

		case EVALUATOR_OPCODE_ASSIGN_FREG:
		case EVALUATOR_OPCODE_ASSIGN_FREG_ADD:
		case EVALUATOR_OPCODE_ASSIGN_FREG_SUB:
		case EVALUATOR_OPCODE_ASSIGN_FREG_MUL:
		case EVALUATOR_OPCODE_ASSIGN_FREG_DIV:
		case EVALUATOR_OPCODE_ASSIGN_FREG_MOD:
			var = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer);
			type1 = stack[_REG_VSP(_REGS(_Machine))].type;
			if (type1 != var->value.type)
			{
				strcpy(state->message, "register assignment from wrong type");
				return 1;
			}

			if (type1 == TYPE_FLOAT)
			{
				switch (term->type)
				{
				case EVALUATOR_OPCODE_ASSIGN_FREG_ADD:
					term->type = EVALUATOR_OPCODE_ASSIGN_FREG_ADD_FP32;
					break;
				case EVALUATOR_OPCODE_ASSIGN_FREG_SUB:
					term->type = EVALUATOR_OPCODE_ASSIGN_FREG_SUB_FP32;
					break;
				case EVALUATOR_OPCODE_ASSIGN_FREG_MUL:
					term->type = EVALUATOR_OPCODE_ASSIGN_FREG_MUL_FP32;
					break;
				case EVALUATOR_OPCODE_ASSIGN_FREG_DIV:
					term->type = EVALUATOR_OPCODE_ASSIGN_FREG_DIV_FP32;
					break;
				}
			}
			break;

		case EVALUATOR_OPCODE_SREG_PREINC:
		case EVALUATOR_OPCODE_SREG_POSTINC:
		case EVALUATOR_OPCODE_SREG_PREDEC:
		case EVALUATOR_OPCODE_SREG_POSTDEC:
			var = _STATIC_REGISTER(_Machine, term->data.as_integer);
			if (TYPE_INT != var->value.type)
			{
				strcpy(state->message, "increment/decrement operator on non-integral type");
				return 1;
			}
			PUSH_INT(_Machine, 0);
			break;
		case EVALUATOR_OPCODE_FREG_PREINC:
		case EVALUATOR_OPCODE_FREG_POSTINC:
		case EVALUATOR_OPCODE_FREG_PREDEC:
		case EVALUATOR_OPCODE_FREG_POSTDEC:
			var = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer);
			if (TYPE_INT != var->value.type)
			{
				strcpy(state->message, "increment/decrement operator on non-integral type");
				return 1;
			}
			PUSH_INT(_Machine, 0);
			break;

		case EVALUATOR_OPCODE_PUSH_FREG:
			var = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer);
			PUSH_TYPED_VALUE(_Machine, 0, term->data.type);
			break;
		case EVALUATOR_OPCODE_PUSH_SREG:
			var = _STATIC_REGISTER(_Machine, term->data.as_integer);
			PUSH_TYPED_VALUE(_Machine, 0, term->data.type);
			break;

		case EVALUATOR_OPCODE_BUILTIN:
		{
			struct value_t temp[10];
			int j = 0;
			for (int i = term->number_of_args - 1; i >= 0; --i)
			{
				temp[j++] = POP_VALUE(_Machine);
			}

			for (int i = 0; i < term->number_of_args; ++i)
			{
				PUSH_VALUE(_Machine, temp[i]);
			}

			PUSH_INT(_Machine, 0);

			_SUB_VSP(_REGS(_Machine), term->number_of_args);
		}
		break;
		case EVALUATOR_OPCODE_CALL:
			cu = GET_CALLABLE_UNIT(_Machine, term->data.as_integer);

			for (int i = cu->number_of_parameters - 1; i >= 0; --i)
			{
				type1 = stack[_REG_VSP(_REGS(_Machine))].type;
				if (type1 != cu->parameters[i])
				{
					strcpy(state->message, "callable unit argument has wrong type");
					return 1;
				}

				DROP(_Machine);
			}

			PUSH_TYPED_VALUE(_Machine, 0, cu->return_type);
			break;
		case EVALUATOR_OPCODE_LUACALL:
			cu = GET_CALLABLE_UNIT(_Machine, term->data.as_integer);

			for (int i = cu->number_of_parameters - 1; i >= 0; --i)
			{
				type1 = stack[_REG_VSP(_REGS(_Machine))].type;
				if (type1 != cu->parameters[i])
				{
					strcpy(state->message, "callable unit argument has wrong type");
					return 1;
				}

				DROP(_Machine);
			}

			PUSH_TYPED_VALUE(_Machine, 0, cu->return_type);
			break;
		case EVALUATOR_OPCODE_UNRESOLVED_EXPERNAL_CALL:
			cu = GET_CALLABLE_UNIT(_Machine, term->data.as_integer);
			PUSH_TYPED_VALUE(_Machine, 0, cu->return_type);
			break;
		case EVALUATOR_OPCODE_PUSH_INTEGER:
			PUSH_INT(_Machine, 0);
			break;
		case EVALUATOR_OPCODE_PUSH_FLOAT:
			PUSH_FLOAT(_Machine, 0);
			break;
		case EVALUATOR_OPCODE_PUSH_STRING:
			_INC_VSP(_REGS(_Machine));
			_REG_VIP(_REGS(_Machine))->as_integer = 0;
			_REG_VIP(_REGS(_Machine))->type = TYPE_CHAR_CONST_PTR;
			break;
		case EVALUATOR_OPCODE_TERMINATOR:
			--term;
			break;
		default:
			_Machine->printf(_Machine, "bad opcode: %d\n", term->type);

			strcpy(state->message, "bad expression token");
			return 1;
		}

		++current;
	} while ((++term)->type != EVALUATOR_OPCODE_TERMINATOR);

	_SET_VSP(_REGS(_Machine), pointer_save);

	strcpy(state->message, "OK");

	return ITSOK;
}

// todo crz: decide on this type of stuff (no pun..)
/*
int type_base_is_integral(int type)
{
	return 0 == (type & TYPE_REAL);
}

int type_base_is_real(int type)
{
	return type & TYPE_REAL;
}

int type_is_pointer(int type)
{
	int t = type & TYPE_POINTER_MASK;

	if (t == TYPE_VALUE) return 0;

	return 1;
}

//int type_is_value(int type)
//{
//
//}

int type_is_assignable_from(int dest, int source)
{
	// hack crz
	if (dest != source) return 0;

	return 1;

	if ((dest & TYPE_POINTER) != (source & TYPE_POINTER))
	{
		return 0;
	}

	switch (dest & TYPE_BASE_MASK)
	{
	case TYPE_CHAR:
		switch (source)
		{
		case TYPE_CHAR:
			return 1;
		default:
			return 0;
		}
		break;
	case TYPE_INT:
		switch (source)
		{
		case TYPE_CHAR:
		case TYPE_INT:
			return 1;
		default:
			return 0;
		}
		break;
	case TYPE_FLOAT:
		switch (source)
		{
		case TYPE_FLOAT:
			return 1;
		default:
			return 0;
		}
		break;
	case TYPE_DOUBLE:
		switch (source)
		{
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
			return 1;
		default:
			return 0;
		}
	}

	return 0;
}


*/