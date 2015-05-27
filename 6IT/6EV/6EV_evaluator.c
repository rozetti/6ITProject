/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6EV.h"

#include <stdio.h>
#include <assert.h>

#define RELS _FRAME(_Machine)->frame_registers

#ifdef EVALUATOR_WITH_DEBUGGER
_6IT_PRIVATE int _6IT_MACHINECALL(evaluate_debug)
#else
_6IT_PRIVATE int _6IT_MACHINECALL(evaluate)
#endif
{
	struct register_t *var;
	struct expression_term_t *term = _REG_XS(_REGS(_Machine)) + _REG_VPC(_REGS(_Machine));

#ifdef EVALUATOR_WITH_DEBUGGER
	struct expression_t expression;
	_Machine->build_expression(_Machine, &expression, _REG_VPC(_REGS(_Machine)));
	int step = 1;
#endif

	int v;
	float f;

	for (;;)
	{
#ifdef EVALUATOR_WITH_DEBUGGER
		if (_Machine->debug)
		{
			if (_Machine->debug->eval_trace_on)
			{
				if (_Machine->evaluator.verbosity)
				{
					_Machine->printf(_Machine, "----------------- evaluation step %d\n", step++);
					_Machine->evaluator.print_state(_Machine, &expression);
				}
			}
		}

#endif
		switch (term->type)
		{
		case EVALUATOR_OPCODE_TERMINATOR:
#ifdef _6IT_STACK_DISPATCH
#else
			_FRAME(_Machine)->evaluator_program_counter = -1;
			_INC_IP(_REGS(_Machine));
#endif
#ifdef EVALUATOR_WITH_DEBUGGER
			if (_Machine->debug)
			{
				//CHECK_VSP(_Machine);

				if (_Machine->debug->eval_trace_on)
				{
					if (_Machine->evaluator.verbosity)
					{
						_Machine->debugger.print_value_stack(&_Machine->debugger);
						_Machine->debugger.print_registers(&_Machine->debugger);
						_Machine->printf(_Machine, "----------------- evaluation complete\n");
					}
				}
			}
#endif

			return 1;

		case EVALUATOR_OPCODE_DROP:
			DROP(_Machine);
			break;

		case EVALUATOR_OPCODE_IADD:
			TOP_INT(_Machine) += term->data.as_integer;
			break;
		case EVALUATOR_OPCODE_ISUB:
			TOP_INT(_Machine) -= term->data.as_integer;
			break;
		case EVALUATOR_OPCODE_IMUL:
			TOP_INT(_Machine) *= term->data.as_integer;
			break;
		case EVALUATOR_OPCODE_IDIV:
			TOP_INT(_Machine) /= term->data.as_integer;
			break;
		case EVALUATOR_OPCODE_IMOD:
			TOP_INT(_Machine) %= term->data.as_integer;
			break;

		case EVALUATOR_OPCODE_ADD:
			LEVEL2_INT(_Machine) += POP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_SUB:
			LEVEL2_INT(_Machine) -= POP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_MUL:
			LEVEL2_INT(_Machine) *= POP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_DIV:
			LEVEL2_INT(_Machine) /= POP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_MOD:
			LEVEL2_INT(_Machine) %= POP_INT(_Machine);
			break;

		case EVALUATOR_OPCODE_IADD_FP32:
			TOP_FLOAT(_Machine) += term->data.as_float;
			break;
		case EVALUATOR_OPCODE_ISUB_FP32:
			TOP_FLOAT(_Machine) -= term->data.as_float;
			break;
		case EVALUATOR_OPCODE_IMUL_FP32:
			TOP_FLOAT(_Machine) *= term->data.as_float;
			break;
		case EVALUATOR_OPCODE_IDIV_FP32:
			TOP_FLOAT(_Machine) /= term->data.as_float;
			break;

		case EVALUATOR_OPCODE_ADD_FP32:
			LEVEL2_FLOAT(_Machine) += POP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_SUB_FP32:
			LEVEL2_FLOAT(_Machine) -= POP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_MUL_FP32:
			LEVEL2_FLOAT(_Machine) *= POP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_DIV_FP32:
			LEVEL2_FLOAT(_Machine) /= POP_FLOAT(_Machine);
			break;

		case EVALUATOR_OPCODE_CAST_INT_FP32:
			SET_TOP_FLOAT((_Machine), (float)TOP_INT(_Machine));
			break;
		case EVALUATOR_OPCODE_CAST_FP32_INT:
			SET_TOP_INT((_Machine), (int)TOP_FLOAT(_Machine));
			break;
		case EVALUATOR_OPCODE_MINUS:
			TOP_INT(_Machine) = -(TOP_INT(_Machine));
			break;
		case EVALUATOR_OPCODE_PLUS:
			TOP_INT(_Machine) = +(TOP_INT(_Machine));
			break;

		case EVALUATOR_OPCODE_MINUS_FP32:
			TOP_FLOAT(_Machine) = -(TOP_FLOAT(_Machine));
			break;
		case EVALUATOR_OPCODE_PLUS_FP32:
			TOP_FLOAT(_Machine) = +(TOP_FLOAT(_Machine));
			break;

		case EVALUATOR_OPCODE_ALLOC_SREG:
			var = _Machine->allocate_static_register(_Machine, term->data.as_integer);
			PUSH_INT(_Machine, var->idx);
			break;
		case EVALUATOR_OPCODE_ASSIGN_SREG:
			var = _STATIC_REGISTER(_Machine, term->data.as_integer);
			var->value = TOP_VALUE(_Machine);
			break;

		case EVALUATOR_OPCODE_ALLOC_FREG:
			var = _Machine->allocate_frame_register(_Machine, term->data.type);
			PUSH_INT(_Machine, var->local_idx);
			break;
		case EVALUATOR_OPCODE_ASSIGN_FREG:
			_FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value = TOP_VALUE(_Machine);
			break;

		case EVALUATOR_OPCODE_ASSIGN_FREG_ADD:
			TOP_INT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer += TOP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_FREG_SUB:
			TOP_INT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer -= TOP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_FREG_MUL:
			TOP_INT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer *= TOP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_FREG_DIV:
			TOP_INT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer /= TOP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_FREG_MOD:
			TOP_INT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer %= TOP_INT(_Machine);
			break;

		case EVALUATOR_OPCODE_ASSIGN_FREG_ADD_FP32:
			TOP_FLOAT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_float += TOP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_FREG_SUB_FP32:
			TOP_FLOAT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_float -= TOP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_FREG_MUL_FP32:
			TOP_FLOAT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_float *= TOP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_FREG_DIV_FP32:
			TOP_FLOAT(_Machine) = _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_float /= TOP_FLOAT(_Machine);
			break;

		case EVALUATOR_OPCODE_FREG_PREINC:
			PUSH_INT(_Machine, ++_FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer);
			break;
		case EVALUATOR_OPCODE_FREG_POSTINC:
			PUSH_INT(_Machine, _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer++);
			break;
		case EVALUATOR_OPCODE_FREG_PREDEC:
			PUSH_INT(_Machine, --_FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer);
			break;
		case EVALUATOR_OPCODE_FREG_POSTDEC:
			PUSH_INT(_Machine, _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value.as_integer--);
			break;

		case EVALUATOR_OPCODE_ASSIGN_SREG_ADD:
			TOP_INT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer += TOP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_SREG_SUB:
			TOP_INT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer -= TOP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_SREG_MUL:
			TOP_INT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer *= TOP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_SREG_DIV:
			TOP_INT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer /= TOP_INT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_SREG_MOD:
			TOP_INT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer %= TOP_INT(_Machine);
			break;

		case EVALUATOR_OPCODE_ASSIGN_SREG_ADD_FP32:
			TOP_FLOAT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_float += TOP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_SREG_SUB_FP32:
			TOP_FLOAT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_float -= TOP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_SREG_MUL_FP32:
			TOP_FLOAT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_float *= TOP_FLOAT(_Machine);
			break;
		case EVALUATOR_OPCODE_ASSIGN_SREG_DIV_FP32:
			TOP_FLOAT(_Machine) = _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_float /= TOP_FLOAT(_Machine);
			break;

		case EVALUATOR_OPCODE_SREG_PREINC:
			PUSH_INT(_Machine, ++_STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer);
			break;
		case EVALUATOR_OPCODE_SREG_POSTINC:
			PUSH_INT(_Machine, _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer++);
			break;
		case EVALUATOR_OPCODE_SREG_PREDEC:
			PUSH_INT(_Machine, --_STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer);
			break;
		case EVALUATOR_OPCODE_SREG_POSTDEC:
			PUSH_INT(_Machine, _STATIC_REGISTER(_Machine, term->data.as_integer)->value.as_integer--);
			break;

		case EVALUATOR_OPCODE_LT:
			v = POP_INT(_Machine);
			TOP_INT(_Machine) = TOP_INT(_Machine) < v;
			break;
		case EVALUATOR_OPCODE_LE:
			v = POP_INT(_Machine);
			TOP_INT(_Machine) = TOP_INT(_Machine) <= v;
			break;
		case EVALUATOR_OPCODE_GT:
			v = POP_INT(_Machine);
			TOP_INT(_Machine) = TOP_INT(_Machine) > v;
			break;
		case EVALUATOR_OPCODE_GE:
			v = POP_INT(_Machine);
			TOP_INT(_Machine) = TOP_INT(_Machine) >= v;
			break;
		case EVALUATOR_OPCODE_EQ:
			v = POP_INT(_Machine);
			TOP_INT(_Machine) = TOP_INT(_Machine) == v;
			break;
		case EVALUATOR_OPCODE_NE:
			v = POP_INT(_Machine);
			TOP_INT(_Machine) = TOP_INT(_Machine) != v;
			break;

		case EVALUATOR_OPCODE_EQ_FP32:
			f = POP_FLOAT(_Machine);
			SET_TOP_INT(_Machine, TOP_FLOAT(_Machine) == f);
			break;
		case EVALUATOR_OPCODE_NE_FP32:
			f = POP_FLOAT(_Machine);
			SET_TOP_INT(_Machine, TOP_FLOAT(_Machine) != f);
			break;
		case EVALUATOR_OPCODE_LT_FP32:
			f = POP_FLOAT(_Machine);
			SET_TOP_INT(_Machine, TOP_FLOAT(_Machine) < f);
			break;
		case EVALUATOR_OPCODE_LE_FP32:
			f = POP_FLOAT(_Machine);
			SET_TOP_INT(_Machine, TOP_FLOAT(_Machine) <= f);
			break;
		case EVALUATOR_OPCODE_GT_FP32:
			f = POP_FLOAT(_Machine);
			SET_TOP_INT(_Machine, TOP_FLOAT(_Machine) > f);
			break;
		case EVALUATOR_OPCODE_GE_FP32:
			f = POP_FLOAT(_Machine);
			SET_TOP_INT(_Machine, TOP_FLOAT(_Machine) >= f);
			break;

		case EVALUATOR_OPCODE_PUSH_FREG:
			PUSH_VALUE(_Machine, _FRAME_REGISTER(_FRAME(_Machine), term->data.as_integer)->value);
			break;
		case EVALUATOR_OPCODE_PUSH_SREG:
			PUSH_VALUE(_Machine, _STATIC_REGISTER(_Machine, term->data.as_integer)->value);
			break;

		case EVALUATOR_OPCODE_BUILTIN:
		{
			struct callable_unit_t *function = _REG_US(_REGS(_Machine)) + term->data.as_integer;
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

			int rv = function->entry_point_ptr(_Machine);
			_SUB_VIP(_REGS(_Machine), term->number_of_args);
			PUSH_INT(_Machine, rv);

			break;
		}
#ifdef _6IT_SUPPORT_LUA
		case EVALUATOR_OPCODE_LUACALL:
		{
			_Machine->push_to_lua(_Machine);

			struct callable_unit_t *function = _REG_US(_REGS(_Machine)) + term->data.as_integer;
			struct value_t temp[10];
			int j = 0;
			for (int i = term->number_of_args - 1; i >= 0; --i)
			{
				temp[j++] = POP_VALUE(_Machine);
			}

			char const *name = _Machine->get_callable_unit_symbol(_Machine, function->idx);
			lua_getglobal(_Machine->lua, name);
			assert(lua_isfunction(_Machine->lua, -1));

			for (int i = 0; i < term->number_of_args; ++i)
			{
				lua_pushnumber(_Machine->lua, temp[i].as_float);
			}

			if (lua_pcall(_Machine->lua, term->number_of_args, 1, 0))
			{
				lua_pop(_Machine->lua, 1); // todo crz: error
				PUSH_FLOAT(_Machine, 0);
			}
			else
			{
				assert(lua_isnumber(_Machine->lua, -1));
				float rv = (float)lua_tonumber(_Machine->lua, -1);
				lua_pop(_Machine->lua, 1);
				PUSH_FLOAT(_Machine, rv);

				_Machine->pull_from_lua(_Machine);
			}

			break;
		}
#endif
		case EVALUATOR_OPCODE_UNRESOLVED_EXPERNAL_CALL: // todo crz: undecided if this fixup smells
		{
			struct callable_unit_t *function = CALLABLE_UNIT(_Machine, term->data.as_integer);
			if (function->is_lua)
			{
				term->type = EVALUATOR_OPCODE_LUACALL;
				continue;
			}
			else if (function->is_resolved)
			{
				term->type = EVALUATOR_OPCODE_CALL;
				term->data.as_integer = function->entry_point_program_counter;
				continue;
			}
			else
			{
				char const *name = _Machine->get_callable_unit_symbol(_Machine, function->idx);
				sprintf(_Machine->exception.message, "unresolved external '%s'", name);
				THROW(&_Machine->exception, 123);
			}

			break;
		}
		case EVALUATOR_OPCODE_CALL:
		{
#ifndef _6IT_STACK_DISPATCH
			_FRAME(_Machine)->evaluator_program_counter = term - _REG_XS(_REGS(_Machine)) + 1;
#endif

			++_FRAME(_Machine);
			SAMPLE_FRAME_BASE_REGISTER_INDEX(_Machine);

			for (int i = term->number_of_args - 1; i >= 0; --i)
			{
				_FRAME_REGISTER(_FRAME(_Machine), i)->value = *_REGS(_Machine)->evaluator_stack_top_pointer--;
				_INC_RSP(_REGS(_Machine));
			}

#ifdef _6IT_STACK_DISPATCH
			opcode_t *return_ip = _REG_IP(_REGS(_Machine));
#else
			_FRAME(_Machine)->return_ip = _REG_IP(_REGS(_Machine));
			_FRAME(_Machine)->entry_ip = _REG_CS(_REGS(_Machine)) + term->data.as_integer;
#endif

			_SET_IP(_REGS(_Machine), _REG_CS(_REGS(_Machine)) + term->data.as_integer);

#ifdef EVALUATOR_WITH_DEBUGGER
			// todo crz: put this back
			_FRAME(_Machine)->callable_unit_idx = term->data.as_integer;
			struct callable_unit_t *function = CALLABLE_UNIT(_Machine, term->data.as_integer);
			struct callable_unit_metadata_t *metadata = _Machine->get_callable_unit_metadata(_Machine, term->data.as_integer);
			for (int i = function->number_of_parameters - 1; i >= 0; --i)
			{
				struct register_metadata_t *m = _Machine->get_register_metadata(_Machine, _FRAME(_Machine)->frame_registers[i].idx);
				m->frame = _FRAME(_Machine)->position;
				sprintf(_Machine->register_metadata[_FRAME(_Machine)->frame_registers[i].idx].symbol, metadata->parameter_metadata[i].symbol, i);
			}
#endif

#ifdef _6IT_STACK_DISPATCH
			_Machine->execute(_Machine);

			_SET_IP(_REGS(_Machine), return_ip);
			_SET_RSP(_REGS(_Machine), FRAME_BASE_REGISTER_INDEX(_Machine, _FRAME(_Machine)));

			--_FRAME(_Machine);
#else
			return 0;
#endif
		}
		break;
		case EVALUATOR_OPCODE_PUSH_INTEGER:
			PUSH_VALUE(_Machine, term->data);
			break;
		case EVALUATOR_OPCODE_PUSH_FLOAT:
			PUSH_VALUE(_Machine, term->data); // todo crz: check this
			break;
		case EVALUATOR_OPCODE_PUSH_STRING:
			PUSH_TYPED_VALUE(_Machine, term->data.as_integer, TYPE_CHAR_CONST_PTR);
			break;
		default:
			sprintf(_Machine->exception.message, "bad expression opcode '%d'", term->type);
			THROW(&_Machine->exception, 1);
			//machine_die_immediately(_Machine, 0, "bad expression opcode");
			break;
		}

		++term;
#ifdef EVALUATOR_WITH_DEBUGGER
		if (_Machine->debug)
		{
			if (_Machine->debug->eval_trace_on)
			{
				if (_Machine->evaluator.verbosity)
				{
					++_REG_VPC(_REGS(_Machine));
					_Machine->debugger.print_value_stack(&_Machine->debugger);
					_Machine->debugger.print_registers(&_Machine->debugger);
				}
			}
		}
#endif
	}
}
