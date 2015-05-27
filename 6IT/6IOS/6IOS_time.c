/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

#include <time.h>

_6IT_PRIVATE void get_current_time_ms(int *milliseconds)
{
	clock_t time = clock();

	*milliseconds = time * 1000 / CLOCKS_PER_SEC;
}
