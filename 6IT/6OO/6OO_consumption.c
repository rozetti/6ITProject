/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6OO.h"

#include <string.h>

_6OO_METHODX(void, consume_token, token_id_t required)
{
	_This->check_token(_This, required);
	_This->move_next_token(_This);
}

_6OO_METHODX(void, consume_string, char const *s)
{
	if (_This->token.type != TOKEN_TYPE_IDENTIFIER)
	{
		_This->die(_This, ERROR_SCANNER_IDENTIFIER_EXPECTED, "unexpected token");
	}

	if (!strcmp(s, _This->token.token))
	{
		_This->move_next_token(_This);
	}

	_This->die(_This, ERROR_SCANNER_WRONG_IDENTIFIER, "unexpected token");
}

_6OO_METHODX(void, consume_symbol, token_id_t required)
{
	_This->check_token_type(_This, TOKEN_TYPE_SYMBOL);
	_This->check_token(_This, required);
	_This->move_next_token(_This);
}

_6OO_METHODX(int, try_consume_token, token_id_t required)
{
	if (_This->token.id == required)
	{
		_This->move_next_token(_This);
		return 1;
	}

	return 0;
}

_6OO_METHODX(int, try_consume_string, char const *s)
{
	if (_This->token.type != TOKEN_TYPE_IDENTIFIER)
	{
		return 0;
	}

	if (!strcmp(s, _This->token.token))
	{
		_This->move_next_token(_This);
		return 1;
	}

	return 0;
}

_6OO_METHODX(int, try_consume_symbol, token_id_t required)
{
	if (_This->token.type != TOKEN_TYPE_SYMBOL)
	{
		return 0;
	}

	if (_This->token.id == required)
	{
		_This->move_next_token(_This);
		return 1;
	}

	return 0;
}
