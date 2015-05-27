/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

DEBUGGER_METHOD(void, print_disassembly)
{
	struct machine_t *machine = _This->machine;

	for (int i = 0; i < _REG_FUP(_REGS(machine)); i++)
	{
		struct callable_unit_t *f = _REG_US(_REGS(machine)) + i;
		char const *name = machine->get_callable_unit_symbol(machine, i);
		machine->printf(machine, "\t%d: %s %s(",
			i,
			machine->type_description(machine, f->return_type), name);

		for (int p = 0; p < f->number_of_parameters; ++p)
		{
			if (!f->parameters[p]) break;

			if (p > 0)
			{
				machine->printf(machine, ", ");
			}

			machine->printf(machine, machine->type_description(machine, f->parameters[p]));
		}

		if (f->entry_point_ptr)
		{
			machine->printf(machine, ")  BUILTIN\n");
		}
		else if (f->is_lua)
		{
			machine->printf(machine, ")  LUA\n");
		}
		else
		{
			machine->printf(machine, ")  %d-->%d\n", f->entry_point_program_counter, f->exit_point_program_counter);
			_This->print_callable_unit_disassembly(_This, f);
		}
	}
}

DEBUGGER_METHOD(void, print_state)
{
	struct machine_t *machine = _This->machine;

	machine->printf(machine, "-------- H6VM State ---------\n");

	machine->printf(machine, "callable units:\n");
	_This->print_disassembly(_This);

	machine->printf(machine, "call stack:\n");

	for (int i = _REG_ESP(_REGS(machine)); i >= 0; --i)
	{
		struct frame_t *sf = _REG_ES(_REGS(machine)) + i;

		machine->printf(machine, "\tframe: %d, function: %d:%s(), args register: %d, locals register: %i\n",
			i, sf->callable_unit_idx,
			machine->get_callable_unit_symbol(machine, sf->callable_unit_idx),
			FRAME_BASE_REGISTER_INDEX(machine, sf),
			0); // todo crz
	}

	_This->print_registers(_This);

	machine->printf(machine, "expressions:\n");

	int idx = 0;
	struct expression_t expression;
	while (idx < _REG_FXP(_REGS(machine)))
	{
		machine->build_expression(machine, &expression, idx);

		machine->evaluator.print_expression(machine, &expression);

		idx += expression.number_of_terms + 1;
	}

	if (_REG_VPC(_REGS(machine)) != -1)
	{
		machine->printf(machine, "evaluator state\n");
		machine->evaluator.print_state(machine, &expression);
	}

	machine->printf(machine, "evaluator stack:\n");

	if (_REG_VSP(_REGS(machine)) != -1)
	{
		for (int i = _REG_VSP(_REGS(machine)); i >= 0; --i)
		{
			struct value_t *value = _REG_VS(_REGS(machine)) + i;

			machine->printf(machine, "\t%d: ", i);
			switch (value->type)
			{
			case TYPE_DOUBLE:
				machine->printf(machine, "double: %f\n", value->as_double);
				break;
			case TYPE_INT:
				machine->printf(machine, "int: %d\n", value->as_integer);
				break;
			case TYPE_CHAR_CONST_PTR:
				machine->printf(machine, "char const *: \"%s\"\n", value->as_char_const_ptr);
				break;
			default:
				machine->die(machine, 0, "bad type on value stack");
				break;
			}
		}
	}
	else
	{
		machine->printf(machine, "\tempty\n");
	}

	_This->print_strings(_This);

	machine->printf(machine, "machine registers:\n");
	machine->printf(machine, "\tprogram counter	            PC:  %d\n", _REG_PC(_REGS(machine)));
	machine->printf(machine, "\texecution stack pointer     ESP: %d\n", _REG_ESP(_REGS(machine)));
	machine->printf(machine, "\tregister stack pointer      RSP: %d\n", _REG_RSP(_REGS(machine)));
	machine->printf(machine, "\tevaluator stack pointer     VSP: %d\n", _REG_VSP(_REGS(machine)));
	machine->printf(machine, "\tfree code pointer           FCP: %d\n", _REG_FCP(_REGS(machine)));
	machine->printf(machine, "\tfree expression pointer     FXP: %d\n", _REG_FXP(_REGS(machine)));
	machine->printf(machine, "\tfree callable unit pointer  FUP: %d\n", _REG_FUP(_REGS(machine)));
	machine->printf(machine, "\tfree register pointer       FRP: %d\n", _REG_FRP(_REGS(machine)));
	machine->printf(machine, "\tfree data pointer           FDP: %d\n", _REG_FDP(_REGS(machine)));
	machine->printf(machine, "\tfree string pointer         FSP: %d\n", _REG_FSP(_REGS(machine)));

#ifdef _6IT_DEBU66ER
	machine->printf(machine, "\tfree symbol index: %d\n", machine->debug->free_symbol_index);

	machine->printf(machine, "metrics:\n");
	machine->printf(machine, "\tstatements interpreted: %d\n", machine->debug->count_statements_interpreted);
	machine->printf(machine, "\tinstructions executed: %d\n", machine->debug->count_instructions_executed);
#endif

	machine->printf(machine, "-----------------\n");
}

