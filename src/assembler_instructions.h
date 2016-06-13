#ifndef __ASSEMBLER_INSTRUCTIONS_H
#define __ASSEMBLER_INSTRUCTIONS_H

#include "assembler_definitions.h"

uint32_t assembler_dataProcessing(instruct *ins);
uint32_t assembler_multiply(instruct *ins);
uint32_t single_data_process(instruct *ins);
uint32_t assembler_special(instruct *ins);
uint32_t branch(instruct *ins, uint32_t currAddress);
uint32_t convertOP2(uint32_t op2_32bit);
OPCODE getOpcodeDetails(char * mnemonic);
uint32_t getCond (char *cond);

#endif
