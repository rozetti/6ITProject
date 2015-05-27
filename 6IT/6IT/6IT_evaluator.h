/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#define EXPRESSION_TERMINATOR -1

extern struct value_t _int_value;

#define PUSH_INT(M, v) _INC_VIP(_REGS(M)); _REG_VIP(_REGS(M))->as_integer = v; _REG_VIP(_REGS(M))->type = TYPE_INT;
//#define PUSH_INT(M, i) _int_value.as_integer = (i); PUSH_VALUE(M, _int_value);

#define PUSH_FLOAT(M, v) _INC_VIP(_REGS(M)); _REG_VIP(_REGS(M))->as_float= v; _REG_VIP(_REGS(M))->type = TYPE_FLOAT;
#define PUSH_VALUE(M, v) _INC_VIP(_REGS(M)); *_REG_VIP(_REGS(M)) = v; 
//#define PUSH_VALUE(M, v) PUSH_TYPED_VALUE(M, v.as_integer, v.type)
#define PUSH_TYPED_VALUE(M, v, t) _INC_VIP(_REGS(M)); _REG_VIP(_REGS(M))->as_integer = v; _REG_VIP(_REGS(M))->type = t;

#define POP_VALUE(M) TOP_VALUE(M); _DEC_VIP(_REGS(M));
#define POP_INT(M) TOP_INT(M); _DEC_VIP(_REGS(M));
#define POP_FLOAT(M) TOP_FLOAT(M); _DEC_VIP(_REGS(M));

#define DROP(M) _DEC_VIP(_REGS(M));

#define TOP_INT(M) _REG_VIP(_REGS(M))->as_integer
#define TOP_FLOAT(M) _REG_VIP(_REGS(M))->as_float
#define TOP_VALUE(M) *_REG_VIP(_REGS(M))

#define SET_TOP_INT(M, i) _REG_VIP(_REGS(M))->as_integer = i; _REG_VIP(_REGS(M))->type = TYPE_INT;
#define SET_TOP_FLOAT(M, f) _REG_VIP(_REGS(M))->as_float = f; _REG_VIP(_REGS(M))->type = TYPE_FLOAT;

#define LEVEL2_INT(M) (_REG_VIP(_REGS(M)) - 1)->as_integer
#define LEVEL2_FLOAT(M) (_REG_VIP(_REGS(M)) - 1)->as_float
#define LEVEL2_VALUE(M) (_REG_VIP(_REGS(M)) - 1)

#define SET_LEVEL2_INT(M, i) (_REG_VIP(_REGS(M)) - 1)->as_integer = i; (_REG_VIP(_REGS(M)) - 1)->type = TYPE_INT;
#define SET_LEVEL2_FLOAT(M, f) (_REG_VIP(_REGS(M)) - 1)->as_float = f; (_REG_VIP(_REGS(M)) - 1)->type = TYPE_FLOAT;

enum stack_effects
{
	H6VM_STACK_UNKNOWN = -9999,
	H6VM_STACK_POP4 = -4,
	H6VM_STACK_POP3 = -3,
	H6VM_STACK_POP2 = -2,
	H6VM_STACK_POP1 = -1,
	H6VM_STACK_NONE = 0,
	H6VM_STACK_PUSH1 = 1,
	H6VM_STACK_PUSH2 = 2,
	H6VM_STACK_PUSH3 = 3,
	H6VM_STACK_PUSH4 = 4,
};