DEBUGGER_METHOD(void, print_strings)
{
	struct machine_t *machine = _This->machine;

	char const *p = _REG_SS(_REGS(machine));

	machine->printf(machine, "strings:\n");
	int offset = 0;

	int fsp = _REG_FSP(_REGS(machine));

	while (offset < fsp)
	{
		machine->printf(machine, "\t%d: %s\n", offset, p + offset);

		int len = strlen(p + offset);
		offset += len + 1;
	}
}

DEBUGGER_METHOD(void, print_current_instruction_disassembly)
{
	struct machine_t *machine = _This->machine;

	int pc = _REG_PC(_REGS(machine));

	struct callable_unit_t *cu = machine->get_callable_unit(machine, _FRAME(machine)->callable_unit_idx);
	machine->printf(machine, "fr:%d cu:%d %s\n",
		_FRAME(machine)->position, cu->idx,
		machine->disassemble_instruction(machine, pc, cu->entry_point_program_counter));

	int expressions[10];
	int number_of_expressions = machine->get_referenced_expressions(machine, pc, expressions);
	for (int i = 0; i < number_of_expressions; ++i)
	{
		machine->printf(machine, "\t\t");

		struct expression_t expression;
		machine->build_expression(machine, &expression, expressions[i]);
		machine->evaluator.print_expression(machine, &expression);
	}
}

DEBUGGER_METHODX(void, print_callable_unit_disassembly, struct callable_unit_t *unit)
{
	struct machine_t *machine = _This->machine;

	int expressions[10];

	for (int pc = unit->entry_point_program_counter; pc <= unit->exit_point_program_counter;)
	{
		machine->printf(machine, "\t\t%s\n", machine->disassemble_instruction(machine, pc, unit->entry_point_program_counter));
		opcode_t opcode = _REG_CS(_REGS(machine))[pc];

		int number_of_expressions = machine->get_referenced_expressions(machine, pc, expressions);
		for (int i = 0; i < number_of_expressions; ++i)
		{
			machine->printf(machine, "\t\t");

			struct expression_t expression;
			machine->build_expression(machine, &expression, expressions[i]);
			machine->evaluator.print_expression(machine, &expression);
		}

		int len = machine->processor.get_instruction_length(opcode);
		pc += len;
	}
}

DEBUGGER_METHOD(void, print_registers)
{
	_This->print_relative_registers(_This);
	_This->print_absolute_registers(_This);
}

DEBUGGER_METHOD(void, print_value_stack)
{
	struct machine_t *machine = _This->machine;

	machine->printf(machine, "value stack:\n");

	for (int i = _REG_VSP(_REGS(machine)); i >= 0; --i)
	{
		struct value_t *value = _REG_VS(_REGS(machine)) + i;

		machine->printf(machine, "    ");
		switch (value->type)
		{
		case TYPE_INT:
			machine->printf(machine, "int: %a\n", machine->format_primitive_value(machine, value));
			break;
		case TYPE_FLOAT:
			machine->printf(machine, "float: %s\n", machine->format_primitive_value(machine, value));
			break;
		case TYPE_CHAR_CONST_PTR:
			machine->printf(machine, "char const *: \"%s\"\n", machine->format_primitive_value(machine, value));
			break;
		default:
			machine->die(machine, 0, "bad type on value stack");
			break;
		}
	}
}

