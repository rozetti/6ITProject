/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

struct machine_t;

struct scheduler_t
{
	struct machine_t *machine;

	void(*next)(struct scheduler_t*);
};