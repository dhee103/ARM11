#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "assembleUint.h"
/*for testing */
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
/*helper function to convert a 32 bit number from big endian to little endian*/
uint32_t bigToLitteEndian(uint32_t num) {

  return num >> 24
       | ((num & 0xff0000) >> 8)
       | ((num & 0xff00) << 8)
       | num << 24;
}

/*Function which gets the details of the instruction's opcode*/
opDetails getOpcodeDetails(char * mnemonic) {
    opDetails result;
    if(!strcmp(mnemonic,"and")) {
        result.opcode = AND;
        result.opcode_binary = B_0;
    } else if(!strcmp(mnemonic,"eor")) {
        result.opcode = EOR;
        result.opcode_binary = B_1;
    } else if(!strcmp(mnemonic,"sub")) {
        result.opcode = SUB;
        result.opcode_binary = B_2;
    } else if(!strcmp(mnemonic,"rsb")) {
        result.opcode = RSB;
        result.opcode_binary = B_3;
    } else if(!strcmp(mnemonic,"add")) {
        result.opcode = ADD;
        result.opcode_binary = B_4;
    } else if(!strcmp(mnemonic,"orr")) {
        result.opcode = ORR;
        result.opcode_binary = B_C;
    } else if(!strcmp(mnemonic,"mov")) {
        result.opcode = MOV;
        result.opcode_binary = B_D;
    } else if(!strcmp(mnemonic,"tst")) {
        result.opcode = TST;
        result.opcode_binary = B_8;
    } else if(!strcmp(mnemonic,"teq")) {
        result.opcode = TEQ;
        result.opcode_binary = B_9;
    } else if(!strcmp(mnemonic,"cmp")) {
        result.opcode = CMP;
        result.opcode_binary = B_A;
    }
    return result;
}

/* converts an number larger than 8 bits to a number less than or equal to 8 bits with a rotation */
uint32_t convertOP2(uint32_t op2_32bit) {
    uint32_t rot;
    uint32_t result = op2_32bit;
    int condition = 1;
    int rotate_value = 0;
    while(condition) {
        int bit0 = (int)(op2_32bit & B_1);
        int bit1 = (int)((op2_32bit & B_2)>>1);
        if((bit0) || (bit1)) {
            condition = 0;
        }
        if(condition) {
            /*shift value = n -> shift 2n times*/
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

/* converts a data processing instruction into binary */
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
    /*rotate value that is bigger than 8 bit*/
    if(Operand2 > 0x000000FF) {
        Operand2 = convertOP2(Operand2);
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

/* converts a multiply instruction into binary */
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

/* converts a special case instruction into binary */
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
        Rd = ins->rd;
        Operand2 = Rd;
        cond = cond << condMask_DP;
        opcode = opcode << opcodeMask_DP;
        S = S << SMask_DP;
        Rd = Rd << RdMask_DP;
        shift_value = shift_value << shiftMask_special;
        Operand2 = Operand2 | shift_value;
        result = result | cond | opcode | S | Rd | Operand2;
        result = bigToLitteEndian(result);
    }

    return result;
}

/*for testing */
int main(void) {
    /*op2_state oper2 = getOP2Details("#50331648");
    uint32_t oper2_data = oper2.data;
    uint32_t oper2_Imm  = oper2.Imm;
    printBits(oper2_data);
    printBits(oper2_Imm);*/
    uint32_t out_assembler;
    uint32_t out_assembler2;
    uint32_t out_assembler3;
    uint32_t out_assembler4;
    instruct *instr = malloc(sizeof(instruct));
    instruct *instr2 = malloc(sizeof(instruct));
    instruct *instr3 = malloc(sizeof(instruct));
    instruct *instr4 = malloc(sizeof(instruct));


    instr->mnemonic = "mov";
    instr->expression = 0;
    instr->rd = 1;
    instr->operand2 = 1;
    instr->rm = 0;
    instr->rs = 0;
    instr->rn = 0;
    instr->Imm = 1;
    out_assembler = assembler_dataProcessing(instr);
    printBits(out_assembler);

    instr2->mnemonic = "mov";
    instr2->expression = 0;
    instr2->rd = 2;
    instr2->operand2 = 23;
    instr2->rm = 0;
    instr2->rs = 0;
    instr2->rn = 1;
    instr2->Imm = 1;
    out_assembler2 = assembler_dataProcessing(instr2);
    printBits(out_assembler2);


    instr3->mnemonic = "tst";
    instr3->expression = 0;
    instr3->rd = 2;
    instr3->operand2 = 1;
    instr3->rm = 1;
    instr3->rs = 2;
    instr3->rn = 1;
    instr3->Imm = 0;
    out_assembler3 = assembler_dataProcessing(instr3);
    printBits(out_assembler3);

    instr4->mnemonic = "andeq";
    instr4->expression = 0;
    instr4->rd = 4;
    instr4->operand2 = 205;
    instr4->rm = 1;
    instr4->rs = 2;
    instr4->rn = 4;
    instr4->Imm = 1;
    out_assembler4 = assembler_special(instr4);
    printBits(out_assembler4);
return 0;
}
