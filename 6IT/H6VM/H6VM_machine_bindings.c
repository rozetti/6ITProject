/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../H6VM.h"

// hack crz: this is all a bit inconsistent and hacky

_H6VM_METHODX(void, bind_compiler, struct compiler_t *compiler)
{
	compiler->machine = _This;
	_This->bind_scanner(_This, compiler->scanner);
}

_H6VM_METHODX(void, bind_scanner, struct scanner_t *scanner)
{
	scanner->machine = _This;
	scanner->environment = _This->environment;
};

_H6VM_METHODX(void, bind_processor, struct processor_t *processor)
{
#ifdef _6IT_DEBU66ER
	_This->execute = processor->execute_debug;
#else
	_This->execute = processor->execute;
#endif
};

_H6VM_METHODX(void, bind_evaluator, struct evaluator_t *evaluator)
{
	_This->evaluate = evaluator->evaluate;
};

_H6VM_METHODX(void, bind_downcompiler, struct downcompiler_t *downcompiler)
{
};

_H6VM_METHODX(void, bind_environment, struct environment_t *environment)
{
	_This->environment = environment;
}

#ifdef _6IT_SUPPORT_LUA
_H6VM_METHODX(void, bind_lua, struct lua_State *state)
{
	_This->lua = state;
}
#endif