#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdint.h>
#include"definitions.h"
#include"data_process.c"

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
