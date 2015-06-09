/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include "SubC.h"
#include "6IL/6IL_opcodes.h"

_SUBC_METHOD_CONST(void, compile_while)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;

	struct expression_t condition_expression;

	scanner->consume_token(scanner, TOKEN_OPEN_PAREN);

	init_new_expression(machine, &condition_expression);
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &condition_expression);

	scanner->consume_token(scanner, TOKEN_CLOSE_PAREN);

	int rel = _REG_RSP(_REGS(machine)) - _State->base_RSP;

	emitter->emit_instruction(emitter, H6VM_OPCODE_CONDITION, condition_expression.first_term_index);

	int opcode_pc = emitter->emit_placeholder(emitter);
	//emitter->emit_expression_index(emitter, condition_expression.first_term_index);
	int jump_over_pc = emitter->emit_placeholder(emitter);

	int loop_PC = _REG_FCP(_REGS(machine));
	_State->loop_PC = opcode_pc;

	if (scanner->try_consume_token(scanner, TOKEN_OPEN_BRACE))
	{
		emitter->set_code(emitter, opcode_pc, H6VM_OPCODE_WHILEB);
		_SUBC_METHOD_NAME(compile_block)(_This, _State);
		scanner->consume_token(scanner, TOKEN_CLOSE_BRACE);
	}
	else
	{
		emitter->set_code(emitter, opcode_pc, H6VM_OPCODE_WHILES);
		_SUBC_METHOD_NAME(compile_single_statement_scope)(_This, _State);
	}

	emitter->set_code(emitter, jump_over_pc, _REG_FCP(_REGS(machine)) - _State->callable_unit_PC);

	emitter->emit_instruction(emitter, H6VM_OPCODE_CONDITION, condition_expression.first_term_index);

	emitter->emit_instruction(emitter, H6VM_OPCODE_JUMPIF, 0);
	//emitter->emit_expression_index(emitter, condition_expression.first_term_index);
	emitter->emit_PC(emitter, loop_PC - _State->callable_unit_PC);

	emitter->emit_instruction(emitter, H6VM_OPCODE_LOADRSP, rel);
}

_SUBC_METHOD_CONST(void, compile_do)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;

	struct expression_t condition_expression;

	int rel = _REG_RSP(_REGS(machine)) - _State->base_RSP;

	int opcode_pc = emitter->emit_placeholder(emitter);
	int while_pc = emitter->emit_placeholder(emitter);

	int loop_PC = _REG_FCP(_REGS(machine));
	_State->loop_PC = opcode_pc;

	if (scanner->try_consume_token(scanner, TOKEN_OPEN_BRACE))
	{
		emitter->set_code(emitter, opcode_pc, H6VM_OPCODE_DOB);
		_SUBC_METHOD_NAME(compile_block)(_This, _State);
		scanner->consume_token(scanner, TOKEN_CLOSE_BRACE);
	}
	else
	{
		emitter->set_code(emitter, opcode_pc, H6VM_OPCODE_DOS);
		_SUBC_METHOD_NAME(compile_single_statement_scope)(_This, _State);
	}

	scanner->consume_string(scanner, "while");
	scanner->consume_token(scanner, TOKEN_OPEN_PAREN);
	init_new_expression(machine, &condition_expression);
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &condition_expression);
	scanner->consume_token(scanner, TOKEN_CLOSE_PAREN);
	scanner->consume_token(scanner, TOKEN_SEMI_COLON);

	emitter->set_code(emitter, while_pc, _REG_FCP(_REGS(machine)));

	emitter->emit_instruction(emitter, H6VM_OPCODE_JUMPIF, 0);
	emitter->emit_expression_index(emitter, condition_expression.first_term_index);
	emitter->emit_PC(emitter, loop_PC);

	emitter->emit_instruction(emitter, H6VM_OPCODE_LOADRSP, rel);
}

// todo crz: rework these

_SUBC_METHOD_CONST(void, compile_continue)
{
	struct instruction_emitter_t *emitter = &_This->machine->instruction_emitter;

	_This->scanner->consume_token(_This->scanner, TOKEN_SEMI_COLON);

	if (-1 == _State->loop_PC)
	{
		THROW(&_This->machine->exception, 1);
	}

	emitter->emit_instruction(emitter, H6VM_OPCODE_LOOP_CONTINUE, 0);
	emitter->emit_PC(emitter, _State->loop_PC);
}

_SUBC_METHOD_CONST(void, compile_break)
{
	struct instruction_emitter_t *emitter = &_This->machine->instruction_emitter;

	_This->scanner->consume_token(_This->scanner, TOKEN_SEMI_COLON);

	if (-1 == _State->loop_PC)
	{
		THROW(&_This->machine->exception, 1);
	}

	emitter->emit_instruction(emitter, H6VM_OPCODE_LOOP_BREAK, 0);
	emitter->emit_PC(emitter, _State->loop_PC);
}

