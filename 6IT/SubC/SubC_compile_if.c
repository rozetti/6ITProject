/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include "SubC.h"
#include "6IL/6IL_opcodes.h"

_SUBC_METHOD_CONST(void, compile_if)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;

	scanner->consume_token(scanner, TOKEN_OPEN_PAREN);

	struct expression_t expression;
	init_new_expression(machine, &expression);
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &expression);

	scanner->consume_token(scanner, TOKEN_CLOSE_PAREN);

	int has_true_block = 0;
	int has_false_block = 0;
	int has_else = 0;

	int rel = _REG_RSP(_REGS(machine)) - _State->base_RSP;

	emitter->emit_instruction(emitter, H6VM_OPCODE_CONDITION, expression.first_term_index);

	int opcode_pc = emitter->emit_placeholder(emitter);

	int else_pc = 0;
	int end_pc = 0;
	int else_opcode_pc = 0;

	if (scanner->try_consume_token(scanner, TOKEN_OPEN_BRACE))
	{
		has_true_block = 1;

		_SUBC_METHOD_NAME(compile_block)(_This, _State);

		scanner->consume_token(scanner, TOKEN_CLOSE_BRACE);
	}
	else
	{
		has_true_block = 0;

		_SUBC_METHOD_NAME(compile_single_statement_scope)(_This, _State);
	}

	if (scanner->try_consume_string(scanner, "else"))
	{
		has_else = 1;

		else_pc = _REG_FCP(_REGS(machine)) - _State->callable_unit_PC;
		else_opcode_pc = emitter->emit_placeholder(emitter);

		int jump_around_else_pc = emitter->emit_PC(emitter, 0);

		if (scanner->try_consume_token(scanner, TOKEN_OPEN_BRACE))
		{
			has_false_block = 1;

			_SUBC_METHOD_NAME(compile_block)(_This, _State);

			scanner->consume_token(scanner, TOKEN_CLOSE_BRACE);
		}
		else
		{
			has_false_block = 0;

			_SUBC_METHOD_NAME(compile_single_statement_scope)(_This, _State);
		}

		end_pc = _REG_FCP(_REGS(machine)) - _State->callable_unit_PC;
	}
	else
	{
		has_else = 0;
		end_pc = _REG_FCP(_REGS(machine)) - _State->callable_unit_PC;
	}

	opcode_t oc;
	if (!has_else)
	{
		oc = MAKE_OPCODE((has_true_block ? H6VM_OPCODE_IFB : H6VM_OPCODE_IFS), end_pc);
	}
	else
	{
		if (has_false_block)
		{
			oc = MAKE_OPCODE((has_true_block ? H6VM_OPCODE_IFBB : H6VM_OPCODE_IFSB), else_pc);
		}
		else
		{
			oc = MAKE_OPCODE((has_true_block ? H6VM_OPCODE_IFBS : H6VM_OPCODE_IFSS), else_pc);
		}

		emitter->set_code(emitter, else_opcode_pc, MAKE_OPCODE(H6VM_OPCODE_ELSE, end_pc));
	}

	emitter->set_code(emitter, opcode_pc, oc);

	// todo crz: try to get rid of this
	emitter->emit_instruction(emitter, H6VM_OPCODE_LOADRSP, rel);

	_REG_VPC(_REGS(machine)) = expression.first_term_index;
}
