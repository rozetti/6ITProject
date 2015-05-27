/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

//#include "6IT.h"
//#include "6OO.h"

#include <string.h>

SCANNER_METHODX(void, consume_token, token_id_t required)
{
	_This->check_token(_This, required);
	_This->move_next_token(_This);
}

SCANNER_METHODX(void, consume_string, char const *s)
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

SCANNER_METHODX(void, consume_symbol, token_id_t required)
{
	_This->check_token_type(_This, TOKEN_TYPE_SYMBOL);
	_This->check_token(_This, required);
	_This->move_next_token(_This);
}

SCANNER_METHODX(int, try_consume_token, token_id_t required)
{
	if (_This->token.id == required)
	{
		_This->move_next_token(_This);
		return 1;
	}

	return 0;
}

SCANNER_METHODX(int, try_consume_string, char const *s)
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

SCANNER_METHODX(int, try_consume_symbol, token_id_t required)
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

//void _6OO_bind_consumption_methods(struct scanner_t *scanner)
//{
//	scanner->try_consume_string = try_consume_string;
//	scanner->consume_string = consume_string;
//	scanner->try_consume_symbol = try_consume_symbol;
//	scanner->consume_symbol = consume_symbol;
//	scanner->try_consume_token = try_consume_token;
//	scanner->consume_token = consume_token;
//}