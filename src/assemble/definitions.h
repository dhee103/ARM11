#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include "fileUtils.h"

#define MAX_LINE_LENGTH 512
#define condMask 28
#define IMask 25
#define opcodeMask 21
#define SMask 20
#define RnMask 16
#define RdMask 12
#define COND_dataProcessing 0x0000000E
#define B_0 0x00000000
#define B_1 0x00000001
#define B_2 0x00000002
#define B_3 0x00000003
#define B_4 0x00000004
#define B_5 0x00000005
#define B_6 0x00000006
#define B_7 0x00000007
#define B_8 0x00000008
#define B_9 0x00000009
#define B_A 0x0000000A
#define B_B 0x0000000B
#define B_C 0x0000000C
#define B_D 0x0000000D
#define B_E 0x0000000E
#define B_F 0x0000000F
#define MAX_12B 0xFFF



typedef struct operand2_state {
    uint32_t data;
    uint32_t Imm;
} op2_state;

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

typedef struct opcodeDetails {
    OPCODE opcode;
    uint32_t opcode_binary;
} opDetails;

#endif