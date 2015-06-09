/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6EV.h"

#ifdef _6IT_AMALGAMATE_SOURCE
#include "6EV/6EV_debug.c"
#include "6EV/6EV_opcodes.c"
#include "6EV/6EV_misc.c"
#include "6EV/6EV_type_checker.c"

#ifdef _6IT_DEBU66ER
#define EVALUATOR_WITH_DEBUGGER
#include "6EV/6EV_evaluator.c"
#undef EVALUATOR_WITH_DEBUGGER
#endif

#include "6EV/6EV_evaluator.c"
#endif

_6IT_DESTRUCTOR(evaluator, 6EV) { }

_6IT_THISCALL_DIE(evaluator, 6EV)
{
	THROW(&_This->exception, fault_code);
}

_6IT_THISCALL_GET_ERROR_STRING(evaluator, 6EV)
{
	switch (error)
	{
	case ERROR_6EV_REGISTER_ASSIGNMENT_FROM_WRONG_TYPE:
		return "Register assignment from wrong type";
	case ERROR_6EV_INT_UNARY_OPERATOR_OPERAND_WRONG_TYPE:
		return "int32 unary operator operand wrong type";
	case ERROR_6EV_FP32_UNARY_OPERATOR_OPERAND_WRONG_TYPE:
		return "fp32 unary operator operand wrong type";
	case ERROR_6EV_CASTINT_OPERAND_MUST_BE_NON_INT:
		return "CASTINT operand must be non-int";
	case ERROR_6EV_CASTFP32_OPERAND_MUST_BE_NON_FP32:
		return "CASTFP32 operand must be non-fp32";
	case ERROR_6EV_INVALID_CAST_TO_INT:
		return "Invalid cast to int";
	case ERROR_6EV_INVALID_CAST_TO_FP32:
		return "Invalid cast to fp32";
	}

	return 0;
}

_6IT_PUBLIC _6IT_CONSTRUCTOR(evaluator, 6EV)
{
	INITIALISE_COMMON_ATTRIBUTES(evaluator, 6EV, _Machine);

	_This->evaluate = _6EV_METHOD_NAME(evaluate);
#ifdef _6IT_DEBU66ER
	_This->evaluate_debug = _6EV_METHOD_NAME(evaluate_debug);
#else
	_This->evaluate_debug = 0; // todo crz: catch this
#endif
	_This->check_expression = _6EV_METHOD_NAME(check_expression);
	_This->print_expression = _6EV_METHOD_NAME(print_expression);
	_This->print_state = _6EV_METHOD_NAME(print_state);
	_This->get_mnemonic = _6EV_METHOD_NAME(get_mnemonic);
}