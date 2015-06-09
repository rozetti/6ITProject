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

#include "6IT.h"

#define _6X_METHOD_NAME(NAME) (_6X##_##NAME)
#define _6X_METHOD(R, NAME) _6IT_PRIVATE R _6IT_THISCALL(expression_parser, _6X_METHOD_NAME(NAME))
#define _6X_METHODX(R, NAME, P1) _6IT_PRIVATE R _6IT_THISCALLX(expression_parser, _6X_METHOD_NAME(NAME), P1)
#define _6X_METHODXX(R, NAME, P1, P2) _6IT_PRIVATE R _6IT_THISCALLXX(expression_parser, _6X_METHOD_NAME(NAME), P1, P2)
#define _6X_METHODXXX(R, NAME, P1, P2, P3) _6IT_PRIVATE R _6IT_THISCALLXXX(expression_parser, _6X_METHOD_NAME(NAME), P1, P2, P3)
#define _6X_METHODXXXX(R, NAME, P1, P2, P3, P4) _6IT_PRIVATE R _6IT_THISCALLXXXX(expression_parser, _6X_METHOD_NAME(NAME), P1, P2, P3, P4)

enum {
	ERROR_6X_NONE = EXPRESSION_PARSER_ERRORS,
	ERROR_6X_CLOSING_PARENTHESIS_EXPECTED,
	ERROR_6X_UNDEFINED_IDENTIFIER
};
