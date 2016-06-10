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
#define PC 15
#define PC_AHEAD_BYTES 8
#define BITS_IN_BYTE 8
#define BITS_IN_WORD 32
#define BRANCH_AHEAD_BYTES 4
#define MAX_32_BIT_NUM 0xFFFFFFFF

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
#define BRANCH_BIT 27
#define SDT_BIT 26
#define OP_REG_BIT 4

/* Bit positions for extracting multiple bits at a time using the half open
 * interval [begin,end) */
#define MULT_BIT_START 4
#define MULT_BIT_END 8
#define COND_START 28
#define COND_END 32
#define OPCODE_START 21
#define OPCODE_END 25
#define OPERAND2_START 0
#define OPERAND2_END 12
#define RN_START 16
#define RN_END 20
#define RD_START 12
#define RD_END 16
#define M_RN_START 12
#define M_RN_END 16
#define M_RD_START 16
#define M_RD_END 20
#define RS_START 8
#define RS_END 12
#define RM_START 0
#define RM_END 4
#define SDT_OFFSET_START 0
#define SDT_OFFSET_END 12
#define BR_OFFSET_START 0
#define BR_OFFSET_END 24
#define SHIFT_T_START 5
#define SHIFT_T_END 7
#define SHIFT_AMOUNT_START 7
#define SHIFT_AMOUNT_END 12
#define OPT_SHIFT_AMOUNT_START 8
#define OPT_SHIFT_AMOUNT_END 12

/* masks & shifts*/
#define MAX_MASK 0xFFFFFFFF
#define MAX_SHIFT 32
#define OFFSET_SIGN_MASK 0x00800000
#define OFFSET_BITS_MASK 0x00FFFFFF
#define NOT_OFFSET_BITS_MASK 0xFF000000
#define BRANCH_SHIFT 2
#define MASK_BIT_31 0x80000000
#define MASK_BIT_0 0x00000001
#define MASK_IMM 0x000000FF
#define MASK_ROT 0x00000F00
#define SHIFT_ROT 8


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

/* Enum named GPIO for the different cases for GPIO pins */
typedef enum {
    PIN_OFF = 0x20200028,
    PIN_ON = 0x2020001C,
    PIN_0_9 = 0x20200000,
    PIN_10_19 = 0x20200004,
    PIN_20_29 = 0x20200008,
} GPIO;

/* information held by the decoded instruction */
typedef struct decoded_instruction {
	OPCODE opcode;
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

/* struct for cpsr flag */
typedef struct cpsrFlag {
	int nbit;
	int zbit;
	int cbit;
	int vbit;
} cpsr;

/* struct for the state of the processor
 * It contains a pointers to decoded instructions, 32 bit registers, 8 bit
 * memory and to the cpsr flag */
typedef struct state {
	decoded_instr *decoded;
	uint32_t *reg;
	uint8_t *memory;
    cpsr *cpsrFlag;
} state;

/* struct for the output from a shifter
 * Enables us to use one function to return both the data and carry bit */
typedef struct shift_output {
	uint32_t data;
	int carry;
} shift_out;

#endif
