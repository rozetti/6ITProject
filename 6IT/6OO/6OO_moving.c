/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

//#include "6IT.h"

SCANNER_METHOD(int, move_next_char)
{
	if (_This->is_eof(_This)) return 0;

	++_This->state.source_offset;
	_This->state.current_char = _This->source[_This->state.source_offset];

	if (_This->is_char(_This, '\n')) ++_This->state.line_number; // todo crz: improve this

	return _This->state.current_char;
}

SCANNER_METHOD(int, move_previous_char)
{
	--_This->state.source_offset;

	_This->state.current_char = _This->source[_This->state.source_offset];

	return _This->state.current_char;
}

SCANNER_METHOD(void, move_past_whitespace)
{
	while (_This->is_whitespace(_This) && _This->move_next_char(_This));
}

SCANNER_METHOD(void, move_to_end_of_line)
{
	while (!_This->is_eol(_This) && _This->move_next_char(_This));
	while (_This->is_eol(_This) && _This->move_next_char(_This));
}

SCANNER_METHOD(void, move_next_token)
{
	_This->lex_next_token(_This);
}

SCANNER_METHOD(void, move_next_line)
{
	int line_number = _This->token.source_line_number;
	while (_This->token.source_line_number == line_number)
	{
		_This->move_next_token(_This);
	}
}

// hack cgd: remove the need for this
SCANNER_METHODX(void, set_token, struct token_t token)
{
	_This->token = token;

	_This->state.source_offset = token.next_token_source_offset;
	_This->state.line_number = token.source_line_number;
	_This->state.current_char = _This->source[_This->state.source_offset];
}

//void _6OO_bind_moving_methods(struct scanner_t *scanner)
//{
//	scanner->move_next_char = move_next_char;
//	scanner->move_next_line = move_next_line;
//	scanner->move_next_token = move_next_token;
//	scanner->move_past_whitespace = move_past_whitespace;
//	scanner->move_previous_char = move_previous_char;
//	scanner->move_to_end_of_line = move_to_end_of_line;
//	scanner->set_token = set_token;
//}