/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <memory.h>
#include <stdlib.h>

static int construct_breakpoints(struct machine_debugger_support_t *debug)
{
	int size = sizeof(struct breakpoint_t) * debug->config.max_number_of_breakpoints;
	if (!(debug->breakpoints = (struct breakpoint_t *)malloc(size)))
	{
		return 0;
	}

	memset(debug->breakpoints, 0, size);

	return 1;
}

static int construct_symbols(struct machine_debugger_support_t *debug)
{
	int size = sizeof(struct symbol_t) * debug->config.max_number_of_symbols;
	if (!(debug->symbols = (struct symbol_t *)malloc(size)))
	{
		return 0;
	}

	memset(debug->symbols, 0, size);

	debug->free_symbol_index = 0;

	return 1;
}

static void destruct_machine_debug(struct machine_debugger_support_t *debug)
{
	if (debug->symbols)
	{
		free(debug->symbols);
		debug->symbols = 0;
	}

	if (debug->breakpoints)
	{
		free(debug->breakpoints);
		debug->breakpoints = 0;
	}
}

int construct_machine_debugger_support(struct machine_debugger_support_t *debug, struct machine_debug_config_t config)
{
	debug->config = config;

	if (construct_breakpoints(debug) &&
		construct_symbols(debug))
	{
		debug->count_statements_interpreted = 0;
		debug->count_instructions_executed = 0;

		debug->token_trace_on = 0;
		debug->eval_trace_on = 0;
		debug->opcode_trace_on = 0;
		debug->call_trace_on = 0;
		debug->line_trace_on = 0;

		return 1;
	}

	destruct_machine_debug(debug);

	return 0;
}

// todo crz: macro for this
int machine_debug_is_active(struct machine_t *machine)
{
	return machine->debug != 0;
}