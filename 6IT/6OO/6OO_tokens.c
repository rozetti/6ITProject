/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6OO.h"

#include <stdio.h>

static char const *_6OO_get_token_string(struct scanner_t *scanner, int id)
{
	int i = 0;
	while (scanner->token_strings[i].id)
	{
		if (id == scanner->token_strings[i].id) return scanner->token_strings[i].string;
		++i;
	}

	return 0;
}

static char const *_6OO_get_token_type_string(token_type_t type)
{
	switch (type)
	{
	case TOKEN_TYPE_INVALID: return "INVALID";
	case TOKEN_TYPE_SYMBOL: return "SYMBOL";
	case TOKEN_TYPE_IDENTIFIER: return "IDENTIFIER";
	case TOKEN_TYPE_NUMBER: return "NUMBER";
	case TOKEN_TYPE_STRING: return "STRING";
	}

	return "UNKNOWN";
}

struct token_string_t _6OO_default_token_strings[] =
{
	{TOKEN_LESS_LESS_EQUALS, "<<="},
	{TOKEN_GREATER_GREATER_EQUALS, ">>="},

	{TOKEN_EQUALS_EQUALS, "=="},
	{TOKEN_BANG_EQUALS, "!="},
	{TOKEN_GREATER_THAN_OR_EQUAL, ">="},
	{TOKEN_LESS_THAN_OR_EQUAL, "<="},
	{TOKEN_PLUSPLUS, "++"},
	{TOKEN_MINUSMINUS, "--"},
	{TOKEN_ANDAND, "&&"},
	{TOKEN_PIPEPIPE, "||"},
	{TOKEN_LESS_LESS, "<<"},
	{TOKEN_GREATER_GREATER, ">>"},

	{TOKEN_PLUS_EQUALS, "+="},
	{TOKEN_MINUS_EQUALS, "-="},
	{TOKEN_ASTERISK_EQUALS, "*="},
	{TOKEN_FORWARD_SLASH_EQUALS, "/="},
	{TOKEN_PERCENT_EQUALS, "%="},
	{TOKEN_AND_EQUALS, "&="},
	{TOKEN_PIPE_EQUALS, "|="},
	{TOKEN_HAT_EQUALS, "^="},

	{TOKEN_OPEN_BRACE, "{"},
	{TOKEN_CLOSE_BRACE, "}"},
	{TOKEN_EQUALS, "="},
	{TOKEN_PLUS, "+"},
	{TOKEN_MINUS, "-"},
	{TOKEN_ASTERISK, "*"},
	{TOKEN_FORWARD_SLASH, "/"},
	{TOKEN_PERCENT, "%"},
	{TOKEN_DOT, "."},
	{TOKEN_LESS_THAN, "<"},
	{TOKEN_GREATER_THAN, ">"},
	{TOKEN_SEMI_COLON, ";"},
	{TOKEN_COMMA, "},"},
	{TOKEN_OPEN_PAREN, "("},
	{TOKEN_CLOSE_PAREN, ")"},
	{TOKEN_OPEN_SQUARE_BRACKET, "["},
	{TOKEN_CLOSE_SQUARE_BRACKET, "]"},
	{TOKEN_HAT, "^"},
	{TOKEN_BANG, "!"},
	{TOKEN_HASH, "#"},
	{TOKEN_QUESTION_MARK, "?"},
	{TOKEN_COLON, ":"},
	{TOKEN_AT, "@"},
	{TOKEN_TILDE, "~"},
	{TOKEN_AND, "&"},
	{TOKEN_PIPE, "|"},
	{0, 0}
};

// todo crz: look at why this has old-style signature
void _6OO_print_token(struct scanner_t *scanner, struct token_t const *token)
{
	char s[1000];

	if (token->source_offset < 0)
	{
		scanner->die(scanner, ERROR_SCANNER_TOKENISER_GONE_WRONG, "token index out of range");
	}

	switch (token->type)
	{
	case TOKEN_TYPE_NUMBER:
		sprintf(s, "token %d: %s, type: NUMBER, id: %i, next: %d, line: %d\n",
			token->source_offset,
			token->token,
			token->id,
			token->next_token_source_offset,
			token->source_line_number);
		break;
	case TOKEN_TYPE_IDENTIFIER:
		sprintf(s, "token %d: %s, type: IDENTIFIER, id: %i, next: %d, line: %d\n",
			token->source_offset,
			token->token,
			token->id,
			token->next_token_source_offset,
			token->source_line_number);
		break;
	case TOKEN_TYPE_STRING:
		sprintf(s, "token %d: \"%s\", type: STRING, id: %i, next: %d, line: %d\n",
			token->source_offset,
			_REG_SS(_REGS(scanner->machine)) + token->id,
			token->id,
			token->next_token_source_offset,
			token->source_line_number);
		break;
	case TOKEN_TYPE_SYMBOL:
		sprintf(s, "token %d: '%s', type: SYMBOL, id: %i, next: %d, string: \"%s\", line: %d\n",
			token->source_offset,
			token->token,
			token->id,
			token->next_token_source_offset,
			scanner->get_token_string(scanner, token->id),
			token->source_line_number);
		break;
	default:
		sprintf(s, "token %d: '%s', type: %s, id: %i, next: %d, string: %s, line: %d\n",
			token->source_offset,
			token->token,
			scanner->get_token_type_string(token->type),
			token->id,
			token->next_token_source_offset,
			scanner->get_token_string(scanner, token->id),
			token->source_line_number);
	}

	scanner->environment->output_string(s);
}

