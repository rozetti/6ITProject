/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

EXPRESSION_EMITTER_METHODXXX(void, append_term, struct expression_t *expression, struct expression_term_t *term, int source_position)
{
	expression->terms[expression->number_of_terms] = *term;

	++expression->number_of_terms;

	if (source_position < expression->source_start)
	{
		expression->source_start = source_position;
	}

	if (source_position > expression->source_end)
	{
		expression->source_end = source_position;
	}
}

EXPRESSION_EMITTER_METHODXXX(void, emit_binary_operator, struct expression_t *expression, int op, int source_position)
{
	struct expression_term_t term;
	term.type = op;
	term.data.as_integer = 0;

	_This->append_term(_This, expression, &term, source_position);
}

EXPRESSION_EMITTER_METHODXXX(void, emit_opcode, struct expression_t *expression, int op, int source_position)
{
	struct expression_term_t term;
	term.type = op;
	term.data.as_integer = 0;
	_This->append_term(_This, expression, &term, source_position);
}

EXPRESSION_EMITTER_METHODXXX(void, emit_unary_operator, struct expression_t *expression, int op, int source_position)
{
	struct expression_term_t term;
	term.type = op;
	term.data.as_integer = 0;
	_This->append_term(_This, expression, &term, source_position);
}

EXPRESSION_EMITTER_METHODXXXX(void, emit_frame_register_assignment, struct expression_t *expression, struct register_t const *reg, int op, int source_position)
{
	int idx = reg->local_idx;

	struct expression_term_t term;

	term.data.as_integer = idx;
	term.type = op;
	term.data.type = reg->value.type;

	_This->append_term(_This, expression, &term, source_position);
}

EXPRESSION_EMITTER_METHODXXXX(void, emit_static_register_assignment, struct expression_t *expression, struct register_t const *reg, int op, int source_position)
{
	int idx = reg->idx;

	struct expression_term_t term;

	term.data.as_integer = idx;
	term.type = op;
	term.data.type = reg->value.type;

	_This->append_term(_This, expression, &term, source_position);
}

EXPRESSION_EMITTER_METHODXX(void, emit_finished, struct expression_t *expression, int source_position)
{
	struct expression_term_t term;
	
	term.type = EXPRESSION_TERMINATOR;
	term.data.as_integer = 0;

	_This->append_term(_This, expression, &term, source_position);
}

_6IT_PUBLIC _6IT_CONSTRUCTOR(expression_emitter, expression_emitter)
{
	_This->machine = _Machine;

	_This->append_term = append_term;

	_This->emit_finished = emit_finished;
	_This->emit_opcode = emit_opcode;
	_This->emit_binary_operator = emit_binary_operator;
	_This->emit_unary_operator = emit_unary_operator;
	_This->emit_frame_register_assignment = emit_frame_register_assignment;
	_This->emit_static_register_assignment = emit_static_register_assignment;
}
