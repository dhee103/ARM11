#include "assembler_instructions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

uint32_t assembler_multiply(instruct *ins) {
    uint32_t result;
    uint32_t Rd;
    uint32_t Rs;
    uint32_t Rm;
    /*initiating values to 0*/
    uint32_t Rn = 0;
    uint32_t A = 0;
    uint32_t S = 0;
    /*multiply instructions have cond = 0x0000000E*/
    uint32_t cond = COND_multiply;
    /*bit 4-7 are the constant 1001*/
    uint32_t bit4_7 = 9;
    /*mla uses additional register Rn
    and sets A bit to be 1*/
    if(!strcmp(ins->mnemonic,"mla")) {
        A = 1;
        Rn = ins->rn;
    }
    Rd = ins->rd;
    Rs = ins->rs;
    Rm = ins->rm;
    /*shift all fields to there corresponding
    starting positions then combine them by logical OR*/
    cond = cond << condMask_MUL;
    A = A << AMask_MUL;
    S = S << SMask_MUL;
    Rn = Rn << RnMask_MUL;
    Rd = Rd << RdMask_MUL;
    bit4_7 = bit4_7 << Mask4_7_MUL;
    Rs = Rs << RsMask_MUL;
    result = cond | A | S | Rn | Rd | bit4_7 | Rs | Rm;
    return result;
}

uint32_t single_data_process(instruct *ins) {

    uint32_t condition = Cond;
    uint32_t offset = ins -> operand2;
    uint32_t rd = ins -> rd;
    uint32_t rn = ins -> rn;
    uint32_t p = (ins -> p) << P;
    uint32_t l = 1;

    if ((ins -> mnemonic)[0] == 's') {
        l = 0;
    }

    return p | (1 << Bits26) | (rd << Bits12) | (condition << Bits28) | abs(offset) | (l << Bits20) | (rn << Bits16)
           | (ins -> u ? U : 0) | ((ins -> imm == 0) << Bits25);

}

/* converts a special case instruction into binary */
uint32_t assembler_special(instruct *ins) {
    uint32_t result;

    if(!strcmp(ins->mnemonic,"lsl")) {
        /*lsl Rn, <#expression> is a mov Rn,Rn, lsl <#expression>
        so Imm is 0, operand2 is the register rd with the shift value
        from the expression*/
        uint32_t shift_value;
        uint32_t Operand2;
        /*got expression from the loader function as operand2
        this value is the shift value*/
        shift_value = ins->operand2;
        Operand2 = ins->rd;
        shift_value = shift_value << shiftMask_special;
        /*Operand2 combine a register address and the shift value*/
        Operand2 = Operand2 | shift_value;
        ins-> imm = 0;
        /*set operand2 to the new Operand2 for mov instruction*/
        ins->operand2 = Operand2;
        /*we process lsl as a mov instruction*/
        ins->mnemonic = "mov";
        result = assembler_dataProcessing(ins);
    }

    return result;
}

uint32_t branch(instruct *ins, uint32_t currAddress) {

    int32_t offset = ins -> expression - currAddress - PIPELINE_EFFECT;
    char type[2];
    type[0] = (ins -> mnemonic)[1];
    type[1] = (ins -> mnemonic)[2];
    uint32_t condition = getCond(type);

    return (condition << Bits28) | (FIVE << Bits25) | ((offset >> 2) & 0x00ffffff);

}

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

uint32_t getCond (char *cond) {
    uint32_t result = al;
    if (strncmp("eq", cond, 2) == 0) {
        result = eq;
    } else if (strncmp("ne", cond, 2) == 0) {
        result = ne;
    } else if (strncmp("ge", cond, 2) == 0) {
        result = ge;
    } else if (strncmp("lt", cond, 2) == 0) {
        result = lt;
    } else if (strncmp("gt", cond, 2) == 0) {
        result = gt;
    } else if (strncmp("le", cond, 2) == 0) {
        result = le;
    }
    return result;
}