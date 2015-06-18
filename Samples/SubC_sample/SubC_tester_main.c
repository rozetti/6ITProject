/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "SubC_tester.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>

#define BINDING_TARGET_REGISTER_NAME "binding_target"

#define RUN_LONG_TESTS

static int source_seek(void *context, int offset)
{
#ifdef _6IT_ANDROID
	AAsset *asset = (AAsset *)context;
	AAsset_seek(asset, offset, SEEK_SET);
#else
	FILE *p = (FILE *)context;

	return fseek(p, offset, SEEK_SET);
#endif

	return 1;
}

static int source_get_char(void *context, char *c)
{
#ifdef _6IT_ANDROID
	AAsset *asset = (AAsset *)context;
	if (0 == AAsset_read(asset, c, 1))
	{
		*c = 0;
		return 0;
	}
#else
	FILE *p = (FILE *)context;

	*c = getc(p);

	if (feof(p))
	{
		*c = 0;
		return 0;
	}
#endif

	return 1;
}

int load_file(struct machine_t *machine, char const *filename, char **buffer)
{
#ifdef _6IT_ANDROID
	struct android_app *app = (struct android_app*)_Bios.context;
	AAssetManager* mgr = app->activity->assetManager;
	AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
	if (NULL == asset) {
		machine->printf(machine, "could not open file\n");
		*buffer = 0;
		return 0;
	}

	long size = AAsset_getLength(asset);
	*buffer = (char *)malloc(sizeof(char)*size + 1);
	AAsset_read(asset, *buffer, size);
	(*buffer)[size] = 0;
	AAsset_close(asset);
#else
	FILE *fp = fopen(filename, "rb");
	if (!fp)
	{
		machine->printf(machine, "could not open file\n");
		*buffer = 0;
		return 0;
	}

	struct stat st;
	fstat(_6IT_Fileno(fp), &st);
	if (!(*buffer = (char *)malloc(st.st_size * sizeof(char) + 1)))
	{
		machine->printf(machine, "malloc failed\n");
		*buffer = 0;
		return 0;
	}

	size_t bytes = fread(*buffer, 1, st.st_size, fp);

	fclose(fp);

#endif

	return 1;
}

int open_file(struct machine_t *machine, char const *filename)
{
#ifdef _6IT_ANDROID
	struct android_app *app = (struct android_app*)_Bios.context;
	AAssetManager* mgr = app->activity->assetManager;
	AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
	if (NULL == asset) return 0;
	machine->environment->context = asset;
#else
	FILE *fp = 0;
	if ((fp = fopen(filename, "rb")) == NULL) return 0;
	machine->environment->context = fp;
#endif

	return 1;
}

void close_file(struct machine_t *machine)
{
#ifdef _6IT_ANDROID
	AAsset_close((AAsset *)machine->environment->context);
#else
	fclose((FILE *)machine->environment->context);
#endif
}

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
	run_file(machine, compile, run, TEST_FILE_ASSET("test_main.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_function_call.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_if.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_loops.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_operators.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_globals.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_float.c"));

	test_binding(machine, compile, run, TEST_FILE_ASSET("test_binding.c"));

#ifdef RUN_LONG_TESTS
	run_file(machine, compile, run, TEST_FILE_ASSET("test_fibonacci_subc.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_fibonacci_native.c"));

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
	_Bios.context = context;

	// todo crz: make this nicer
	construct_6IOS(&_Bios, 0);

	struct environment_t env = construct_environment();
	env.output_string = _Bios.print_string;
	env.input_line = _Bios.read_line;
	env.read_next_char = source_get_char;
	env.read_seek = source_seek;

	return SubC_tester_run_tests(&env, run_tests_callback);
}

