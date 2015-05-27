/*****************************************************************************
The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org.
******************************************************************************
The MIT License (MIT)

Copyright(c) 2015 Conrad Rozetti

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

#pragma once

#include "6IT/6IT_platform.h"

//#define PROGRAM_COUNTER_INDEX_BASED
#define REGISTER_STACK_INDEX_BASED
#define _6IT_STACK_DISPATCH
#define _6IT_SUPPORT_LUA
//#define _6IT_SUPPORT_INTERRUPTS
#define _6IT_SUPPORT_THREADS
#define _6IT_DEBU66ER
//#define _6IT_CHECKED_BUILD

#define _TIMESLICE 10000
#define _NUMBER_OF_INTERRUPT_LEVELS 10

#ifdef _6IT_SUPPORT_THREADS

#ifdef _6IT_STACK_DISPATCH
#undef _6IT_STACK_DISPATCH
#endif

#ifndef _6IT_SUPPORT_INTERRUPTS
#define _6IT_SUPPORT_INTERRUPTS
#endif

#endif

#ifdef _6IT_DEBU66ER
#ifndef _6IT_SUPPORT_INTERRUPTS
#define _6IT_SUPPORT_INTERRUPTS
#endif
#endif

typedef volatile unsigned char atomic_flag_t; // todo crz: platform-specific betterness

// crz: these must stay at the top
#include "6IT/6IT_macros.h"
#include "6IT/6IT_classes.h"
#include "6IT/6IT_outside_world.h" // crz: this should have no dependency on 6IT

#include "6IT/6IT_bios.h"
#include "6IT/6IT_types.h"
#include "6IT/6IT_tokens.h"
#include "6IT/6IT_interfaces.h"
#include "6IT/6IT_constructors.h"

#include "6IT/6IT_end_user.h"

#include <assert.h>

#ifdef _6IT_PRIVATE
#undef _6IT_PRIVATE
#endif
#define _6IT_PRIVATE static
