#ifndef DEFS
#define DEFS
/* This file includes defintions that will be used for the emulator */

/* Memory & Registers */
#define BYTES_IN_WORD 4
#define BITS_IN_WORD 32
#define MEMORY_SIZE 65536
#define NUM_REGISTERS 17
#define SP 13
#define LR 14
#define PC 15
#define CPSR 16
#define PC_AHEAD_BYTES 8

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

/* Enum named INSTRUCTION for instruction type */
typedef enum {
	DATA_PROC,
	MULTIPLY,
	SINGLE_DT,
	BRANCH,
	TERMINATION
} INSTRUCTION;

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

/*Enum named OPCODE_T Data Processing instructions */
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
} OPCODE_T;

/* Enum named SHIFT_T for shift type */
typedef enum {
	LSL = 0,
	LSR = 1,
	ASR = 2,
	ROR = 3
} SHIFT_T;

#endif
