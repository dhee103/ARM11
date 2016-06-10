#ifndef __DEF_H
#define __DEF_H

#include <stdint.h>

#define U 1 << 23
#define PIPELINE_EFFECT 8
#define Cond 14
#define PC 15
#define P 24
#define Bits25 25
#define U 1 << 23
#define Bits26  26
#define Bits28  28
#define Bits12  12
#define Bits20  20
#define Bits16  16
#define Bits25  25
#define FIVE 5
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

enum {

  eq = 0,
  ne = 1,
  ge = 10,
  lt = 11,
  gt = 12,
  le = 13,
  al = 14,

};

typedef struct instruction {

  char *mnemonic;
  uint32_t operand2;
  uint32_t rd; 
  uint32_t rn; 
  uint32_t rs;
  uint32_t rm;
  uint32_t imm;
  uint32_t expression;
  uint32_t lastAdd;
  uint32_t p;
  uint32_t u;

} instruct;

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


#endif 
