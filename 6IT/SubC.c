/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "SubC.h"

_SUBC_METHOD_CONST(void, compile_program);
_SUBC_METHOD_CONST(void, compile_single_statement);
_SUBC_METHOD_CONST(void, compile_statements);
_SUBC_METHOD_CONST(void, compile_block);
_SUBC_METHOD_CONST(void, compile_single_statement_scope);

#ifdef _6IT_AMALGAMATE_SOURCE
#include "SubC/SubC_compiler_whitespace.c"

#include "SubC/SubC_variables.c"

#include "SubC/SubC_compile_expression.c"
#include "SubC/SubC_compile_for.c"
#include "SubC/SubC_compile_function.c"
#include "SubC/SubC_compile_if.c"
#include "SubC/SubC_compile_loops.c"
#include "SubC/SubC_compile_assert.c"
#include "SubC/SubC_compiler.c"
#include "SubC/SubC_compile_program.c"
#endif

#include <memory.h>

void override_move_past_whitespace(struct scanner_t *scanner);

_6IT_DESTRUCTOR(compiler, SubC)
{
	// todo crz: might need to be careful not to destruct the base object
	_This->scanner->destruct(_This->scanner);
}

_6IT_CONSTRUCTOR(compiler, SubC)
{
	construct_6OO(&BASE_OBJECT(_This->scanner), _Machine);
	INITIALISE_POLYMORPHIC_ATTRIBUTE(_This->scanner);

	DERIVED_OBJECT(_This->scanner).move_past_whitespace = override_move_past_whitespace;

	_This->destruct = _SUBC_METHOD_NAME(destruct);
	_This->compile = _SUBC_METHOD_NAME(compile);
	_This->scanner = &DERIVED_OBJECT(_This->scanner);
}

