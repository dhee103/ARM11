#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
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

uint32_t bigToLitteEndian(uint32_t num) {

  return num >> 24
       | ((num & 0xff0000) >> 8)
       | ((num & 0xff00) << 8)
       | num << 24;
}

uint32_t digit_to_uint32(char d) {
    uint32_t result;
    switch(d) {
    case '1' :
        result = B_1;
        break;
    case '2' :
        result = B_2;
        break;
    case '3' :
        result = B_3;
        break;
    case '4' :
        result = B_4;
        break;
    case '5' :
        result = B_5;
        break;
    case '6' :
        result = B_6;
        break;
    case '7' :
        result = B_7;
        break;
    case '8' :
        result = B_8;
        break;
    case '9' :
        result = B_9;
        break;
    case 'A' :
        result = B_A;
        break;
    case 'B' :
        result = B_B;
        break;
    case 'C' :
        result = B_C;
        break;
    case 'D' :
        result = B_D;
        break;
    case 'E' :
        result = B_E;
        break;
    case 'F' :
        result = B_F;
        break;
    default :
        result = B_0;
        break;

    }

    return result;
}

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
    /*printf("enter getOp\n");*/
if(0 == strcmp(mnemonic,"and")) {
    /*printf("got and\n");*/
    result.opcode = AND;
    result.opcode_binary = B_0;
} else if(0 == strcmp(mnemonic,"eor")) {
    /*printf("got eor\n");*/
    result.opcode = EOR;
    result.opcode_binary = B_1;
} else if(0 == strcmp(mnemonic,"sub")) {
    /*printf("got sub\n");*/
    result.opcode = SUB;
    result.opcode_binary = B_2;
} else if(0 == strcmp(mnemonic,"rsb")) {
    /*printf("got rsb\n");*/
    result.opcode = RSB;
    result.opcode_binary = B_3;
} else if(0 == strcmp(mnemonic,"add")) {
    /*printf("got add\n");*/
    result.opcode = ADD;
    result.opcode_binary = B_4;
} else if(0 == strcmp(mnemonic,"orr")) {
    /*printf("got orr\n");*/
    result.opcode = ORR;
    result.opcode_binary = B_C;
} else if(0 == strcmp(mnemonic,"mov")) {
    /*printf("got mov\n");*/
    result.opcode = MOV;
    result.opcode_binary = B_D;
} else if(0 == strcmp(mnemonic,"tst")) {
    /*printf("got tst\n");*/
    result.opcode = TST;
    result.opcode_binary = B_8;
} else if(0 == strcmp(mnemonic,"teq")) {
    /*printf("got teq\n");*/
    result.opcode = TEQ;
    result.opcode_binary = B_9;
} else if(0 == strcmp(mnemonic,"cmp")) {
    /*printf("got cmp\n");*/
    result.opcode = CMP;
    result.opcode_binary = B_A;
}
/*printf("out getOp\n");*/
return result;
}
uint32_t convertOP2(uint32_t op2_32bit) {
    uint32_t rot;
    uint32_t result = op2_32bit;
    int condition = 1;
    int rotate_value = 0;
    while(1 == condition) {
        int bit0 = (int)(op2_32bit & B_1);
        int bit1 = (int)((op2_32bit & B_2)>>1);
        if((1 == bit0) || (1 == bit1)) {
            condition = 0;
        }
        if(1 == condition) {
            printf("shift\n");
            op2_32bit = op2_32bit >> 2;
            rotate_value++;
        }
    }
    if(0 != rotate_value) {
        /*rotate backward (1 round is 16)*/
        rot = (uint32_t)(16 - rotate_value);
        result = op2_32bit | (rot << 8);
    }
    return result;
}
op2_state getOP2Details(char * operand2) {
    op2_state result;
    result.data = B_0;
    result.Imm = B_0;
    if('#' == operand2[0]) {
        result.Imm = B_1;
        operand2++;
        if(('0' == operand2[0]) && ('x' == operand2[1])) {
            uint32_t temp;
            int i = 0;
            operand2 += 2;
            while('\0' != operand2[i]) {
                temp = digit_to_uint32(operand2[i]);
                result.data = (result.data << 4) + temp;
                i++;
            }

        } else {
            result.data = (uint32_t)atoi(operand2);
        }
        result.data = convertOP2(result.data);
    } else {
        result.data = getRegAddress(operand2);
    }
    return result;
}