DEBUGGER_METHOD(void, print_evaluator_state)
{
	struct machine_t *machine = _This->machine;

	struct expression_t expression;
	machine->build_expression(machine, &expression, _REG_VPC(_REGS(machine)));// machine->evaluator.expression_start_index);

	//	if (!machine->current_expression_start_index) return;

	if (!machine->debug)
	{
		machine->die(machine, 0, "no machine debug info");
	}

	machine->printf(machine, "---------- expression %d:%d:%d --------------\n",
		expression.first_term_index, expression.number_of_terms, _REG_VPC(_REGS(machine)));

	machine->evaluator.print_expression(machine, &expression);// machine->evaluator.expression_start_index);
	// todo crz: this
	//for (int i = 0; i < expression.number_of_terms; ++i)
	//{
	//	if (i == machine->current_expression_counter)
	//	{
	//		machine->printf(machine, "--> ");
	//	}
	//	else
	//	{
	//		machine->printf(machine, "    ");
	//	}

	//	struct expression_term_t *term = expression_get_term(machine, &expression, i);
	//	machine_print_expression_term(machine, term);
	//	machine->printf(machine, "\n");
	//}

	machine->debugger.print_value_stack(&machine->debugger);

	machine->printf(machine, "------------------------------------\n");
}

//void machine_print_disassembly(struct machine_t *machine)
//{
//	machine->printf(machine, "-------- H6VM Disassembly ---------\n");
//
//	for (int pc = 0; pc < _REG_FCP(_REGS(machine));)
//	{
//		machine->printf(machine, "%s\n", machine_disassemble_instruction(machine, pc));
//		opcode_t opcode = _REG_CS(_REGS(machine))[pc];
//		int len = h6vm_instruction_length(opcode);
//		pc += len;
//	}
//
//	machine->printf(machine, "------------------------------------\n");
//}


DEBUGGER_METHODX(void, print_register, struct register_t *v)
{
	struct machine_t *machine = _This->machine;

	struct register_metadata_t *m = machine->get_register_metadata(machine, v->idx);

	char const *name = machine->register_get_symbol(machine, v->idx);
	char desc[80];
	strcpy(desc, machine->type_description(machine, v->value.type));

	switch (v->value.type)
	{
	case TYPE_EXPRESSION:
		machine->printf(machine, "%d: {%d}, type: EXPRESSION, relative: %d, frame: %d\n",
			v->idx,
			v->value.as_integer,
			v->local_idx,
			m->frame);
		break;
	case TYPE_INT:
		machine->printf(machine, "%d: %s %s = %d, type: %d, relative: %d, frame: %d\n",
			v->idx,
			desc,
			name,
			v->value.as_integer,
			v->value.type,
			v->local_idx,
			m->frame);
		break;
	case TYPE_FLOAT:
		machine->printf(machine, "%d: %s %s = %f, type: %d, relative: %d, frame: %d, lua: %d\n",
			v->idx,
			desc,
			name,
			v->value.as_float,
			v->value.type,
			v->local_idx,
			m->frame,
			v->is_lua);
		break;
	default:
		machine->printf(machine, "?? %d: %s %s = %f, type: %d, relative: %d, frame: %d\n",
			v->idx,
			desc,
			name,
			v->value.as_integer,
			v->value.type,
			v->local_idx,
			m->frame);
		break;
	}
}

DEBUGGER_METHOD(void, print_relative_registers)
{
	struct machine_t *machine = _This->machine;

	machine->printf(machine, "relative registers:\n");

	int last = _REG_RSP(_REGS(machine)) - 1;
	int first = machine->config.max_number_of_globals;
	int bp = FRAME_BASE_REGISTER_INDEX(machine, _FRAME(machine));
	int rsp = _REG_RSP(_REGS(machine));

	if (rsp > first) // todo crz: this hooks wrong (and hacky)
	{
		for (int i = last; i >= first; i--)
		{
			if (i == bp)
			{
				machine->printf(machine, "--> ");
			}
			else
			{
				machine->printf(machine, "    ");
			}

			struct register_t *v = _REG_RS(_REGS(machine)) + i;

			_This->print_register(_This, v);
		}
	}
	else
	{
		machine->printf(machine, "\tnone\n");
	}
}

DEBUGGER_METHOD(void, print_absolute_registers)
{
	struct machine_t *machine = _This->machine;

	machine->printf(machine, "absolute registers:\n");
	if (_REG_FRP(_REGS(machine)) > 0)
	{
		for (int i = 0; i < _REG_FRP(_REGS(machine)); i++)
		{
			struct register_t *v = _REG_RS(_REGS(machine)) + i;
			machine->printf(machine, "\t");
			_This->print_register(_This, v);
		}
	}
	else
	{
		machine->printf(machine, "\tnone\n");
	}
}

