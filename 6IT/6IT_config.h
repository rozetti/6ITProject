/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#ifdef _6IT_CONFIGURED
#error "6IT is already configured"
#endif

#define _6IT_AMALGAMATE_SOURCE

//#define PROGRAM_COUNTER_INDEX_BASED
#define REGISTER_STACK_INDEX_BASED
#define _6IT_STACK_DISPATCH
#define _6IT_SUPPORT_LUA
//#define _6IT_SUPPORT_INTERRUPTS
//#define _6IT_SUPPORT_THREADS
//#define _6IT_DEBU66ER
//#define _6IT_CHECKED_BUILD

#define _TIMESLICE 10000
#define _NUMBER_OF_INTERRUPT_LEVELS 10



#define _6IT_CONFIGURED