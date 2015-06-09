/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

//#include "6IT.h"
//#include "6OO.h"

#include "6OO_reader.c" // hack crz

_6OO_METHOD(int, tokenise)
{
	// todo crz: make this load lazily
	if (!(_This->source = scanner_load_program(_This)))
	{
		return 0;
	}

	construct_token(&_This->token);

	int i = 0;

	_This->state.line_number = 0;
	_This->state.source_offset = 0;
	_This->state.current_char = _This->source[_This->state.source_offset];

	_This->lex_next_token(_This);

	return 1;
}

_6OO_METHOD(void, lex_next_token)
{
	_This->token.token[0] = 0;
	_This->token.type = TOKEN_TYPE_INVALID;
	_This->token.id = 0;
	_This->token.source_offset = _This->token.next_token_source_offset;

	_This->move_past_whitespace(_This);

	_This->token.source_line_number = _This->state.line_number;
	_This->token.source_offset = _This->state.source_offset;

	if (_This->is_eof(_This))
	{
		_This->token.type = TOKEN_TYPE_EOF;
	}
	else if (_This->is_digit(_This))
	{
		_This->extract_numeric_literal(_This);
		_This->token.type = TOKEN_TYPE_NUMBER;
	}
	else if (_This->is_alpha(_This))
	{
		_This->extract_alphanumeric(_This);
		_This->token.type = TOKEN_TYPE_IDENTIFIER;
		_This->token.id = 0;
	}
	else if (_This->is_char(_This, '\''))
	{
		_This->extract_char_literal(_This);
		_This->token.type = TOKEN_TYPE_CHAR;
	}
	else if (_This->is_char(_This, '\"'))
	{
		int index = _REG_FSP(_REGS(_This->machine));
		_This->extract_string_literal(_This);
		_This->token.type = TOKEN_TYPE_STRING;
		_This->token.id = index;
	}
	else
	{
		_This->token.id = _This->match_next_token(_This);

		if (0 == _This->token.id)
		{
			_This->print_token(_This, &_This->token);
			_This->die(_This, ERROR_SCANNER_UNRECOGNISED_TOKEN, "unrecognised token\n");
		}

		_This->token.type = TOKEN_TYPE_SYMBOL;

		char const *s = _This->get_token_string(_This, _This->token.id);
		if (s)
		{
			// todo crz: what's going on here?
//			extract_chars(scanner, (int)strlen(s));
		}
		else
		{
			_This->die(_This, ERROR_SCANNER_TOKEN_STRING_NOT_FOUND, "token not found");
		}
	}

	_This->token.next_token_source_offset = _This->state.source_offset;

	if (DEBUG_SHOW_TOKENS(_This))
	{
		_This->print_token(_This, &_This->token);
	}
}


