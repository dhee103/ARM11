#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdint.h>
#include"definitions.h"

/* converts an number larger than 8 bits to a number less than
or equal to 8 bits with a rotation */
uint32_t convertOP2(uint32_t op2_32bit) {
    uint32_t rot;
    uint32_t result = op2_32bit;
    int condition = 1;
    int rotate_value = 0;
    while(condition) {
        /*check bit0 and bit1*/
        int bit0 = (op2_32bit & 1);
        int bit1 = ((op2_32bit & 2)>>1);
        /*stop the while loop either bit0 or bit1 is 1*/
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
        /*count rotate backward (1 round is 16)*/
        rot = (uint32_t)(16 - rotate_value);
        /*rot is the shift value which starts at bit 8*/
        result = op2_32bit | (rot << 8);
    }
    return result;
}

OPCODE getOpcodeDetails(char * mnemonic) {
    OPCODE result;
    if(!strcmp(mnemonic,"and")) {
        result = AND;
    } else if(!strcmp(mnemonic,"eor")) {
        result = EOR;
    } else if(!strcmp(mnemonic,"sub")) {
        result = SUB;
    } else if(!strcmp(mnemonic,"rsb")) {
        result = RSB;
    } else if(!strcmp(mnemonic,"add")) {
        result = ADD;
    } else if(!strcmp(mnemonic,"orr")) {
        result = ORR;
    } else if(!strcmp(mnemonic,"mov")) {
        result = MOV;
    } else if(!strcmp(mnemonic,"tst")) {
        result = TST;
    } else if(!strcmp(mnemonic,"teq")) {
        result = TEQ;
    } else if(!strcmp(mnemonic,"cmp")) {
        result = CMP;

    }
    return result;
}

uint32_t assembler_dataProcessing(instruct *ins) {
    uint32_t result;
    /*initiating values to 0*/
    uint32_t S = 0;
    uint32_t Rn = 0;
    uint32_t Rd = 0;
    /*all data processing instructions have cond = 0x0000000E*/
    uint32_t cond = COND_dataProcessing;
    /*receive I bit from the loader function*/
    uint32_t I = ins -> imm;
    /*receive Operand2 from the loader function*/
    uint32_t Operand2 = ins->operand2;
    /*get an enum of the instruction*/
    OPCODE ins_op = getOpcodeDetails(ins->mnemonic);
    uint32_t opcode = ins_op;
    switch(ins_op) {
                case AND :
                case EOR :
                case ORR :
                case SUB :
                case RSB :
                case ADD :
                    /*These instructions set Rd and Rn*/
                    Rd = ins->rd;
                    Rn = ins->rn;
                    break;
                case TST :
                case TEQ :
                case CMP :
                    /*These instructions set S bit to be 1
                    and set Rn from corresponding input rn*/
                    S = 1;
                    Rn = ins->rn;
                    break;
                case MOV :
                    /*mov instruction uses just 1 register(Rd)*/
                    Rd = ins->rd;
                    break;

    }
    /*rotate value that is bigger than 8 bit to be
    an 8 bit with a shift value*/
    if(Operand2 > 0x000000FF) {
        Operand2 = convertOP2(Operand2);
    }
    /*each components are in the different bit positions in
    the binary code so I shift them to the start positions of
    each fields then combine them together by logical OR operation
    to get the binary instruction*/
    cond = cond << condMask_DP;
    I = I << IMask_DP;
    opcode = opcode << opcodeMask_DP;
    S = S << SMask_DP;
    Rn = Rn << RnMask_DP;
    Rd = Rd << RdMask_DP;
    result = cond | I | opcode | S | Rn | Rd | Operand2;

    return result;
}
