#ifndef __DEFS_H
#define __DEFS_H
/* This file includes definitions that will be used for the emulator */

/* Memory & Registers */
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#define BYTES_IN_WORD 4
#define MEM_SIZE 65536
#define NUM_REGISTERS 17
#define PC 13
#define CPSR 14
#define PC_AHEAD_BYTES 8
#define BITS_IN_BYTE 8

/* Bit position */
#define MS_BIT 31
#define N_BIT 31
#define Z_BIT 30
#define C_BIT 29
#define V_BIT 28
#define I_BIT 25
#define S_BIT 20
#define A_BIT 21
#define P_BIT 24
#define U_BIT 23
#define L_BIT 20
#define isBranch_bit 27
#define isSDT_bit 26
#define isMult_bit 25

/* masks & shifts*/
#define condMask 0xf
#define condMask_shift 4
#define opcodeMask 0x7
#define oneMask 0x1 << 3
#define opcodeMask_shift 13
#define multMask 0x9 << 28
#define BRANCHSHIFT 2
#define check_bit31 0x80000000
#define check_bit0 0x00000001
#define check_bit4 0x01
#define check_shift_type 0x06
#define check_shift_value 0xF8
#define check_Rs 0xf0
#define check_bit0_7 0x000000FF
#define check_bit8_11 0x00000F00
#define check_bit0_3 0x0000000F
#define check_bit4_11 0x00000FF0
#define FULLSHIFT 31

/* Enum named COND for instruction condition */
typedef enum {
	EQ = 0,
	NE = 1,
	GE = 10,
	LT = 11,
	GT = 12,
	LE = 13,
	AL = 14
} COND;

/*Enum named OPCODE Data Processing instructions */
typedef enum {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} OPCODE;

/* Enum named SHIFT for shift type */
typedef enum {
	LSL = 0,
	LSR = 1,
	ASR = 2,
	ROR = 3
} SHIFT;

/* information held by the decoded instruction */
typedef struct decoded_instruction {
	COND condition;
	OPCODE opcode;
	SHIFT shift;
	uint32_t shiftAmount;
	int isImm;
	int isSet;
	int isAcc;
	int isPre;
	int isUp;
	int isLoad;
	uint32_t rd;
	uint32_t rn;
	uint32_t rs;
	uint32_t rm;
	uint32_t offset;
    uint32_t operand2;
} decoded_instr;

typedef struct cpsrFlag {
	int nbit;
	int zbit;
	int cbit;
	int vbit;
} cpsr;

typedef struct state {
	decoded_instr *decoded;
	uint32_t *reg;
	uint8_t *memory;
    cpsr *cpsrFlag;
} state;

typedef struct shift_output {
	uint32_t data;
	int carry;
} shift_out;

#endif
