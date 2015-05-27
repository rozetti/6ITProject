/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6EV/6EV_opcodes.h"
#include "6IL/6IL_opcodes.h"

// hack crz
static struct expression_t init_expression;
static struct expression_t condition_expression;
static struct expression_t iteration_expression;

DOWNCOMPILER_METHOD_CONST(int, downcompile_FORRI)
{
	struct machine_t *machine = _This->machine;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;

	opcode_t *next_p = _State->unit_p + OPERAND3(machine);

	int is_increl = 0;
	int rel_reg = 0;
	int lower_limit = 0;
	int upper_limit = 0;

	is_increl = 1;
	lower_limit = init_expression.terms[0].data.as_integer;
	upper_limit = condition_expression.terms[1].data.as_integer;
	int operator = condition_expression.terms[2].type;

	switch (operator)
	{
	case EVALUATOR_OPCODE_LT:
		break;
	case EVALUATOR_OPCODE_LE:
		++upper_limit;
		break;
	}
	
	rel_reg = init_expression.terms[1].data.as_integer;

	emitter->emit_instruction(emitter, T6IL_FORRI, rel_reg);
	emitter->emit_expression_index(emitter, lower_limit);
	int jump_to_next_pc = emitter->emit_placeholder(emitter);

	int loop_PC = _REG_FCP(_REGS(machine)) - _State->downcompiled_callable_unit_PC;

	_State->pc += machine->processor.get_instruction_length(OPCODE(machine));
	while (_State->pc < _State->callable_unit_PC + OPERAND3(machine))
	{
		downcompile_instruction(_This, _State);
	}

	opcode_t next_oc = OPCODE_INSTRUCTION(_REG_CS(_REGS(machine))[_State->pc]);
	int next_len = machine->processor.get_instruction_length(next_oc);
	_State->pc += next_len;

	int inc = 1;
	if (1 == inc)
	{
		emitter->emit_instruction(emitter, T6IL_NEXTRINCI, rel_reg);
		emitter->emit_expression_index(emitter, upper_limit);
		emitter->emit_integer(emitter, loop_PC);
	}
	else if (-1 == inc)
	{
		emitter->emit_instruction(emitter, T6IL_NEXTRDECI, rel_reg);
		emitter->emit_expression_index(emitter, upper_limit);
		emitter->emit_integer(emitter, loop_PC);
	}
	else
	{
		emitter->emit_instruction(emitter, T6IL_NEXTRI, rel_reg);
		emitter->emit_expression_index(emitter, upper_limit);
		emitter->emit_integer(emitter, MAKE_OPERAND(inc, loop_PC));
	}

	emitter->set_code(emitter, jump_to_next_pc, _REG_FCP(_REGS(machine)) - _State->downcompiled_callable_unit_PC);

	return 1;
}

DOWNCOMPILER_METHOD_CONST(int, downcompile_FORX)
{
	struct machine_t *machine = _This->machine;

	machine->build_expression(machine, &init_expression, OPERAND1(machine));
	machine->build_expression(machine, &condition_expression, OPERAND2(machine));

	opcode_t *next_p = _State->unit_p + OPERAND3(machine);

	int next_expressions_index = next_p[1];
	machine->build_expression(machine, &iteration_expression, next_expressions_index);

	if (init_expression.number_of_terms == 2 &&
		init_expression.terms[0].type == EVALUATOR_OPCODE_PUSH_INTEGER &&
		init_expression.terms[1].type == EVALUATOR_OPCODE_ASSIGN_FREG &&
		iteration_expression.number_of_terms == 1 &&
		iteration_expression.terms[0].type == EVALUATOR_OPCODE_FREG_PREINC &&
		condition_expression.number_of_terms == 3 &&
		condition_expression.terms[0].type == EVALUATOR_OPCODE_PUSH_FREG &&
		condition_expression.terms[1].type == EVALUATOR_OPCODE_PUSH_INTEGER &&
		condition_expression.terms[2].type == EVALUATOR_OPCODE_LE &&
		condition_expression.terms[0].data.as_integer == init_expression.terms[1].data.as_integer &&
		init_expression.terms[1].data.as_integer == init_expression.terms[1].data.as_integer)
	{
		return downcompile_FORRI(_This, _State);
	}

	return 0;
}

