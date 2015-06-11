/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../SubC.h"

_SUBC_METHOD_CONST(void, compile_for)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;

	struct expression_t init_expression;
	struct expression_t condition_expression;
	struct expression_t iteration_expression;

	scanner->consume_token(scanner, TOKEN_OPEN_PAREN);

	init_new_expression(machine, &init_expression);
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &init_expression);

	scanner->consume_token(scanner, TOKEN_SEMI_COLON);

	init_new_expression(machine, &condition_expression);
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &condition_expression);

	scanner->consume_token(scanner, TOKEN_SEMI_COLON);

	init_new_expression(machine, &iteration_expression);
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &iteration_expression);

	scanner->consume_token(scanner, TOKEN_CLOSE_PAREN);

	int rel = _REG_RSP(_REGS(machine)) - _State->base_RSP;

	int rel_reg = 0;
	int lower_limit = 0;
	int upper_limit = 0;

	emitter->emit_instruction(emitter, H6VM_OPCODE_EXEC, init_expression.first_term_index);
	emitter->emit_instruction(emitter, H6VM_OPCODE_CONDITION, condition_expression.first_term_index);

	int opcode_pc = emitter->emit_placeholder(emitter);
	int oc = 0;

	int loop_PC = _REG_FCP(_REGS(machine));
	_State->loop_PC = opcode_pc;

	if (scanner->try_consume_token(scanner, TOKEN_OPEN_BRACE))
	{
		oc = H6VM_OPCODE_FORB;
		_SUBC_METHOD_NAME(compile_block)(_This, _State);
		scanner->consume_token(scanner, TOKEN_CLOSE_BRACE);
	}
	else
	{
		oc = H6VM_OPCODE_FORS;
		_SUBC_METHOD_NAME(compile_single_statement_scope)(_This, _State);
	}

	int next_pc;

	next_pc = emitter->emit_instruction(emitter, H6VM_OPCODE_EXEC, iteration_expression.first_term_index);
	emitter->emit_instruction(emitter, H6VM_OPCODE_CONDITION, condition_expression.first_term_index);
	emitter->emit_instruction(emitter, H6VM_OPCODE_NEXT, loop_PC - _State->callable_unit_PC);

	emitter->set_code(emitter, opcode_pc, MAKE_OPCODE(oc, (next_pc - _State->callable_unit_PC)));

	emitter->emit_instruction(emitter, H6VM_OPCODE_LOADRSP, rel);
}

