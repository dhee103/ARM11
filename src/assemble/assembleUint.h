#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#define condMask_DP 28
#define IMask_DP 25
#define opcodeMask_DP 21
#define SMask_DP 20
#define RnMask_DP 16
#define RdMask_DP 12
#define condMask_MUL 28
#define AMask_MUL 21
#define SMask_MUL 20
#define RnMask_MUL 12
#define RdMask_MUL 16
#define Mask4_7_MUL 4
#define RsMask_MUL 8
#define COND_dataProcessing 0x0000000E
#define COND_multiply 0x0000000E
#define shiftMask_special 7

typedef struct instruction {
    char *mnemonic;
    uint32_t rd;
    uint32_t rn;
    uint32_t rs;
    uint32_t rm;
    uint32_t operand2;
    uint32_t Imm;
} instruct;

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

/*typedef struct opcodeDetails {
    OPCODE opcode;
    uint32_t opcode_binary;
} opDetails;*/

#endif
