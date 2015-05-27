#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>

#include "6IT.h"
#include "SubCRT.h"

static char buffer[100];

static struct machine_t machine;
static struct compiler_t compiler;
static struct environment_t env;

static struct lua_State *lua;

//#define BREAK_AT_START
//#define BREAK_AT_END
//#define DUMP_COMPILED_CODE // todo crz: finc out why corrupts the registers in the lua test (with threading on)

#ifndef _6IT_DEBU66ER
#undef BREAK_AT_START
#undef BREAK_AT_END
#endif

static int stream_seek(void *context, int offset)
{
	FILE *p = (FILE *)context;

	return fseek(p, offset, SEEK_SET);
}

static int stream_get_char(void *context, char *c)
{
	FILE *p = (FILE *)context;

	*c = getc(p);

	if (feof(p))
	{
		*c = 0;
		return 0;
	}

	return 1;
}

static void teardown()
{
	machine.destruct(&machine);
	compiler.destruct(&compiler);
#ifdef _6IT_SUPPORT_LUA
	lua_close(lua);
#endif	
}

#ifdef _6IT_SUPPORT_LUA
static int setup_lua()
{
	lua = 0;

#ifdef _6IT_SUPPORT_LUA
	struct lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, "test_fibonacci_lua.lua");

	lua = L;

	return 1;
#endif

	return 0;
}
#endif

static int setup()
{
	struct machine_config_t config;
	construct_default_H6VM_config(&config);
	construct_H6VM(&machine, &machine, config);

	env = construct_environment();
	env.output_string = _Bios.print_string;
	env.input_line = _Bios.read_line;
	env.read_next_char = stream_get_char;
	env.read_seek = stream_seek;

	construct_6IL(&machine.processor, &machine);
	construct_6EV(&machine.evaluator, &machine);
	construct_6YM(&machine.downcompiler, &machine);
	construct_SubC(&compiler, &machine);

	compiler.verbosity = 0;
	compiler.scanner->verbosity = 0;
	machine.downcompiler.verbosity = 0;
	machine.evaluator.verbosity = 0;
	machine.verbosity = 0;

	machine.bind_environment(&machine, &env);
	machine.bind_compiler(&machine, &compiler);
	machine.bind_processor(&machine, &machine.processor);
	machine.bind_evaluator(&machine, &machine.evaluator);
	machine.bind_downcompiler(&machine, &machine.downcompiler);
#ifdef _6IT_SUPPORT_LUA
	machine.bind_lua(&machine, lua);
#endif

	return 1;
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

static int compile()
{
	if (CATCH(&compiler.scanner->exception))
	{
		puts("scanner fault");
		return 0;
	}

	machine.new_program(&machine);
	bind_SubCRT(&machine);
	int parms[] = { TYPE_INT, 0 };
	machine.add_builtin(&machine, TYPE_INT, "c_fib", c_fib, parms);

	if (!compiler.compile(&compiler))
	{
		puts("could not compile source");
		return 0;
	}

	machine.downcompiler.downcompile(&machine.downcompiler);

#ifdef DUMP_COMPILED_CODE
	machine.debugger.print_state(&machine.debugger);
#endif

	return 1;
}

static int run()
{
	if (CATCH(&machine.exception))
	{
		machine.printf(&machine, "virtual machine fault occured during compilation\n");
		return 0;
	}

	_FRAME(&machine)->evaluator_program_counter = -1;
#ifdef _6IT_SUPPORT_INTERRUPTS
	_CLEAR_IRQF(&machine);
#endif

	struct callable_unit_t *si = machine.find_callable_unit(&machine, "static");
	_SET_PC(_REGS(&machine), si->entry_point_program_counter);
	machine.execute(&machine);

	struct callable_unit_t *cu = machine.find_callable_unit(&machine, "main");
	if (!cu)
	{
		machine.printf(&machine, "main() entry point not found\n");
		return 0;
	}

#ifdef _6IT_SUPPORT_THREADS
	struct thread_t worker1;
	struct thread_t worker2;
	machine.number_of_threads = 1; // hack crz

	struct callable_unit_t *th1 = machine.find_callable_unit(&machine, "worker1");
	if (th1)
	{
		machine.create_thread(&machine, &worker1, th1);
		machine.printf(&machine, "intitialised worker thread 1\n");
	}

	struct callable_unit_t *th2 = machine.find_callable_unit(&machine, "worker2");
	if (th2)
	{
		machine.create_thread(&machine, &worker2, th2);
		machine.printf(&machine, "intitialised worker thread 2\n");
	}

	machine.switch_context(&machine, machine.primary_thread.id);
#endif

	_SET_PC(_REGS(&machine), cu->entry_point_program_counter);
	_FRAME(&machine)->entry_ip = _REG_CS(_REGS(&machine)) + cu->entry_point_program_counter;
	_FRAME(&machine)->callable_unit_idx = cu->idx;

	_Bios.get_current_time_ms(&machine.execution_start_ms);
	machine.pause_ms = 0;

	int end_ms;
	if (CATCH(&machine.exception))
	{
		_Bios.get_current_time_ms(&end_ms);
		int d = end_ms - machine.execution_start_ms - machine.pause_ms;
		machine.printf(&machine, "virtual machine fault occured after %d seconds, %d milliseconds\n", d / 1000, d % 1000);
#ifdef BREAK_AT_END
		machine.debugger.enter_processor_debugger(&machine.debugger, 0);
#endif
		return 0;
	}

#ifdef BREAK_AT_START
	machine.interrupt_controller.request(&machine.interrupt_controller, INTERRUPT_BREAK);
#endif

	machine.execute(&machine);
	_Bios.get_current_time_ms(&end_ms);

#ifdef _6IT_SUPPORT_THREADS
	machine.switch_context(&machine, machine.primary_thread.id);
#endif
	int rv = POP_INT(&machine);

	int d = end_ms - machine.execution_start_ms - machine.pause_ms;
	machine.printf(&machine, "main() returned %d after %d seconds, %d milliseconds\n", rv, d / 1000, d % 1000);

#ifdef BREAK_AT_END
	machine.debugger.enter_processor_debugger(&machine.debugger, 0);
#endif

	return 1;
}

int run_file(char const *filename)
{
	machine.printf(&machine, "\nrunning file '%s' ------------------------------------------------------------\n\n", filename);

	FILE *fp = 0;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		machine.printf(&machine, "could not open program\n");
		return 0;
	}

	machine.environment->context = fp;

	int compiled = compile();

	fclose(fp);

	if (compiled)
	{
#ifdef _6IT_SUPPORT_LUA
		machine.bind_lua(&machine, lua);
#endif
		run();
		assert(machine.check_state(&machine));
	}

	return 1;
}