uint32_t assembler_dataProcessing(instruct *ins) {
    uint32_t result = B_0;
    op2_state op2 = getOP2Details(ins->operand2);
    uint32_t cond = COND_dataProcessing;
    uint32_t I = op2.Imm;
    uint32_t S = B_0;
    uint32_t Rn = B_0;
    uint32_t Rd = B_0;
    uint32_t Operand2 = op2.data;
    opDetails ins_op = getOpcodeDetails(ins->mnemonic);
    uint32_t opcode = ins_op.opcode_binary;
    /*printf("%s\n",ins->mnemonic);
    printf("%s\n",ins->operand2);
    printf("%s\n",ins->rd);
    printf("%d\n",ins_op.opcode);
    printBits(ins_op.opcode_binary);*/
    switch(ins_op.opcode) {
        /*printf("got here\n");*/
                case AND :
                case EOR :
                case ORR :
                case SUB :
                case RSB :
                case ADD :
                    Rd = getRegAddress(ins->rd);
                    Rn = getRegAddress(ins->rn);
                    break;
                case TST :
                case TEQ :
                case CMP :
                    S = B_1;
                    Rn = getRegAddress(ins->rn);
                    break;
                case MOV :
                    /*printf("got here\n");*/
                    Rd = getRegAddress(ins->rd);
                    break;

    }
    cond = cond << condMask_DP;
    I = I << IMask_DP;
    opcode = opcode << opcodeMask_DP;
    S = S << SMask_DP;
    Rn = Rn << RnMask_DP;
    Rd = Rd << RdMask_DP;
    result = result | cond | I | opcode | S | Rn | Rd | Operand2;
    result = bigToLitteEndian(result);
    return result;
}

uint32_t assembler_multiply(instruct *ins) {
    uint32_t Rd;
    uint32_t Rs;
    uint32_t Rm;
    uint32_t Rn = B_0;
    uint32_t result = B_0;
    uint32_t cond = COND_multiply;
    uint32_t A = B_0;
    uint32_t S = B_0;
    uint32_t bit4_7 = B_9;
    if(0 == strcmp(ins->mnemonic,"mla")) {
        printf("mla\n");
        A = B_1;
        Rn = getRegAddress(ins->rn);
    }
    Rd = getRegAddress(ins->rd);
    Rs = getRegAddress(ins->rs);
    Rm = getRegAddress(ins->rm);
    cond = cond << condMask_MUL;
    A = A << AMask_MUL;
    S = S << SMask_MUL;
    Rn = Rn << RnMask_MUL;
    Rd = Rd << RdMask_MUL;
    bit4_7 = bit4_7 << Mask4_7_MUL;
    Rs = Rs << RsMask_MUL;
    result = result | cond | A | S | Rn | Rd | bit4_7 | Rs | Rm;
    result = bigToLitteEndian(result);
    return result;
}



int main(void) {
    /*op2_state oper2 = getOP2Details("#50331648");
    uint32_t oper2_data = oper2.data;
    uint32_t oper2_Imm  = oper2.Imm;
    printBits(oper2_data);
    printBits(oper2_Imm);*/
    uint32_t out_assembler;
    instruct *instr = malloc(sizeof(instruct));
    instr->mnemonic = "mla";
    instr->rd = "r3";
    instr->rm = "r1";
    instr->rs = "r2";
    instr->rn = "r4";
    out_assembler = assembler_multiply(instr);
    printBits(out_assembler);
return 0;
}
