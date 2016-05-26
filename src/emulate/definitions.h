#ifndef DEFS
#define DEFS
/* This file includes definitions that will be used for the emulator */

/* Memory & Registers */
#define BYTES_IN_WORD 4
#define BITS_IN_WORD 32
#define MEMORY_SIZE 65536
#define PC 13
#define CPSR 14
#define PC_AHEAD_BYTES 8
#define PC_OFFSET 4

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

/* masks & shifts*/
#define condMask 0xf
#define condMask_shift 4
#define opcodeMask 0x7
#define oneMask 0x1 << 3
#define opcodeMask_shift 13
#define multMask 0x9 << 28
#define NFlagMask 0x1
#define NFlagShift 7
#define ZFlagMask 0x1
#define ZFlagShift 6

/* Enum named INSTR for instruction type */
typedef enum {
	DATA_PROC,
	MULTIPLY,
	SINGLE_DT,
	BRANCH,
	END
} INSTR;

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
	INSTR instruction;
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
} decoded_instr;

//state of processor
typedef struct state {
	decoded_instr *decoded;
	uint32_t *reg;
//	TODO: check if 8 or 32 bit memory
    uint8_t *memory;
    uint32_t pc;
    struct cprsFlag cprs;
	int isFetched;
	int isDecoded;
	int isExecuted;
} state;

typedef struct cprsFlag {
    uint8_t n;
    uint8_t z;
    uint8_t c;
    uint8_t v;
}cprs;

typedef struct shift_output {
	uint32_t data;
	int carry;
} shift_out;

extern uint32_t reg[15];


#endif
