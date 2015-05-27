/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

struct token_t
{
	char token[MAX_TOKEN_LENGTH];
	token_type_t type;
	token_id_t id;
	int source_offset;
	int next_token_source_offset;
	int source_line_number;
};

struct token_string_t
{
	int id;
	char const *string;
};

struct scanner_state_t
{
	int source_offset;
	int line_number;
	char current_char;
};

struct scanner_t
{
	DEFINE_COMMON_ATTRIBUTES(scanner);

	int(*tokenise)(struct scanner_t*);

	char const *source;

	struct token_t token;
	struct scanner_state_t state;

	struct token_string_t *token_strings;
	int max_number_of_tokens;

	void(*lex_next_token)(struct scanner_t*);

	// crz: moving

	int(*move_next_char)(struct scanner_t*);
	int(*move_previous_char)(struct scanner_t*);
	void(*move_to_end_of_line)(struct scanner_t*);
	void(*move_next_token)(struct scanner_t*);
	void(*move_past_whitespace)(struct scanner_t*);
	void(*move_next_line)(struct scanner_t*);

	// crz: classification

	int(*is_token)(struct scanner_t const*, int id);

	int(*is_eof)(struct scanner_t const*);
	int(*is_eol)(struct scanner_t const*);
	int(*is_whitespace)(struct scanner_t const*);
	int(*is_char)(struct scanner_t const*, char ch);
	int(*is_alpha)(struct scanner_t const*);
	int(*is_digit)(struct scanner_t const*);
	int(*is_alphanumeric)(struct scanner_t const*);
	int(*matches_string)(struct scanner_t*, char const *s);

	// crz: extraction

	void(*extract_numeric_literal)(struct scanner_t*);
	void(*extract_string_literal)(struct scanner_t*);
	void(*extract_char_literal)(struct scanner_t*);
	void(*extract_alphanumeric)(struct scanner_t*);
	void(*extract_char)(struct scanner_t*);
	void(*extract_chars)(struct scanner_t*, int n);

	// crz: strings

	char const *(*get_token_string)(struct scanner_t*, int id);
	char const *(*get_token_type_string)(token_type_t type);

	void(*check_token)(struct scanner_t*, token_id_t required);
	void(*check_token_type)(struct scanner_t*, token_id_t type);
	void(*consume_token)(struct scanner_t*, token_id_t required);
	void(*consume_symbol)(struct scanner_t*, token_id_t required);
	void(*consume_string)(struct scanner_t*, char const *s);
	int(*try_consume_token)(struct scanner_t*, token_id_t required);
	int(*try_consume_symbol)(struct scanner_t*, token_id_t required);
	int(*try_consume_string)(struct scanner_t*, char const *s);

	void(*set_token)(struct scanner_t*, struct token_t);
	void(*advance_token)(struct scanner_t*);
	int(*match_token)(struct scanner_t*, char const*);
	int(*match_next_token)(struct scanner_t*);
	void(*print_token)(struct scanner_t*, struct token_t const*);
};
