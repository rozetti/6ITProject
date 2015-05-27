/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

//#include "6IT.h"
#include "6OO_reader.h"

// todo crz: proper way to handle this stuff

#include <stdio.h>
#include <stdlib.h>

// hack crz
#define PROG_SIZE 10000

// todo crz: abstract this
char *scanner_load_program(struct scanner_t *scanner)
{
	char *buffer;
	if ((buffer = (char *)malloc(PROG_SIZE)) == 0)
	{
		puts("allocation failure");
		return 0;
	}

	int i = 0;
	char *p = buffer;

	while (scanner->environment->read_next_char(scanner->environment->context, p) && i<PROG_SIZE)
	{
		p++; i++;
	};

	return buffer;
}
