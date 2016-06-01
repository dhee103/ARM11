#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "assembleUint.h"
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


opDetails getOpcodeDetails(char * mnemonic) {
    opDetails result;
    if(0 == strcmp(mnemonic,"and")) {
        result.opcode = AND;
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


uint32_t assembler_dataProcessing(instruct *ins) {
    uint32_t result = B_0;
    uint32_t cond = COND_dataProcessing;
    uint32_t I = ins->Imm;
    uint32_t S = B_0;
    uint32_t Rn = B_0;
    uint32_t Rd = B_0;
    uint32_t Operand2 = ins->operand2;
    opDetails ins_op = getOpcodeDetails(ins->mnemonic);
    uint32_t opcode = ins_op.opcode_binary;
    switch(ins_op.opcode) {
                case AND :
                case EOR :
                case ORR :
                case SUB :
                case RSB :
                case ADD :
                    Rd = ins->rd;
                    Rn = ins->rn;
                    break;
                case TST :
                case TEQ :
                case CMP :
                    S = B_1;
                    Rn = ins->rn;
                    break;
                case MOV :
                    Rd = ins->rd;
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
        A = B_1;
        Rn = ins->rn;
    }
    Rd = ins->rd;
    Rs = ins->rs;
    Rm = ins->rm;
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

uint32_t assembler_special(instruct *ins) {
    uint32_t result =B_0;

    if(0 == strcmp(ins->mnemonic,"lsl")) {
        uint32_t shift_value;
        uint32_t Operand2;
        /*lsl is a mov so cond = COND_dataProcessing*/
        uint32_t cond = COND_dataProcessing;
        /*from mov*/
        uint32_t S = B_0;
        uint32_t Rd = B_0;
        opDetails ins_op = getOpcodeDetails("mov");
        uint32_t opcode = ins_op.opcode_binary;
        shift_value = ins->expression;
        Rd = ins->rn;
        Operand2 = Rd;
        cond = cond << condMask_DP;
        opcode = opcode << opcodeMask_DP;
        S = S << SMask_DP;
        Rd = Rd << RdMask_DP;
        shift_value = shift_value << shiftMask_special;
        Operand2 = Operand2 | shift_value;
        result = result | cond | opcode | S | Rd | Operand2;
        printBits(result);
        result = bigToLitteEndian(result);
    }

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
    instr->mnemonic = "lsl";
    instr->expression = 1;
    /*instr->rd = "r3";
    instr->rm = "r1";
    instr->rs = "r2";*/
    instr->rn = 1;
    out_assembler = assembler_special(instr);
    printBits(out_assembler);
return 0;
}
