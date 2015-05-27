/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

#include "6IT.h"

struct value_t
{
	union
	{
		// todo crz: get rid of most of this
		int32_t as_int32;
		uint32_t as_uint32;
		int64_t as_int64;
		uint64_t as_uint64;
		short as_short;
		unsigned short as_unsigned_short;
		long as_long;
		unsigned long as_unsigned_long;
		char as_char;
		unsigned char as_unisgned_char;
		double as_double;
		float as_float;
		char const *as_char_const_ptr;

		int as_integer;
		double as_real;
		void *as_pointer;

		int32_t E;
		uint32_t e;

		int16_t X;
		uint16_t x;

		int8_t B;
		uint8_t b;

		double D;
		int64_t R;
		uint64_t r;

		float F;
		void *P;
	};
	data_type_t type;
};

struct register_t
{
	struct value_t value;
	int16_t idx;
	int16_t local_idx;
	int is_lua; // hack crz
	int is_external;
};

struct register_metadata_t
{
	char symbol[MAX_IDENTIFIER_LENGTH + 1];
	int frame;
};

struct machine_registers_t
{
	struct register_t *register_stack;
	int register_stack_top_index;
	struct value_t *evaluator_stack;
	struct value_t *evaluator_stack_top_pointer;
	int evaluator_program_counter;
	opcode_t *code_segment;
	opcode_t *instruction_pointer;
	struct frame_t *execution_stack;
	struct frame_t *frame;
	struct expression_term_t *expression_segment;

	struct register_t FRP; // crz: free absolute register pointer
	struct register_t FXP; // crz: free expression pointer
	struct register_t FCP;
	struct register_t DS;
	struct register_t FDP;
	struct register_t SS; // crz: string segment
	struct register_t FSP; // crz: free string pointer
	struct register_t US;
	struct register_t FUP;
	struct register_t ACC;
	struct register_t CMP;

};

#define _REGS(M) (&(M)->registers)

// todo crz: lots of this
#ifdef _6IT_CHECKED_BUILD
#define CHECK_STATE(M) (M)->check_state(M)
#define CHECK_VSP(M) assert(REG_VSP(M) >= -1)
#define REGISTER_CHANGED(M) (M)->state_changed(M)
#define _RSP_CHANGED(regs) //printf("RSP: %d\n", _REG_RSP(regs));// __debugbreak();
#else
#define CHECK_STATE(M)
#define CHECK_VSP(M)
#define REGISTER_CHANGED(M)
#define _RSP_CHANGED(regs)
#endif

#define _REG_US(regs) ((struct callable_unit_t *)(regs)->US.value.P)
#define _REG_FUP(regs) ((regs)->FUP.value.E)

#define _REG_SS(regs) ((char *)(regs)->SS.value.P)
#define _REG_FSP(regs) ((regs)->FSP.value.E)
#define _INC_FSP(regs) (++(regs)->FSP.value.E)

#define _REG_ACC(regs) ((regs)->ACC.value.E)
#define _REG_ACC_F(regs) ((regs)->ACC.value.F)

#define _REG_CMP(regs) ((regs)->CMP.value.E)
#define _REG_CMP_F(regs) ((regs)->CMP.value.F)

#define _REG_VPC(reg) ((reg)->evaluator_program_counter)
#define _SET_VPC(reg, v) (_REG_VPC(reg) = (v))

#define _REG_XS(regs) ((struct expression_term_t *)(regs)->expression_segment)
#define _REG_FXP(regs) ((regs)->FXP.value.E)

#define _REG_CS(reg) ((reg)->code_segment)
#define _CODE_FLOAT(regs, p) ((float)(*(_REG_CS(regs) + p)))
#define _REG_FCP(regs) ((regs)->FCP.value.E)

#define _REG_DS(regs) ((void *)(regs)->DS.value.E)
#define _REG_FDP(regs) ((regs)->FDP.value.E)

#define _REG_FRP(regs) ((regs)->FRP.value.E)

#define _REG_ES(regs) ((regs)->execution_stack)

#define _REG_ESP(regs) ((regs)->frame - _REG_ES(regs))
#define _SET_ESP(regs, p) (regs)->frame = (_REG_ES(regs) + (p))

// crz: program counter

#define _REG_PC(regs) (_REG_IP(regs) - _REG_CS(regs))
#define _SET_PC(regs, pc) (_SET_IP(regs, _REG_CS(regs) + (pc)))
#define _REG_IP(reg) (reg)->instruction_pointer
#define _SET_IP(reg, ip) ((reg)->instruction_pointer = (ip))
#define _INC_IP(reg) (++(reg)->instruction_pointer)
#define _ADD_IP(reg, v) ((reg)->instruction_pointer += (v))

// crz: register stack

#define _STATIC_REGISTER(M, idx) (_REG_RS(_REGS(M)) + (idx))

#define _FRAME(M) (_REGS(M)->frame)
#define FRAME_BASE_REGISTER_INDEX(M, F) ((F)->frame_registers - _REG_RS(_REGS(M)))
#define SAMPLE_FRAME_BASE_REGISTER_INDEX(M) (_FRAME(M)->frame_registers = (_REG_RS(_REGS(M)) + _REG_RSP(_REGS(M))))

#define _REG_RS(reg) ((reg)->register_stack)
#define _SET_FRAME_REGISTERS(M, p) (_FRAME(M)->frame_registers = (p))
#define _FRAME_REGISTER(F, idx) ((F)->frame_registers + (idx))
#define _REG_RSP(reg) ((reg)->register_stack_top_index)
#define _SET_RSP(reg, v) ((reg)->register_stack_top_index = (v)); _RSP_CHANGED(reg);
#define _INC_RSP(reg) (++(reg)->register_stack_top_index); _RSP_CHANGED(reg);
#define _ADD_RSP(reg, v) ((reg)->register_stack_top_index += (v)); _RSP_CHANGED(reg);

// crz: evaluator stack

#define _REG_VS(regs) ((regs)->evaluator_stack)

#define _REG_VSP(regs) (_REG_VIP(regs) - _REG_VS(regs))
#define _INC_VSP(regs) _INC_VIP(regs)
#define _DEC_VSP(regs) _DEC_VIP(regs)
#define _SUB_VSP(regs, v) _SUB_VIP(regs, v)
#define _SET_VSP(regs, v) _SET_VIP(regs, (_REG_VS(regs) + v))

#define _REG_VIP(reg) (reg)->evaluator_stack_top_pointer
#define _SET_VIP(reg, top) (reg)->evaluator_stack_top_pointer = (top);
#define _INC_VIP(regs) ++(regs)->evaluator_stack_top_pointer;
#define _DEC_VIP(regs) --(regs)->evaluator_stack_top_pointer;
#define _SUB_VIP(regs, v) (regs)->evaluator_stack_top_pointer -= (v)
