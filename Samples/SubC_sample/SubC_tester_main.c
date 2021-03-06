/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#ifdef __cplusplus
extern "C"
{
#endif
#include "6IT.h"
#include "SubC_tester.h"
#include "subc_tester_bios.h"
int SubC_tester_main(void *context);

#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>

#ifdef _6IT_QT_
#include <QFile>
#endif

#define BINDING_TARGET_REGISTER_NAME "binding_target"

#define RUN_LONG_TESTS


static int c_fib_inner(int n)
{
	if (0 == n) return 0;
	if (1 == n) return 1;

	return c_fib_inner(n - 1) + c_fib_inner(n - 2);
}

static int c_fib(struct machine_t *machine)
{
	return c_fib_inner(TOP_INT(machine));
}

void prepare_compile_native_fib(struct machine_t *machine)
{
	machine->add_builtin(machine, TYPE_INT, "c_fib", c_fib, (int[]) { TYPE_INT, 0 });
}

int run_file(struct machine_t *machine, int(*compile)(compile_callback_t), int(*run)(run_callback_t, run_callback_t), char const *filename)
{
	machine->printf(machine, "\nrunning file '%s' ------------------------------------------------------------\n\n", filename);

	if (!open_file(machine, filename))
	{
		machine->printf(machine, "could not open program\n");
		return 0;
	}

	int compiled = (*compile)(prepare_compile_native_fib);
	close_file(machine);

	if (compiled)
	{
		(*run)(0, 0);
		assert(machine->check_state(machine));
	}

	return 1;
}

void test_binding_pre_callback(struct machine_t *machine)
{
	machine->printf(machine, "setting static register '%s' to integer value %d\n", BINDING_TARGET_REGISTER_NAME, 42);
	SET_STATIC_REGISTER_INT(machine, BINDING_TARGET_REGISTER_NAME, 42);
}

void test_binding_post_callback(struct machine_t *machine)
{
	int rv;
	float f;
	int v;

	v = GET_STATIC_REGISTER_INT(machine, BINDING_TARGET_REGISTER_NAME);
	machine->printf(machine, "static register '%s' has integer value %d\n", BINDING_TARGET_REGISTER_NAME, v);
	assert(v == 1729);

	struct callable_unit_t const *cu = FIND_CALLABLE_UNIT(machine, "increment_test");

    for (int n = 0; n < 10; ++n)
	{
		rv = machine->vcall(machine, cu, 0);
		assert(rv == 1);
	}
	v = GET_STATIC_REGISTER_INT(machine, BINDING_TARGET_REGISTER_NAME);
	assert(v == (1729 + 10));

	rv = machine->vcall(machine, FIND_CALLABLE_UNIT(machine, "add_test"), 0, 13);
	assert(rv == 1);
	v = GET_STATIC_REGISTER_INT(machine, BINDING_TARGET_REGISTER_NAME);
	assert(v == 1729 + 10 + 13);
	machine->printf(machine, "sum is %d\n", v);

	rv = machine->vcall(machine, FIND_CALLABLE_UNIT(machine, "return_int_test"), &v);
	assert(v == 314);
	machine->printf(machine, "return value was %d\n", v);

	rv = machine->vcall(machine, FIND_CALLABLE_UNIT(machine, "sum_test"), &v, 42, 1729);
	assert(rv == 1);
	assert(v == 42 + 1729);
	machine->printf(machine, "return value was %d\n", v);

	rv = machine->vcall(machine, FIND_CALLABLE_UNIT(machine, "sum_test_float"), &f, 3.14f, 2.78f);
	assert(rv == 1);
	assert(fabs(f) - (3.14f + 2.78f) <= FLT_EPSILON);
	machine->printf(machine, "return value was %f\n", f);

	rv = machine->vcall(machine, FIND_CALLABLE_UNIT(machine, "test_printf"), 0, "hello SubC!", 1729, 3.14);
	assert(rv == 1);
}

static int test_binding(struct machine_t *machine, int(*compile)(compile_callback_t), int(*run)(run_callback_t, run_callback_t), char const *filename)
{
	machine->printf(machine, "testing binding on file '%s' ------------------------------------------------------------\n\n", filename);

	if (!open_file(machine, filename))
	{
		machine->printf(machine, "could not open program\n");
		return 0;
	}

	int compiled = (*compile)(0);
	close_file(machine);

	if (compiled)
	{
		(*run)(test_binding_pre_callback, test_binding_post_callback);
		assert(machine->check_state(machine));
	}

	return 1;
}

static void run_tests_callback(struct machine_t *machine, int(*compile)(compile_callback_t), int(*run)(run_callback_t, run_callback_t))
{
	run_file(machine, compile, run, TEST_FILE_ASSET("test_main.subc"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_function_call.subc"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_if.subc"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_loops.subc"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_operators.subc"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_globals.subc"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_float.subc"));

	test_binding(machine, compile, run, TEST_FILE_ASSET("test_binding.subc"));

#ifdef RUN_LONG_TESTS
    run_file(machine, compile, run, TEST_FILE_ASSET("test_fibonacci_subc.subc"));
    run_file(machine, compile, run, TEST_FILE_ASSET("test_fibonacci_native.subc"));

#ifdef _6IT_SUPPORT_LUA
	machine->environment->lua = luaL_newstate();
	machine->bind_lua(machine, machine->environment->lua);

	luaL_openlibs(machine->environment->lua);

	char *source = 0;
	if (load_file(machine, TEST_FILE_ASSET("test_fibonacci_lua.lua"), &source))
	{
		luaL_dostring(machine->environment->lua, source);
		free(source);
	}

	run_file(machine, compile, run, TEST_FILE_ASSET("test_fibonacci_lua.c"));
	lua_close(machine->environment->lua);
	machine->environment->lua = 0;
#endif
#endif

#ifdef _6IT_SUPPORT_THREADS
	run_file(machine, compile, run, TEST_FILE_ASSET("test_threads.c"));
#endif
}

int SubC_tester_main(void *context)
{
	construct_6IOS(&_Bios, 0);

	_Bios.context = context;

	struct environment_t env = construct_environment();
	env.output_string = _Bios.print_string;
	env.input_line = _Bios.read_line;
	env.read_next_char = source_get_char;
	env.read_seek = source_seek;

	auto rv = SubC_tester_run_tests(&env, run_tests_callback);

	return rv;
}

