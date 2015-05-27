/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <memory.h>

static struct expression_term_t *get_final_expression_term(struct machine_t *machine, struct expression_t *expression)
{
	return expression->terms + expression->number_of_terms - 1;
}

static void machine_build_expression(struct machine_t *machine, struct expression_t *expression, int ptr)
{
	expression->terms = _REG_XS(_REGS(machine)) + ptr;
	expression->first_term_index = ptr;
	expression->number_of_terms = 0;

	struct expression_term_t *term = expression->terms;

	while (term->type != EXPRESSION_TERMINATOR)
	{
		++term;
	}

	expression->number_of_terms = term - expression->terms;
}

