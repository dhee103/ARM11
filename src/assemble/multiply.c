#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdint.h>
#include"definitions.h"

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
