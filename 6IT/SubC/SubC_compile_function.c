/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "SubC.h"

#include "6IL/6IL_opcodes.h"

#include <string.h>

_SUBC_METHODX_CONST(void, compile_function_parameters, struct callable_unit_t *function)
{
	struct machine_t *machine = _This->machine;
	struct frame_t *frame = _State->frame;
	struct scanner_t *scanner = _This->scanner;

	int parameter_index = 0;

	// todo crz: if already declared, just check parms match

	scanner->consume_token(scanner, TOKEN_OPEN_PAREN);
	if (!scanner->try_consume_token(scanner, TOKEN_CLOSE_PAREN))
	{
		do
		{
			data_type_t dt = _State->expression_parser_state->parser->parse_data_type(_State->expression_parser_state->parser, _State->expression_parser_state);
			if (TYPE_INVALID == dt)
			{
				strcpy(_State->expression_parser_state->message, "bad parameter");
				THROW(&_State->expression_parser_state->exception, 1); // todo crz: proper fault code
			}

			//struct register_t *p = function->parameters + parameter_index;
			//p->value.type = dt;

			function->parameters[parameter_index] = dt;
			machine->set_callable_unit_parameter_symbol(machine, function->idx, parameter_index, scanner->token.token);

			push_local_variable(_This->machine, dt, scanner->token.token);

			++parameter_index;
			scanner->move_next_token(scanner);
		} while (scanner->try_consume_token(scanner, TOKEN_COMMA));

		scanner->consume_token(scanner, TOKEN_CLOSE_PAREN);
	}

	function->number_of_parameters = parameter_index;
}

_SUBC_METHOD_CONST(void, compile_function)
{
	struct machine_t *machine = _This->machine;
	struct frame_t *frame = _State->frame;
	struct scanner_t *scanner = _This->scanner;
	struct instruction_emitter_t *emitter = &machine->instruction_emitter;

	_State->callable_unit_PC = _REG_FCP(_REGS(machine));

	data_type_t return_type = _State->expression_parser_state->parser->parse_data_type(_State->expression_parser_state->parser, _State->expression_parser_state);

	struct callable_unit_t *function = machine->find_callable_unit(machine, scanner->token.token);
	if (!function)
	{
		function = machine->add_callable_unit(machine, return_type, scanner->token.token);
	}

	if (_SUBC_REPORT_PROGRESS(_This))
	{
		machine->printf(_This->machine, "SubC: compiling callable unit: %s\n", scanner->token.token);
	}

//	machine->set_callable_unit_symbol(machine, function->idx, scanner->token.token);

	scanner->move_next_token(scanner);

	int rsp = _REG_RSP(_REGS(machine));

	_SUBC_METHOD_NAME(compile_function_parameters)(_This, _State, function);

	if (!scanner->try_consume_token(scanner, TOKEN_OPEN_BRACE))
	{
		scanner->consume_token(scanner, TOKEN_SEMI_COLON);

		if (_SUBC_REPORT_PROGRESS(_This))
		{
			_This->machine->printf(_This->machine, "SubC: declared callable unit: %s\n", machine->get_callable_unit_symbol(machine, function->idx));
		}

		_SET_RSP(_REGS(machine), rsp);

		return;
	}

	function->entry_point_program_counter = _REG_FCP(_REGS(machine));
	function->is_resolved = 1;

	_SUBC_METHOD_NAME(compile_block)(_This, _State);
	if (!_State->fatal)
	{
		scanner->consume_token(scanner, TOKEN_CLOSE_BRACE);

		_SET_RSP(_REGS(machine), rsp);

		// todo crz: handle checking return type
		switch (return_type)
		{
		case TYPE_INT:
			emitter->emit_instruction(emitter, H6VM_OPCODE_RETURN_INTEGER, 0);
			break;
		case TYPE_VOID:
			emitter->emit_instruction(emitter, H6VM_OPCODE_RETURN, 0);
			break;
		case TYPE_FLOAT:
			emitter->emit_instruction(emitter, H6VM_OPCODE_RETURN_FLOAT, 0);
			emitter->emit_float(emitter, 0.0f);
			break;
		default:
			strcpy(_State->expression_parser_state->message, "unsupported return type");
			THROW(&_State->expression_parser_state->exception, 1); // todo crz: proper fault code
		}

		function->exit_point_program_counter = _REG_FCP(_REGS(machine)) - 1;

		if (_SUBC_REPORT_PROGRESS(_This))
		{
			_This->machine->printf(_This->machine, "SubC: compiled callable unit: %s\n", machine->get_callable_unit_symbol(machine, function->idx));
		}
	}
	else
	{
		if (_SUBC_REPORT_PROGRESS(_This))
		{
			_This->machine->printf(_This->machine, "SubC: fatal error, unable to continue compiling callable unit");
		}
	}
}

_SUBC_METHOD_CONST(void, compile_return)
{
	struct instruction_emitter_t *emitter = &_This->machine->instruction_emitter;
	struct scanner_t *scanner = _This->scanner;

	if (scanner->try_consume_token(scanner, TOKEN_SEMI_COLON))
	{
		// todo crz: type check this
		emitter->emit_instruction(emitter, H6VM_OPCODE_RETURN, 0);
		return;
	}

	struct expression_t expression;
	init_new_expression(_This->machine, &expression);
	_State->expression_parser_state->parser->parse_expression(_State->expression_parser_state->parser, _State->expression_parser_state, &expression);

	scanner->consume_token(scanner, TOKEN_SEMI_COLON);

	emitter->emit_instruction(emitter, H6VM_OPCODE_EVAL, expression.first_term_index);
	emitter->emit_instruction(emitter, H6VM_OPCODE_RETURN, 0);
}

