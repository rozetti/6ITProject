/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6IL.h"

#include <memory.h>
#include <assert.h>
#include <stdio.h>

#define SET_FPC(PC) _SET_IP(_REGS(_Machine), entry_ip + (PC)) 
#define IP _REG_IP(_REGS(_Machine))
#define RELS _FRAME(_Machine)->frame_registers

//#define USE_SECOND_LEVEL_JUMP_TABLE

#ifdef PROCESSOR_WITH_DEBUGGER
_6IT_PRIVATE void _6IT_MACHINECALL(execute_debug)
#else
_6IT_PRIVATE void _6IT_MACHINECALL(execute)
#endif
{
	opcode_t *entry_ip = IP;

	int n;
	for (;;)
	{
#ifdef _6IT_SUPPORT_INTERRUPTS

		if (_IRQF(_Machine))
		{
			_CLEAR_IRQF(_Machine);

			for (int i = 0; i < _NUMBER_OF_INTERRUPT_LEVELS; ++i)
			{
				if (!_Machine->interrupt_levels[i]) continue;

				_Machine->interrupt_levels[i] = 0;
				
				switch (i)
				{
#ifdef _6IT_DEBU66ER
				case INTERRUPT_BREAK:
					_Machine->debugger.enter_processor_debugger(&_Machine->debugger, 0);
					break;
				case INTERRUPT_SINGLE_STEP:
					_Machine->debugger.enter_processor_debugger(&_Machine->debugger, 0);
					break;
#endif
#ifdef _6IT_SUPPORT_THREADS
				case INTERRUPT_YIELD:
					_Machine->scheduler.next(&_Machine->scheduler);
					entry_ip = _FRAME(_Machine)->entry_ip;
					break;
#endif
				}
			}
		}
#endif

#ifdef PROCESSOR_WITH_DEBUGGER
		int pc = _REG_PC(_REGS(_Machine));
		int oc = OPCODE(_Machine);
		int op1 = OPERAND1(_Machine);
		int op2 = OPERAND2(_Machine);

		if (_Machine->processor.verbosity >= 2)
		{
			_Machine->debugger.print_current_instruction_disassembly(&_Machine->debugger);
			if (_Machine->processor.verbosity >= 3)
			{
				_Machine->debugger.print_minidump(&_Machine->debugger);
				if (_Machine->processor.verbosity >= 4)
				{
					_Machine->debugger.print_relative_registers(&_Machine->debugger);
				}
			}
		}
#endif

		switch (OPCODE(_Machine))
		{
		case T6IL_ADDR:
			_REG_ACC(_REGS(_Machine)) = (_FRAME_REGISTER(_FRAME(_Machine), OPERAND1(_Machine))->value.as_integer += OPERAND1(_Machine));
			_ADD_IP(_REGS(_Machine), 2);
			break;
		case T6IL_INCR:
			_REG_ACC(_REGS(_Machine)) = ++_FRAME_REGISTER(_FRAME(_Machine), OPERAND1(_Machine))->value.as_integer;
			_INC_IP(_REGS(_Machine));
			break;
		case T6IL_INCA:
			++_REG_RS(_REGS(_Machine))[OPERAND1(_Machine)].value.as_integer;
			_INC_IP(_REGS(_Machine));
			break;
		case T6IL_INC:
			++_REG_ACC(_REGS(_Machine));
			_INC_IP(_REGS(_Machine));
			break;
		case T6IL_DECR:
			_REG_ACC(_REGS(_Machine)) = --_FRAME_REGISTER(_FRAME(_Machine), OPERAND1(_Machine))->value.as_integer;
			_ADD_IP(_REGS(_Machine), 2);
			break;
		case T6IL_DECA:
			--_REG_RS(_REGS(_Machine))[OPERAND1(_Machine)].value.as_integer;
			_ADD_IP(_REGS(_Machine), 2);
			break;
		case T6IL_DEC:
			--_REG_ACC(_REGS(_Machine));
			_INC_IP(_REGS(_Machine));
			break;

		case T6IL_JUMPIFEQ:
			if (_REG_CMP(_REGS(_Machine)) == _REG_ACC(_REGS(_Machine)))
			{
				SET_FPC(PROCESSOR_OPCODE_PARM(IP));
			}
			else
			{
				_INC_IP(_REGS(_Machine));
			}
			break;
		case T6IL_JUMPIFNE:
			if (_REG_CMP(_REGS(_Machine)) != _REG_ACC(_REGS(_Machine)))
			{
				SET_FPC(PROCESSOR_OPCODE_PARM(IP));
			}
			else
			{
				_INC_IP(_REGS(_Machine));
			}
			break;
		case T6IL_JUMPIFNEI:
			if (_REG_ACC(_REGS(_Machine)) != OPERAND1(_Machine))
			{
				SET_FPC(PROCESSOR_OPCODE_PARM(IP));
			}
			else
			{
				_ADD_IP(_REGS(_Machine), 2);
			}
			break;
			// todo crz: second stage optimisation, spot IFXXX followed immediately by RETURNINT
		case T6IL_RETURNIIFREQI:
		{
			if (_FRAME_REGISTER(_FRAME(_Machine), PROCESSOR_OPCODE_PARM(IP))->value.as_integer == OPERAND1_16H(_Machine))
			{
				PUSH_INT(_Machine, OPERAND1_16L(_Machine));

				//INC_VSP(machine);
				//_REG_VIP(_REGS(machine)).as_integer = OPERAND1_16L;
				//_REG_VIP(_REGS(machine)).type = TYPE_INT;
				return;
			}
			else _ADD_IP(_REGS(_Machine), 2);
			break;
		}
		case T6IL_JUMPIFREQI:
		{
			if (_FRAME_REGISTER(_FRAME(_Machine), PROCESSOR_OPCODE_PARM(IP))->value.as_integer == OPERAND1_16H(_Machine))
			{
				SET_FPC(OPERAND1_16L(_Machine));
			}
			else
			{
				_ADD_IP(_REGS(_Machine), 2);
			}
			break;
		}
		case T6IL_JUMPIFRNEI:
		{
			if (_FRAME_REGISTER(_FRAME(_Machine), PROCESSOR_OPCODE_PARM(IP))->value.as_integer != OPERAND1_16H(_Machine))
			{
				SET_FPC(OPERAND1_16L(_Machine)); // todo crz: downcompiler to convert this to relative jump
			}
			else
			{
				_ADD_IP(_REGS(_Machine), 2);
			}
			break;
		}
		case T6IL_FORRI:
			_FRAME_REGISTER(_FRAME(_Machine), PROCESSOR_OPCODE_PARM(IP))->value.as_integer = OPERAND1(_Machine);
			_ADD_IP(_REGS(_Machine), 3);
			break;
		case T6IL_NEXTRI:
		{
			_FRAME_REGISTER(_FRAME(_Machine), PROCESSOR_OPCODE_PARM(IP))->value.as_integer += OPERAND2_16H(_Machine);

			(_FRAME_REGISTER(_FRAME(_Machine), PROCESSOR_OPCODE_PARM(IP))->value.as_integer == (OPERAND1(_Machine))) ? _ADD_IP(_REGS(_Machine), 3) : SET_FPC(OPERAND2_16L(_Machine));
			break;
		}
		case T6IL_NEXTRINCI:
			(++_FRAME_REGISTER(_FRAME(_Machine), PROCESSOR_OPCODE_PARM(IP))->value.as_integer == (OPERAND1(_Machine))) ? _ADD_IP(_REGS(_Machine), 3) : SET_FPC(OPERAND2(_Machine));
			break;
		case T6IL_NEXTRDECI:
			(--_FRAME_REGISTER(_FRAME(_Machine), PROCESSOR_OPCODE_PARM(IP))->value.as_integer == OPERAND1(_Machine)) ? _ADD_IP(_REGS(_Machine), 3) : SET_FPC(OPERAND2(_Machine));
			break;

		case H6VM_OPCODE_LOADRSP:
			_SET_RSP(_REGS(_Machine), (FRAME_BASE_REGISTER_INDEX(_Machine, _FRAME(_Machine)) + PROCESSOR_OPCODE_PARM(IP)));
			_INC_IP(_REGS(_Machine));
			break;

#ifdef USE_SECOND_LEVEL_JUMP_TABLE
		default:
			switch (OPCODE(_Machine))
			{
#endif
			case H6VM_OPCODE_EXEC:
#ifdef _6IT_STACK_DISPATCH
			{
				_SET_VPC(_REGS(_Machine), PROCESSOR_OPCODE_PARM(IP));
				struct value_t *vsp = _REG_VIP(_REGS(_Machine));
				_Machine->evaluate(_Machine);
				_SET_VIP(_REGS(_Machine), vsp);
				_INC_IP(_REGS(_Machine));
			}
#else
				if (-1 == _FRAME(_Machine)->evaluator_program_counter)
				{
					_SET_VPC(_REGS(_Machine), PROCESSOR_OPCODE_PARM(IP));
					_FRAME(_Machine)->evaluator_stack_pointer = _REG_VIP(_REGS(_Machine));
				}
				else
				{
					_SET_VPC(_REGS(_Machine), _FRAME(_Machine)->evaluator_program_counter);
				}

				if (_Machine->evaluate(_Machine))
				{
					_SET_VIP(_REGS(_Machine), _FRAME(_Machine)->evaluator_stack_pointer);
				}
				entry_ip = _FRAME(_Machine)->entry_ip;
#endif
				break;
			case H6VM_OPCODE_EVAL:
#ifdef _6IT_STACK_DISPATCH
				_SET_VPC(_REGS(_Machine), PROCESSOR_OPCODE_PARM(IP));
				_Machine->evaluate(_Machine);
				_INC_IP(_REGS(_Machine));
#else
				if (-1 == _FRAME(_Machine)->evaluator_program_counter)
				{
					_SET_VPC(_REGS(_Machine), PROCESSOR_OPCODE_PARM(IP));
				}
				else
				{
					_SET_VPC(_REGS(_Machine), _FRAME(_Machine)->evaluator_program_counter);
				}

				_Machine->evaluate(_Machine);
				entry_ip = _FRAME(_Machine)->entry_ip;
#endif
				break;
			case H6VM_OPCODE_CONDITION:
#ifdef _6IT_STACK_DISPATCH
				_SET_VPC(_REGS(_Machine), PROCESSOR_OPCODE_PARM(IP));
				_Machine->evaluate(_Machine);
				_Machine->condition_flag = (0 != TOP_INT(_Machine));
				DROP(_Machine);
				_INC_IP(_REGS(_Machine));
#else
				if (-1 == _FRAME(_Machine)->evaluator_program_counter)
				{
					_SET_VPC(_REGS(_Machine), PROCESSOR_OPCODE_PARM(IP));
				}
				else
				{
					_SET_VPC(_REGS(_Machine), _FRAME(_Machine)->evaluator_program_counter);
				}

				if (_Machine->evaluate(_Machine))
				{
					int n = POP_INT(_Machine);
					_Machine->condition_flag = (0 != n);
				}
				entry_ip = _FRAME(_Machine)->entry_ip;
#endif
				break;
			case H6VM_OPCODE_ASSERT:
				if (!_Machine->condition_flag)
				{
					_Machine->printf(_Machine, "assert failed\n");
#ifdef _6IT_DEBU66ER
					_Machine->debugger.enter_processor_debugger(&_Machine->debugger, 0);
#else
					// todo crz: possibly barf out here
#endif
				}
				_INC_IP(_REGS(_Machine));
				break;
			case H6VM_OPCODE_RETURN_EVALUATION:
				_REG_VPC(_REGS(_Machine)) = PROCESSOR_OPCODE_PARM(IP);
				_Machine->evaluate(_Machine); // todo crz: flat dispatch version
				return;

			case T6IL_RETURNINT:
			{
				PUSH_INT(_Machine, PROCESSOR_OPCODE_PARM(IP));

#ifdef _6IT_STACK_DISPATCH
				return;
#else
				if (_FRAME(_Machine)->position == 0) return;

				_SET_IP(_REGS(_Machine), _FRAME(_Machine)->return_ip);
				_SET_RSP(_REGS(_Machine), FRAME_BASE_REGISTER_INDEX(_Machine, _FRAME(_Machine)));
				--_FRAME(_Machine);
				break;
#endif
			}

			case H6VM_OPCODE_RETURN:
#ifdef _6IT_STACK_DISPATCH
				return;
#else
				if (_FRAME(_Machine)->position == 0) return;

				_SET_IP(_REGS(_Machine), _FRAME(_Machine)->return_ip);
				_SET_RSP(_REGS(_Machine), FRAME_BASE_REGISTER_INDEX(_Machine, _FRAME(_Machine)));
				--_FRAME(_Machine);
				break;
#endif
			case H6VM_OPCODE_RETURN_INTEGER:
				PUSH_INT(_Machine, PROCESSOR_OPCODE_PARM(IP));
#ifdef _6IT_STACK_DISPATCH // todo crz
				return;
#else
				break;
#endif
			case H6VM_OPCODE_IFB:
			case H6VM_OPCODE_IFS:
				_Machine->condition_flag ? _INC_IP(_REGS(_Machine)) : SET_FPC(PROCESSOR_OPCODE_PARM(IP));
				break;
			case H6VM_OPCODE_RETURN_FLOAT:
				PUSH_FLOAT(_Machine, (float)OPERAND2(_Machine));
				return;
			case H6VM_OPCODE_JUMPIF:
				_Machine->condition_flag ? SET_FPC(OPERAND1(_Machine)) : _ADD_IP(_REGS(_Machine), 2);
				break;
			case H6VM_OPCODE_WHILEB:
			case H6VM_OPCODE_WHILES:
				_Machine->condition_flag ? _ADD_IP(_REGS(_Machine), 2) : SET_FPC(OPERAND1(_Machine) + 3);
				break;
			case H6VM_OPCODE_FORS:
			case H6VM_OPCODE_FORB:
				_Machine->condition_flag ? _INC_IP(_REGS(_Machine)) : SET_FPC(PROCESSOR_OPCODE_PARM(IP) + 3);
				break;
			case H6VM_OPCODE_NEXT:
				_Machine->condition_flag ? SET_FPC(PROCESSOR_OPCODE_PARM(IP)) : _INC_IP(_REGS(_Machine));
				break;

			case H6VM_OPCODE_BREAK:
			{
#ifdef _6IT_DEBU66ER
				*_REG_IP(_REGS(_Machine)) = (_Machine->debug->breakpoints + PROCESSOR_OPCODE_PARM(IP))->opcode;;
				_Machine->interrupt_controller.request(&_Machine->interrupt_controller, INTERRUPT_BREAK);
#else
				// todo crz: do something bad here
#endif
				break;
			}
			case H6VM_OPCODE_DROP:
				DROP(_Machine);
				_INC_IP(_REGS(_Machine));
				break;
			case H6VM_OPCODE_NOP:
				_INC_IP(_REGS(_Machine));
				break;
			default:
				sprintf(_Machine->exception.message, "invalid opcode '%d' at PC=%d", OPCODE(_Machine), _REG_PC(_REGS(_Machine)));
				THROW(&_Machine->exception, 1);
			}
#ifdef USE_SECOND_LEVEL_JUMP_TABLE
		}
#endif
	}
}
