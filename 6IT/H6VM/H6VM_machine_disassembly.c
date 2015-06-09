/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "H6VM.h"

#include <string.h>
#include <stdio.h>

// hack crz: replace these with generalised versions
_6IT_PRIVATE char const * _6IT_THISCALLX(machine, _H6VM_METHOD_NAME(type_base_description), data_type_t type)
{
	if (TYPE_VARARG == type)
	{
		return "...";
	}

	switch (type & TYPE_BASE_MASK)
	{
	case TYPE_VOID: return "void";
	case TYPE_CHAR: return "char";
	case TYPE_INT: return "int";
	case TYPE_LONG: return "long";
	case TYPE_FLOAT: return "float";
	case TYPE_DOUBLE: return "double";
	case TYPE_STRUCT: return "struct";
	case TYPE_UNION: return "union";
	}

	return "INVALID";
}

static char const *_H6VM_METHOD_NAME(format_machine_code)(struct machine_t *machine, int type, int code)
{
	static char s[100];

	switch (type)
	{
	case H6VM_REG_REL:
	case H6VM_REG_ABS:
	case H6VM_EXPR:
	case H6VM_PC:
	case H6VM_REG_SPECIAL:
	case H6VM_FPC:
	case H6VM_IPC:
	case H6VM_ADDR:
	case H6VM_INT32:
	case H6VM_INT16:
	case H6VM_FP32:
	case H6VM_STR:
	case H6VM_FUNC:
		sprintf(s, "%03d", code);
		break;
	case H6VM_INT16_FPC16:
	case H6VM_INT16_IPC16:
		sprintf(s, "%03d:%03d", OPERAND_16H(code), OPERAND_16L(code));
		break;
	default:
		machine->die(machine, 0, "invalid operand type");
	}

	return s;
}

static char const *_H6VM_METHOD_NAME(format_operand)(struct machine_t *machine, int type, int operand)
{
	static char s[20];
	switch (type)
	{
	case 0:
		return "";
	case H6VM_REG_REL:
		sprintf(s, "[+%d]", operand);
		break;
	case H6VM_REG_ABS:
		sprintf(s, "[+%d]", operand);
		break;
	case H6VM_EXPR:
		sprintf(s, "{%d}", operand);
		break;
	case H6VM_PC:
		sprintf(s, ">>%d", operand);
		break;
	case H6VM_REG_SPECIAL:
		sprintf(s, ".%d", operand);
		break;
	case H6VM_FPC:
		sprintf(s, ">[%d]", operand);
		break;
	case H6VM_IPC:
		sprintf(s, ">%d", operand);
		break;
	case H6VM_ADDR:
		sprintf(s, "$%d", operand);
		break;
	case H6VM_INT16:
		sprintf(s, "%d", operand);
		break;
	case H6VM_INT32:
		sprintf(s, "%d", operand);
		break;
	case H6VM_FP32:
		sprintf(s, "%f", (float)operand);
		break;
	case H6VM_STR:
		sprintf(s, "%d", operand);
		break;
	case H6VM_FUNC:
		sprintf(s, "(%d)", operand);
		break;
	case H6VM_INT16_FPC16:
		sprintf(s, "%d >[%d]", OPERAND_16H(operand), OPERAND_16L(operand));
		break;
	case H6VM_INT16_IPC16:
		sprintf(s, "%d >%d", OPERAND_16H(operand), OPERAND_16L(operand));
		break;
	default:
		machine->die(machine, 0, "invalid operand type");
	}

	return s;
}

_H6VM_METHODXX(int, get_referenced_expressions, int pc, int *expressions)
{
	int number_of_expressions = 0;

	opcode_t *_ip = _REG_CS(_REGS(_This)) + pc;
	struct mnemonic_t const *mn = _This->processor.get_mnemonic(OPCODE(_This));

	if (mn->parm_type == H6VM_EXPR)
	{
		expressions[number_of_expressions++] = OPCODE_PARM(_This);
	}

	for (int i = 0; i < mn->number_of_operands; ++i)
	{
		if (H6VM_EXPR == mn->operands[i])
		{
			expressions[number_of_expressions++] = _ip[i + 1];
		}
	}

	return number_of_expressions;
}

_H6VM_METHODX(char const *, format_primitive_value, struct value_t *value)
{
	static char buffer[30];
	switch (value->type)
	{
	case TYPE_INT:
		sprintf(buffer, "%d", value->as_integer);
		break;
	case TYPE_FLOAT:
		sprintf(buffer, "%f", value->as_float);
		break;
	case TYPE_CHAR_CONST_PTR:
		sprintf(buffer, "\"%s\"", _This->get_string(_This, value->as_integer));
		//		sprintf(buffer, "<string>");
		break;
	default:
		_This->die(_This, ERROR_H6VM_UNKNOWN_TYPE_ON_VALUE_STACK, "bad type on value stack");
		break;
	}

	return buffer;
}

_H6VM_METHODXX(char const *, disassemble_instruction, int pc, int callable_unit_PC)
{
	_SET_PC(_REGS(_This), pc);
	struct mnemonic_t const *mn = _This->processor.get_mnemonic(OPCODE(_This));

	if (!mn)
	{
		_This->die(_This, 0, "bad opcode");
	}

	static char s[500];

	if (callable_unit_PC >= 0)
	{
		sprintf(s, "%04d %04d %-10s", pc, pc - callable_unit_PC, mn->mnemonic);
	}
	else
	{
		sprintf(s, "%04d %-10s", pc, mn->mnemonic);
	}
	opcode_t codes[5];

	if (mn->parm_type)
	{
		strcat(s, " ");
		strcat(s, _H6VM_METHOD_NAME(format_operand)(_This, mn->parm_type, OPCODE_PARM(_This)));
	}

	opcode_t *operand_p = _REG_IP(_REGS(_This)) + 1;// _ip + 1;
	for (int i = 0; i < mn->number_of_operands; ++i)
	{
		int operand = *operand_p++;

		codes[i] = operand;
		strcat(s, " ");
		strcat(s, _H6VM_METHOD_NAME(format_operand)(_This, mn->operands[i], operand));
	}

	while (strlen(s) < 40) strcat(s, " ");

	strcat(s, _This->format_machine_code(_This, H6VM_INT32, mn->opcode));

	if (mn->parm_type)
	{
		strcat(s, ":");
		strcat(s, _This->format_machine_code(_This, mn->parm_type, OPCODE_PARM(_This)));
	}

	for (int i = 0; i < mn->number_of_operands; ++i)
	{
		strcat(s, " ");
		int type = mn->operands[i];
		int code = codes[i];
		char const *ss = _This->format_machine_code(_This, type, code);
		strcat(s, ss);
	}

	return s;
}

_H6VM_METHODX(char const *, type_description, data_type_t type)
{
	static char buff[30];
	buff[0] = 0;

	if (TYPE_EXPRESSION == type)
	{
		return "EXPRESSION";
	}

	if (type & TYPE_UNSIGNED)
	{
		strcat(buff, "unsigned ");
	}

	strcat(buff, _H6VM_METHOD_NAME(type_base_description)(_This, type));

	if (type & TYPE_CONST)
	{
		strcat(buff, " const");
	}

	if (type & TYPE_POINTER)
	{
		strcat(buff, " *");
	}
	else if (type & TYPE_POINTER2)
	{
		strcat(buff, " **");
	}

	return buff;
}


