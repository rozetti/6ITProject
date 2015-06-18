/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include "6IT.h"

typedef void(*run_callback_t)(struct machine_t *);
typedef void(*compile_callback_t)(struct machine_t *);
typedef void(*run_tests_callback_t)(struct machine_t *, int(*compile)(compile_callback_t), int(*run)(run_callback_t, run_callback_t));

int SubC_tester_main(void *context);
int SubC_tester_run_tests(struct environment_t *, run_tests_callback_t);
int SubC_tester_load_file(struct machine_t *machine, char const *filename, char **buffer);

#ifdef _6IT_ANDROID
#define TEST_FILE_ASSET(filename) filename
#else
#define TEST_FILE_ASSET(filename) "../TestFiles/" filename
#endif
