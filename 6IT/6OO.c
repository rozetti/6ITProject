/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6OO.h"

#include "6OO/6OO_classification.c"
#include "6OO/6OO_extraction.c"
#include "6OO/6OO_consumption.c"
#include "6OO/6OO_moving.c"
#include "6OO/6OO_tokeniser.c"
#include "6OO/6OO_tokens.c"

_6IT_DESTRUCTOR(scanner) { }

_6IT_THISCALL_DIE(scanner)
{
	_This->environment->output_string(message);
	_This->environment->output_string("\n");

	THROW((&_This->exception), fault_code);
}

_6IT_THISCALL_GET_ERROR_STRING(scanner)
{
	switch (error)
	{
	case ERROR_SCANNER_UNEXPECTED_TOKEN:
		return "Unexpected token";
	case ERROR_SCANNER_UNEXPECTED_TOKEN_TYPE:
		return "Unexpected token type";

	case ERROR_SCANNER_CHAR_LITERAL_MISSING_INITIAL_QUOTE:
		return "Char literal missing initial quote";
	case ERROR_SCANNER_CHAR_LITERAL_MISSING_CLOSING_QUOTE:
		return "Char literal missing closing quote";
	case ERROR_SCANNER_EOF_IN_CHAR_LITERAL:
		return "End of file in char literal";

	case ERROR_SCANNER_EOF_IN_STRING_LITERAL:
		return "End of file in string literal";
	case ERROR_SCANNER_EOL_IN_STRING_LITERAL:
		return "Line break in string literal";
	case ERROR_SCANNER_STRING_LITERAL_MISSING_INITIAL_QUOTE:
		return "String literal missing initial quote";
	case ERROR_SCANNER_STRING_LITERAL_MISSING_CLOSING_QUOTE:
		return "String literal missing closing quote";
	case ERROR_SCANNER_STRING_STORE_OVERFLOW:
		return "String store overflow";

	case ERROR_SCANNER_IDENTIFIER_EXPECTED:
		return "Identifier expected";
	case ERROR_SCANNER_WRONG_IDENTIFIER:
		return "Wrong identifier found";
	case ERROR_SCANNER_UNRECOGNISED_TOKEN:
		return "Unrecognised token";
	case ERROR_SCANNER_TOKEN_STRING_NOT_FOUND:
		return "Token string not found";
	case ERROR_SCANNER_TOKENISER_GONE_WRONG:
		return "Something bad happened in the tokeniser";
	case ERROR_SCANNER_EXTERNAL_ERROR:
		return "External error";
	}

	return 0;
}

_6IT_CONSTRUCTOR(scanner, 6OO)
{
	INITIALISE_COMMON_ATTRIBUTES(scanner, _Machine);

	_This->get_token_string = get_token_string;
	_This->get_token_type_string = get_token_type_string;

	_This->tokenise = tokenise;
	_This->lex_next_token = lex_next_token;
	_This->print_token = _6OO_print_token;
	_This->token_strings = _6OO_default_token_strings;

	_This->is_alpha = is_alpha;
	_This->is_alphanumeric = is_alphanumeric;
	_This->is_char = is_char;
	_This->is_digit = is_digit;
	_This->is_eof = is_eof;
	_This->is_eol = is_eol;
	_This->is_token = is_token;
	_This->is_whitespace = is_whitespace;
	_This->matches_string = matches_string;
	_This->check_token = check_token;
	_This->check_token_type = check_token_type;
	_This->match_token = match_token;
	_This->match_next_token = match_next_token;

	_This->extract_alphanumeric = extract_alphanumeric;
	_This->extract_char = extract_char;
	_This->extract_chars = extract_chars;
	_This->extract_char_literal = extract_char_literal;
	_This->extract_numeric_literal = extract_numeric_literal;
	_This->extract_string_literal = extract_string_literal;

	_This->move_next_char = move_next_char;
	_This->move_next_line = move_next_line;
	_This->move_next_token = move_next_token;
	_This->move_past_whitespace = move_past_whitespace;
	_This->move_previous_char = move_previous_char;
	_This->move_to_end_of_line = move_to_end_of_line;
	_This->set_token = set_token;

	_This->try_consume_string = try_consume_string;
	_This->consume_string = consume_string;
	_This->try_consume_symbol = try_consume_symbol;
	_This->consume_symbol = consume_symbol;
	_This->try_consume_token = try_consume_token;
	_This->consume_token = consume_token;
}

