/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6X.h"
#include "6EV/6EV_opcodes.h" // todo crz: please try to decouple this

_6X_METHODXXX(void, emit_string, struct expression_t *expression, int index, int source_position)
{
	struct expression_emitter_t *emitter = &_This->machine->expression_emitter;

	struct expression_term_t term;
	
	term.type = EVALUATOR_OPCODE_PUSH_STRING;
	term.data.as_integer = index;
	term.data.type = TYPE_CHAR_CONST_PTR;

	emitter->append_term(emitter, expression, &term, source_position);
}

_6X_METHODXXX(void, emit_variable, struct expression_t *expression, struct register_t const *var, int source_position)
{
	struct expression_emitter_t *emitter = &_This->machine->expression_emitter;

	struct expression_term_t term;

	if (var->local_idx < 0)
	{
		term.type = EVALUATOR_OPCODE_PUSH_SREG;
		term.data.as_integer = var->idx;
	}
	else
	{
		term.type = EVALUATOR_OPCODE_PUSH_FREG;
		term.data.as_integer = var->local_idx;
	}

	term.data.type = var->value.type;

	emitter->append_term(emitter, expression, &term, source_position);
}

_6X_METHODXXX(void, emit_float, struct expression_t *expression, float value, int source_position)
{
	struct expression_emitter_t *emitter = &_This->machine->expression_emitter;

	struct expression_term_t term;

	term.type = EVALUATOR_OPCODE_PUSH_FLOAT;
	term.data.as_float = value;
	term.data.type = TYPE_FLOAT;

	emitter->append_term(emitter, expression, &term, source_position);
}

_6X_METHODXXX(void, emit_integer, struct expression_t *expression, int value, int source_position)
{
	struct expression_emitter_t *emitter = &_This->machine->expression_emitter;

	struct expression_term_t term;

	term.type = EVALUATOR_OPCODE_PUSH_INTEGER;
	term.data.as_integer = value;
	term.data.type = TYPE_INT;

	emitter->append_term(emitter, expression, &term, source_position);
}

_6X_METHODXXXX(void, emit_call, struct expression_t *expression, struct callable_unit_t *function, int number_of_args, int source_position)
{
	struct expression_emitter_t *emitter = &_This->machine->expression_emitter;

	struct expression_term_t term;

	if (!function->is_resolved)
	{
		term.type = EVALUATOR_OPCODE_UNRESOLVED_EXPERNAL_CALL;
	}
	else if (function->entry_point_ptr)
	{
		term.type = EVALUATOR_OPCODE_BUILTIN;
	}
	else if (function->is_lua)
	{
		term.type = EVALUATOR_OPCODE_LUACALL;
	}
	else if (function->exit_point_program_counter)
	{
		term.type = EVALUATOR_OPCODE_UNRESOLVED_EXPERNAL_CALL; // hack crz: this might be a bit hacky
	}
	else
	{
		term.type = EVALUATOR_OPCODE_UNRESOLVED_EXPERNAL_CALL;
	}

	term.data.as_integer = function->idx;
	term.data.type = function->return_type;
	term.number_of_args = number_of_args;

	emitter->append_term(emitter, expression, &term, source_position);
}

_6X_METHODXXX(void, emit_static_register_allocation, struct expression_t *expression, int idx, int source_position)
{
	struct expression_emitter_t *emitter = &_This->machine->expression_emitter;

	struct expression_term_t term;

	term.data.as_integer = idx;
	term.type = EVALUATOR_OPCODE_ALLOC_SREG;
	term.data.type = TYPE_INT;

	emitter->append_term(emitter, expression, &term, source_position);
}

_6X_METHODXXX(void, emit_frame_register_allocation, struct expression_t *expression, int type, int source_position)
{
	struct expression_emitter_t *emitter = &_This->machine->expression_emitter;

	struct expression_term_t term;

	term.data.as_integer = type;
	term.type = EVALUATOR_OPCODE_ALLOC_FREG;
	term.data.type = TYPE_INT;

	emitter->append_term(emitter, expression, &term, source_position);
}

