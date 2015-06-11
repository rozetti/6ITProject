/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6OO.h"

#include <string.h>
#include <ctype.h>
#include <stdio.h>

_6OO_METHOD_CONST(int, is_eof)
{
	int eof = 0;

	if (_This->is_char(_This, 0)) eof = 1;

	return eof;
}

_6OO_METHOD_CONST(int, is_eol)
{
	if (_This->is_char(_This, '\n')) return 1;
	if (_This->is_char(_This, '\r')) return 1;

	return 0;
}

_6OO_METHOD_CONST(int, is_whitespace)
{
	if (_This->is_char(_This, ' ')) return 1;
	if (_This->is_char(_This, '\t')) return 1;
	if (_This->is_char(_This, '\n')) return 1;
	if (_This->is_char(_This, '\r')) return 1;

	return 0;
}

_6OO_METHODX_CONST(int, is_char, char ch)
{
	return _This->state.current_char == ch;
}

// todo crz: move to matching
_6OO_METHODX(int, matches_string, char const *s)
{
	if (_This->state.current_char != s[0]) return 0;

	struct scanner_state_t scanner_save = _This->state;

	for (size_t i = 1; i < strlen(s); ++i)
	{
		_This->move_next_char(_This);

		if (_This->state.current_char != s[i])
		{
			_This->state = scanner_save;
			return 0;
		}
	}

	_This->state = scanner_save;

	return 1;
}

_6OO_METHOD_CONST(int, is_alpha)
{
	return isalpha(_This->state.current_char);
}

_6OO_METHOD_CONST(int, is_digit)
{
	return isdigit(_This->state.current_char);
}

_6OO_METHOD_CONST(int, is_alphanumeric)
{
	return isalnum(_This->state.current_char) || _This->is_char(_This, '_');
}

_6OO_METHODX_CONST(int, is_token, int id)
{
	return id == _This->token.id;
}

_6OO_METHODX(void, check_token, token_id_t required)
{
	if (_This->token.id == required) return;

	char s[100];
	sprintf(s, "expected token '%s', got '%s'\n",
		_This->get_token_string(_This, (int)required),
		_This->get_token_string(_This, _This->token.id));
	_This->environment->output_string(s);

	_This->die(_This, ERROR_SCANNER_UNEXPECTED_TOKEN, "unexpected token");
}

_6OO_METHODX(void, check_token_type, token_id_t type)
{
	if (_This->token.type == type) return;

	_This->die(_This, ERROR_SCANNER_UNEXPECTED_TOKEN_TYPE, "unexpected token");
}

_6OO_METHODX(int, match_token, char const *token)
{
	int i = 0;
	while (_This->token_strings[i].id)
	{
		if (!strcmp(_This->token_strings[i].string, token))
		{
			strcpy(_This->token.token, token);

			return _This->token_strings[i].id;
		}
		++i;
	}

	return 0;
}

_6OO_METHOD(int, match_next_token)
{
	int i = 0;
	while (_This->token_strings[i].id)
	{
		struct scanner_state_t state = _This->state;

		char const *s = _This->token_strings[i].string;

		size_t c;
		for (c = 0; c < strlen(s) && _This->state.current_char == s[c]; ++c)
		{
			_This->move_next_char(_This);
		}

		if (c == strlen(s))
		{
			strcpy(_This->token.token, s);
			return _This->token_strings[i].id;
		}

		_This->state = state;
		++i;
	}

	return 0;
}
