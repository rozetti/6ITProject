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

#include "6EV/6EV_opcodes.h"

enum
{
	ERROR_6EV_ERRORS = EVALUATOR_ERRORS,
	ERROR_6EV_REGISTER_ASSIGNMENT_FROM_WRONG_TYPE,
	ERROR_6EV_INT_UNARY_OPERATOR_OPERAND_WRONG_TYPE,
	ERROR_6EV_FP32_UNARY_OPERATOR_OPERAND_WRONG_TYPE,
	ERROR_6EV_CASTINT_OPERAND_MUST_BE_NON_INT,
	ERROR_6EV_CASTFP32_OPERAND_MUST_BE_NON_FP32,
	ERROR_6EV_INVALID_CAST_TO_INT,
	ERROR_6EV_INVALID_CAST_TO_FP32,
};
