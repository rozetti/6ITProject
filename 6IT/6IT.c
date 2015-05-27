/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <memory.h>

void construct_token(struct token_t *token)
{
	token->source_offset = 0;
	token->next_token_source_offset = 0;
	token->source_line_number = 0;
}

struct environment_t construct_environment()
{
	struct environment_t env;
	memset(&env, 0, sizeof(struct environment_t));
	return env;
}

void construct_expression(struct expression_t *expression)
{
	memset(expression, 0, sizeof(struct expression_t));

	expression->source_start = INT32_MAX;
	expression->source_end = 0;
	expression->is_global = 0;
}

// todo crz: meh
void init_new_expression(struct machine_t *machine, struct expression_t *expression)
{
	construct_expression(expression);

	expression->first_term_index = _REG_FXP(_REGS(machine));
	expression->terms = _REG_XS(_REGS(machine)) + expression->first_term_index;
}

void construct_expression_parser_state(struct expression_parser_state_t *state, struct expression_parser_t *parser)
{
	memset(state, 0, sizeof(struct expression_parser_state_t));

	state->parser = parser;

	state->use_immediates = 1;
	state->numbers_are_floats = 0;
}

