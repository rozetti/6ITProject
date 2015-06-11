/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6OO.h"

#include <string.h>

_6OO_METHOD(void, extract_string_literal)
{
	if (!_This->is_char(_This, '\"'))
	{
		_This->die(_This, ERROR_SCANNER_STRING_LITERAL_MISSING_INITIAL_QUOTE, "string literal missing initial quote");
		return;
	}

	if (!_This->move_next_char(_This))
	{
		_This->die(_This, ERROR_SCANNER_EOF_IN_STRING_LITERAL, "EOF in string literal");
		return;
	}

	int i = 0;
	while (!_This->is_char(_This, '\"'))
	{
		if (_REG_FSP(_REGS(_This->machine)) > _This->machine->config.string_segment_size)
		{
			_This->die(_This, ERROR_SCANNER_STRING_STORE_OVERFLOW, "string store overflow");
			return;
		}

		_REG_SS(_REGS(_This->machine))[_REG_FSP(_REGS(_This->machine))] = _This->state.current_char;
		_INC_FSP(_REGS(_This->machine));

		if (!_This->move_next_char(_This))
		{
			_This->die(_This, ERROR_SCANNER_EOF_IN_STRING_LITERAL, "EOF in string literal");
			return;
		}

		if (_This->is_eol(_This))
		{
			_This->die(_This, ERROR_SCANNER_EOL_IN_STRING_LITERAL, "end of line in string literal");
			return;
		}
	}

	_REG_SS(_REGS(_This->machine))[_REG_FSP(_REGS(_This->machine))] = 0;
	_INC_FSP(_REGS(_This->machine));

	_This->move_next_char(_This);
}

_6OO_METHOD(void, extract_char_literal)
{
	if (!_This->is_char(_This, '\''))
	{
		_This->die(_This, ERROR_SCANNER_CHAR_LITERAL_MISSING_INITIAL_QUOTE, "char literal missing initial single quote");
		return;
	}

	if (!_This->move_next_char(_This))
	{
		_This->die(_This, ERROR_SCANNER_EOF_IN_CHAR_LITERAL, "EOF in char literal");
		return;
	}

	_This->token.token[0] = _This->state.current_char;
	_This->token.token[1] = 0;

	if (!_This->move_next_char(_This))
	{
		_This->die(_This, ERROR_SCANNER_EOF_IN_CHAR_LITERAL, "EOF in char literal");
		return;
	}

	if (!_This->is_char(_This, '\''))
	{
		_This->die(_This, ERROR_SCANNER_CHAR_LITERAL_MISSING_CLOSING_QUOTE, "char literal missing closing single quote");
		return;
	}

	_This->move_next_char(_This);
}

_6OO_METHOD(void, extract_numeric_literal)
{
	int i = 0;

	while (_This->is_digit(_This) || (i > 0 && _This->is_char(_This, '.')))
	{
		_This->token.token[i++] = _This->source[_This->state.source_offset];
		if (!_This->move_next_char(_This)) break;
	}

	_This->token.token[i] = 0;
}

_6OO_METHOD(void, extract_alphanumeric)
{
	int i = 0;

	while (_This->is_alphanumeric(_This))
	{
		_This->token.token[i++] = _This->source[_This->state.source_offset];
		if (!_This->move_next_char(_This)) break;
	}

	_This->token.token[i] = 0;
}

_6OO_METHOD(void, extract_char)
{
	if (_This->is_eof(_This)) return;

	_This->token.token[0] = _This->state.current_char;
	_This->token.token[1] = 0;
	_This->move_next_char(_This);
}

_6OO_METHODX(void, extract_chars, int n)
{
	int i = 0;
	for (; i < n; ++i)
	{
		_This->token.token[i] = _This->state.current_char;
		if (!_This->move_next_char(_This)) break;
	}

	_This->token.token[i] = 0;
}
