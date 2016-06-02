#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "definitions.h"
//directly using anans (original design)

uint32_t getRegAddress(char *reg) {
if (0 == strcmp(reg,"r0")){
    return B_0;
} else if (0 == strcmp(reg, "r1")) {
    return B_1;
} else if (0 == strcmp(reg, "r2")) {
    return B_2;
} else if (0 == strcmp(reg, "r3")) {
    return B_3;
} else if (0 == strcmp(reg, "r4")) {
    return B_4;
} else if (0 == strcmp(reg, "r5")) {
    return B_5;
} else if (0 == strcmp(reg, "r6")) {
    return B_6;
} else if (0 == strcmp(reg, "r7")) {
    return B_7;
} else if (0 == strcmp(reg, "r8")) {
    return B_8;
} else if (0 == strcmp(reg, "r9")) {
    return B_9;
} else if (0 == strcmp(reg, "r10")) {
    return B_A;
} else if (0 == strcmp(reg, "r11")) {
    return B_B;
} else if (0 == strcmp(reg, "r12")) {
    return B_C;
} else if (0 == strcmp(reg, "r13")) {
    return B_D;
} else if (0 == strcmp(reg, "r14")) {
    return B_E;
} else  {
    return B_F;
}
}

opDetails getOpcodeDetails(char * mnemonic) {
    opDetails result;
if(0 == strcmp(mnemonic,"and")) {
    result.opcode = MOV;
    result.opcode_binary = B_0;
} else if(0 == strcmp(mnemonic,"eor")) {
    result.opcode = EOR;
    result.opcode_binary = B_1;
} else if(0 == strcmp(mnemonic,"sub")) {
    result.opcode = SUB;
    result.opcode_binary = B_2;
} else if(0 == strcmp(mnemonic,"rsb")) {
    result.opcode = RSB;
    result.opcode_binary = B_3;
} else if(0 == strcmp(mnemonic,"add")) {
    result.opcode = ADD;
    result.opcode_binary = B_4;
} else if(0 == strcmp(mnemonic,"orr")) {
    result.opcode = ORR;
    result.opcode_binary = B_C;
} else if(0 == strcmp(mnemonic,"mov")) {
    result.opcode = MOV;
    result.opcode_binary = B_D;
} else if(0 == strcmp(mnemonic,"tst")) {
    result.opcode = TST;
    result.opcode_binary = B_8;
} else if(0 == strcmp(mnemonic,"teq")) {
    result.opcode = TEQ;
    result.opcode_binary = B_9;
} else if(0 == strcmp(mnemonic,"cmp")) {
    result.opcode = CMP;
    result.opcode_binary = B_A;
}
return result;
}

op2_state getOP2Details(char * operand2) {
    op2_state result;

    return result;
}

uint32_t assembler(instruct *ins) {
    uint32_t result = B_0;
    op2_state op2 = getOP2Details(ins->operand2);
    uint32_t cond = COND_dataProcessing;
    uint32_t I = op2.Imm;
    uint32_t S = B_0;
    uint32_t Operand2 = op2.data;
    opDetails ins_op = getOpcodeDetails(ins->mnemonic);
    uint32_t opcode = ins_op.opcode_binary;
    uint32_t Rn;
    uint32_t Rd;
    switch(ins_op.opcode) {
                case AND :
                case EOR :
                case ORR :
                case SUB :
                case RSB :
                case ADD :

                    break;

                case TST :
                case TEQ :
                case CMP :
                    S = B_1;

                    break;
                case MOV :

                    break;

    }
    cond = cond << condMask;
    I = I << IMask;
    opcode = opcode << opcodeMask;
    S = S << SMask;
    Rn = Rn << RnMask;
    Rd = Rd << RdMask;
    result = result & cond & I & opcode & S & Rn & Rd;
    return result;
}

void printBits(uint32_t x) {
  int i;
  uint32_t mask = 1 << 31;
  for(i=0; i<32; ++i) {
    if((x & mask) == 0){
      printf("0");
    }
    else {
      printf("1");
  }
    x = x << 1;
  }
    printf("\n");
  }

//int main(void) {
//    uint32_t opc = getOpcodeDetails("mov").opcode_binary;
//    printBits(opc);
//return 0;
//}