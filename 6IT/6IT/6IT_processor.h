/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#define OPCODE_INSTRUCTION(o) (o & 0xffff)
#define OPCODE_IMMEDIATE(o) ((o & 0xffff0000) >> 16)

#define OPERAND_16H(o) ((o & 0xffff0000) >> 16)
#define OPERAND_16L(o) (o & 0xffff)

#define OPCODE(M) (short)(_REG_IP(_REGS(M))[0] & 0xffff)
#define OPCODE_PARM(M) ((_REG_IP(_REGS(M))[0] & 0xffff0000) >> 16)
#define PROCESSOR_OPCODE_PARM(ip) (((*(ip)) & 0xffff0000) >> 16)

#define SET_OPCODE(a, c) (a = (a & 0xffff0000) | c)
#define SET_OPCODE_PARM(a, p) (a = ((a & 0xffff) | (p << 16))) 
#define MAKE_OPCODE(c, p) (c & 0xffff | (p << 16))

#define OPERAND1(M) _REG_IP(_REGS(M))[1]
#define OPERAND1_16H(M) ((_REG_IP(_REGS(M))[1] & 0xffff0000) >> 16)
#define OPERAND1_16L(M) (_REG_IP(_REGS(M))[1] & 0xffff)

#define OPERAND2(M) _REG_IP(_REGS(M))[2]
#define OPERAND2_16H(M) ((_REG_IP(_REGS(M))[2] & 0xffff0000) >> 16)
#define OPERAND2_16L(M) (_REG_IP(_REGS(M))[2] & 0xffff)

#define OPERAND3(M) _REG_IP(_REGS(M))[3]

#define OPERAND_16H(o) ((o & 0xffff0000) >> 16)

#define MAKE_OPERAND(h, l) ((l & 0xffff) | ((h & 0xffff) << 16))

enum intrinsics_opcodes
{
	MACHINE_OPCODE_BREAK = -2,
	MACHINE_OPCODE_STOP = -1,
	MACHINE_OPCODE_INVALID = 0,
};

