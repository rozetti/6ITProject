/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include <setjmp.h>

#define EXCEPTION_MESSAGE_LENGTH 100

struct exception_t
{
	int fault_code;
	char message[100];
	jmp_buf restore_state;
	struct machine_t *machine;
};

struct environment_t
{
	int(*read_next_char)(void *context, char *);
	int(*read_seek)(void *context, int offset);

	void(*input_line)(char *buffer, int max_length);
	void(*output_string)(char const *);

	void *context;
};

#ifdef _6IT_SUPPORT_LUA
#include "..\..\ThirdParty\lua\lua.h"
#include "..\..\ThirdParty\lua\lualib.h"
#include "..\..\ThirdParty\lua\lauxlib.h"
#endif

