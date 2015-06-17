/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "SubC_tester.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <malloc.h>

static int source_seek(void *context, int offset)
{
#ifdef _6IT_ANDROID
	AAsset *asset = (AAsset *)context;
	AAsset_seek(asset, offset, SEEK_SET);
#else
	FILE *p = (FILE *)context;

	return fseek(p, offset, SEEK_SET);
#endif
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
	fstat(_fileno(fp), &st);
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

int run_file(struct machine_t *machine, int(*compile)(), int(*run)(), char const *filename)
{
	machine->printf(machine, "\nrunning file '%s' ------------------------------------------------------------\n\n", filename);
#ifdef _6IT_ANDROID
	struct android_app *app = (struct android_app*)_Bios.context;
	AAssetManager* mgr = app->activity->assetManager;
	AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
	if (NULL == asset) {
		machine->printf(machine, "could not open program\n");
		return 0;
	}
	machine->environment->context = asset;
#else
	FILE *fp = 0;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		machine->printf(machine, "could not open program\n");
		return 0;
	}
	machine->environment->context = fp;
#endif

	int compiled = (*compile)();
#ifdef _6IT_ANDROID
	AAsset_close(asset);
#else
	fclose(fp);
#endif

	if (compiled)
	{
#ifdef _6IT_SUPPORT_LUA
		if (machine->environment->lua)
		{
			machine->bind_lua(machine, machine->environment->lua);
		}
#endif
		(*run)();
		assert(machine->check_state(machine));
	}

	return 1;
}

static void run_tests_callback(struct machine_t *machine, int(*compile)(), int(*run)())
{
	run_file(machine, compile, run, TEST_FILE_ASSET("test_main.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_function_call.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_if.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_loops.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_operators.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_globals.c"));
	run_file(machine, compile, run, TEST_FILE_ASSET("test_float.c"));

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

