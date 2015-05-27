/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

// todo crz: macros
// hack crz: this is all a bit inconsistent and hacky

static void machine_bind_compiler(struct machine_t *machine, struct compiler_t *compiler)
{
	compiler->machine = machine;
	machine->bind_scanner(machine, compiler->scanner);
}

static void machine_bind_scanner(struct machine_t *machine, struct scanner_t *scanner)
{
	scanner->machine = machine;
	scanner->environment = machine->environment;
};

static void machine_bind_processor(struct machine_t *machine, struct processor_t *processor)
{
#ifdef _6IT_DEBU66ER
	machine->execute = processor->execute_debug;
#else
	machine->execute = processor->execute;
#endif
};

static void machine_bind_evaluator(struct machine_t *machine, struct evaluator_t *evaluator)
{
	machine->evaluate = evaluator->evaluate;
};

static void machine_bind_downcompiler(struct machine_t *machine, struct downcompiler_t *downcompiler)
{
};

static void machine_bind_environment(struct machine_t *machine, struct environment_t *environment)
{
	machine->environment = environment;
}

#ifdef _6IT_SUPPORT_LUA
static void machine_bind_lua(struct machine_t *machine, struct lua_State *state)
{
	machine->lua = state;
}
#endif