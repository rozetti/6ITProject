/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

enum processor_opcodes
{
	H6VM_OPCODE_BREAK = MACHINE_OPCODE_BREAK,
	H6VM_OPCODE_STOP = MACHINE_OPCODE_STOP,
	H6VM_OPCODE_INVALID = MACHINE_OPCODE_INVALID,

	// crz: skinny

	T6IL_MOVSI, // crz: move integer to special register
	T6IL_MOVSF, // crz: move fp to special register
	T6IL_MOVRI, // crz: move integer to relative register
	T6IL_MOVRF, // crz: move fp to relative register
	T6IL_MOVAI, // crz: move integer to absolute register
	T6IL_MOVAF, // crz: move fp to absolute register

	T6IL_MOVSR, // crz: move relative register to special register
	T6IL_MOVSA, // crz: move absolute register to special register
	T6IL_MOVSS, // crz: move special register to special register

	T6IL_MOVRS, // crz: move special register to relative register
	T6IL_MOVAS, // crz: move special register to absolute register

	T6IL_LOADI, // crz: move integer to accumulator
	T6IL_LOADF, // crz: move fp to accumulator
	T6IL_LOADR, // crz: move relative register to accumulator
	T6IL_LOADA, // crz: move absolute register to accumulator
	T6IL_LOADS, // crz: move special register to accumulator

	T6IL_INCR, // crz: increment relative register 
	T6IL_INCA, // crz: increment absolute register
	T6IL_INC, // crz: increment accumulator

	T6IL_DECR, // crz: increment relative register 
	T6IL_DECA, // crz: increment absolute register
	T6IL_DEC, // crz: increment accumulator

	T6IL_ADDR,
	T6IL_ADDA,

	T6IL_RETURNIIFREQI,
	T6IL_JUMPIFREQI,
	T6IL_JUMPIFRNEI,

	T6IL_JUMPIFEQI,
	T6IL_JUMPIFNEI,
	T6IL_JUMPIFEQ, // crz: jump to operand2 if comparand == accumulator
	T6IL_JUMPIFNE, // crz: jump to operand2 if comparand != accumulator
	T6IL_JUMPIFLT, // crz: jump to operand2 if comparand < accumulator
	T6IL_JUMPIFGT,
	T6IL_JUMPIFLE,
	T6IL_JUMPIFGE,

	T6IL_FORRI,
	T6IL_NEXTRI,
	T6IL_NEXTRINCI,
	T6IL_NEXTRDECI,

	T6IL_RETURNINT,
	// crz: code

	H6VM_OPCODE_JUMP, // crz: relative current PC
	H6VM_OPCODE_JUMPIF,
	H6VM_OPCODE_LJUMP, // crz: absolute PC
	H6VM_OPCODE_LJUMPIF,

	H6VM_OPCODE_FORS,
	H6VM_OPCODE_FORB,
	H6VM_OPCODE_NEXT,

	H6VM_OPCODE_WHILES,
	H6VM_OPCODE_WHILEB,

	H6VM_OPCODE_DOS,
	H6VM_OPCODE_DOB,

	H6VM_OPCODE_LOOP_BREAK,
	H6VM_OPCODE_LOOP_CONTINUE,

	H6VM_OPCODE_IFS,
	H6VM_OPCODE_IFB,
	H6VM_OPCODE_IFSS,
	H6VM_OPCODE_IFSB,
	H6VM_OPCODE_IFBS,
	H6VM_OPCODE_IFBB,
	H6VM_OPCODE_ELSE,

	H6VM_OPCODE_EVAL,
	H6VM_OPCODE_EXEC,
	H6VM_OPCODE_CONDITION, // crz: pops result and sets condition_flag
	H6VM_OPCODE_DROP,
	H6VM_OPCODE_ASSERT, // crz: die is condition_flag is false

	H6VM_OPCODE_RETURN_POP,
	H6VM_OPCODE_RETURN_FREG,
	H6VM_OPCODE_RETURN_SREG,
	H6VM_OPCODE_RETURN, // crz: void
	H6VM_OPCODE_RETURN_FLOAT,
	H6VM_OPCODE_RETURN_INTEGER,
	H6VM_OPCODE_RETURN_STRING,
	H6VM_OPCODE_RETURN_EVALUATION,

	// crz: register stack operations

	H6VM_OPCODE_PUSHRSP,
	H6VM_OPCODE_POPRSP,
	H6VM_OPCODE_LOADRSP,

	H6VM_OPCODE_NOP
};

