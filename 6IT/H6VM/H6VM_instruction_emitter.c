/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

INSTRUCTION_EMITTER_METHOD(int, emit_placeholder)
{
	_REG_CS(_REGS(_This->machine))[_REG_FCP(_REGS(_This->machine))] = 1729;

	return _REG_FCP(_REGS(_This->machine))++;
}

INSTRUCTION_EMITTER_METHODXX(int, emit_instruction, int opcode, int parm)
{
	_REG_CS(_REGS(_This->machine))[_REG_FCP(_REGS(_This->machine))] = MAKE_OPCODE(opcode, parm);

	return _REG_FCP(_REGS(_This->machine))++;
}

INSTRUCTION_EMITTER_METHODX(int, emit_PC, int pc)
{
	_REG_CS(_REGS(_This->machine))[_REG_FCP(_REGS(_This->machine))] = pc;

	return _REG_FCP(_REGS(_This->machine))++;
}

INSTRUCTION_EMITTER_METHODX(int, emit_integer, int n)
{
	_REG_CS(_REGS(_This->machine))[_REG_FCP(_REGS(_This->machine))] = n;

	return _REG_FCP(_REGS(_This->machine))++;
}

INSTRUCTION_EMITTER_METHODX(int, emit_float, float f)
{
	// todo crz: I don't like this
	_CODE_FLOAT(_REGS(_This->machine), _REG_FCP(_REGS(_This->machine))) = f;

	return _REG_FCP(_REGS(_This->machine))++;
}

INSTRUCTION_EMITTER_METHODX(int, emit_expression_index, int index)
{
	_REG_CS(_REGS(_This->machine))[_REG_FCP(_REGS(_This->machine))] = index;

	return _REG_FCP(_REGS(_This->machine))++;
}

INSTRUCTION_EMITTER_METHODXX(void, set_code, int pc, int code)
{
	_REG_CS(_REGS(_This->machine))[pc] = code;
}

_6IT_PUBLIC _6IT_CONSTRUCTOR(instruction_emitter, instruction_emitter)
{
	_This->machine = _Machine;

	_This->emit_instruction = emit_instruction;
	_This->emit_PC = emit_PC;
	_This->emit_integer = emit_integer;
	_This->emit_float = emit_float;
	_This->emit_expression_index = emit_expression_index;
	_This->emit_placeholder = emit_placeholder;
	_This->set_code = set_code;
}
