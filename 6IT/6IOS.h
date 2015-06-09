/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include "6IT.h"

// crz: this stuff is not actually needed as the bios functions are system-wide
#define _6IOS_METHOD_NAME(NAME) (_6IOS##_##NAME)
#define _6IOS_METHOD(R, NAME) PRIVATE_METHOD(debugger, R, _6IOS_METHOD_NAME(NAME)) 
#define _6IOS_METHODX(R, NAME, P1) PRIVATE_METHODX(debugger, R, _6IOS_METHOD_NAME(NAME), P1)
#define _6IOS_METHODXX(R, NAME, P1, P2) PRIVATE_METHODXX(debugger, R, _6IOS_METHOD_NAME(NAME), P1, P2)
