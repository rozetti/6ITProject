/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

struct bios_t _Bios; 

#ifdef _6IT_AMALGAMATE_SOURCE
#include "H6VM/H6VM_execution_stack.c"
#include "H6VM/H6VM_evaluator_stack.c"
#include "H6VM/H6VM_instruction_emitter.c"
#include "H6VM/H6VM_expression_emitter.c"
#include "H6VM/H6VM_thread.c"
#include "H6VM/H6VM_scheduler.c"
#include "H6VM/H6VM_debug.c"

#include "H6VM/H6VM_machine.c"
#endif