int run_tests()
{
	run_file("test_main.c");
	run_file("test_function_call.c");
	run_file("test_if.c");
	run_file("test_loops.c");
	
	run_file("test_operators.c");
	
	run_file("test_globals.c");
	run_file("test_float.c");

	run_file("test_fibonacci_subc.c");
#ifdef _6IT_SUPPORT_LUA
	run_file("test_fibonacci_lua.c");
#endif
	run_file("test_fibonacci_native.c");

#ifdef _6IT_SUPPORT_THREADS
	run_file("test_threads.c");
#endif

	return 1;
}

DWORD WINAPI run_tests_thread(LPVOID blah)
{
	run_tests();

	return 42;
}

int main(int argc, char *argv[])
{
#ifdef _6IT_SUPPORT_LUA
	setup_lua();
#endif

	if (!setup())
	{
		puts("could not set up test environment\n");
		return 2;
	}

#ifdef _6IT_SUPPORT_THREADS
	DWORD id;
	HANDLE th = CreateThread(NULL, 0, run_tests_thread, NULL, 0, &id);

	if (th)
	{
		while (WaitForSingleObject(th, 5) == WAIT_TIMEOUT)
		{
			machine.interrupt_controller.request(&machine.interrupt_controller, INTERRUPT_YIELD);
		}

		CloseHandle(th);
	}
#else
		run_tests();
#endif

	teardown();

	return 0;
}

