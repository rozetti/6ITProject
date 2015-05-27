/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#ifdef _6IT_SUPPORT_INTERRUPTS

#define _IRQF(M) ((M)->interrupt_flag)
#define _SET_IRQF(M) ((M)->interrupt_flag = 1)
#define _CLEAR_IRQF(M) ((M)->interrupt_flag = 0)

// todo crz: add '_LEVEL_' or IRQL
enum {
	INTERRUPT_NONE = 0,
	INTERRUPT_SINGLE_STEP,
	INTERRUPT_BREAK,
	INTERRUPT_YIELD
};

struct machine_t;

struct interrupt_controller_t
{
	struct machine_t *machine;

	void(*request)(struct interrupt_controller_t*, int level);
};

#endif