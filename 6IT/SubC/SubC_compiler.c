/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include "SubC.h"
#include "6IL/6IL_opcodes.h"

_SUBC_METHOD_CONST(void, compile_single_statement)
{
	if (CATCH(&_This->machine->exception))
	{
		++_State->number_of_errors;
		// todo crz: try to skip remainder of statement, to allow compilation to resume
		_State->fatal = 1;
		return;
	}

	struct scanner_t *scanner = _This->scanner;

	if (scanner->try_consume_string(scanner, "return"))
	{
		_SUBC_METHOD_NAME(compile_return)(_This, _State);
	}
	else if (scanner->try_consume_string(scanner, "assert"))
	{
		_SUBC_METHOD_NAME(compile_assert)(_This, _State);
		scanner->consume_token(scanner, TOKEN_SEMI_COLON);
	}
	else if (scanner->try_consume_string(scanner, "if"))
	{
		_SUBC_METHOD_NAME(compile_if)(_This, _State);
	}
	else if (scanner->try_consume_string(scanner, "break"))
	{
		_SUBC_METHOD_NAME(compile_break)(_This, _State);
	}
	else if (scanner->try_consume_string(scanner, "continue"))
	{
		_SUBC_METHOD_NAME(compile_continue)(_This, _State);
	}
	else if (scanner->try_consume_string(scanner, "while"))
	{
		_SUBC_METHOD_NAME(compile_while)(_This, _State);
	}
	else if (scanner->try_consume_string(scanner, "do"))
	{
		_SUBC_METHOD_NAME(compile_do)(_This, _State);
	}
	else if (scanner->try_consume_string(scanner, "for"))
	{
		_SUBC_METHOD_NAME(compile_for)(_This, _State);
	}
	else
	{
		_SUBC_METHOD_NAME(compile_expression)(_This, _State);
		scanner->consume_token(scanner, TOKEN_SEMI_COLON);
	}
}

_SUBC_METHOD_CONST(void, compile_statements)
{
	while ((_This->scanner->token.id != TOKEN_CLOSE_BRACE) && (!_State->fatal))
	{
		_SUBC_METHOD_NAME(compile_single_statement)(_This, _State);
	}
}

_SUBC_METHOD_CONST(void, compile_block)
{
	int rsp = _REG_RSP(_REGS(_This->machine));
	_SUBC_METHOD_NAME(compile_statements)(_This, _State);
	_SET_RSP(_REGS(_This->machine), rsp);
}

_SUBC_METHOD_CONST(void, compile_single_statement_scope)
{
	int rsp = _REG_RSP(_REGS(_This->machine));
	_SUBC_METHOD_NAME(compile_single_statement)(_This, _State);
	_SET_RSP(_REGS(_This->machine), rsp);
}

_6IT_PRIVATE int _SUBC_METHOD_NAME(compile)(struct compiler_t *_This)
{
	struct compiler_state_t state;
	memset(&state, 0, sizeof(struct compiler_state_t));

	// todo crz: make this lazy
	if (!_This->scanner->tokenise(_This->scanner))
	{
		return 0;
	}

	struct expression_parser_t expression_parser;
	construct_6X(&expression_parser, _This->machine, _This->scanner);

	struct expression_parser_state_t expression_parser_state;
	construct_expression_parser_state(&expression_parser_state, &expression_parser);

	state.frame = _REG_ES(_REGS(_This->machine));
	state.expression_parser_state = &expression_parser_state;
	state.loop_PC = -1;
	state.number_of_errors = 0;
	state.number_of_warnings = 0;
	state.fatal = 0;

	_SUBC_METHOD_NAME(compile_program)(_This, &state);

	if (state.number_of_errors != 0)
	{
		return 0;
	}

	return 1;
}

