#include "getCodes.h"
#include "instructions.h"
#include "instructionCycle.h"
#include "utilities.h"

#include <stdio.h>


int bitAccess(int index, uint32_t instruction) {
    uint32_t mask = 1;
    mask = mask << index;
    return (mask & instruction) != 0;
}

// gets instruction in big endian form
uint32_t getInstruction(state *st) {
//    make sure it's a multiple of 4 (valid alignment)
    uint32_t pcVal = st->reg[PC];
    uint32_t instr = 0;
    for (int i=0; i < 4; i++) {
        instr |= st->memory[pcVal+i] << i*BITS_IN_BYTE;
    }
    return instr;
//  (st->memory[pcVal] | st->memory[pcVal+1] << BITS_IN_BYTE | st->memory[pcVal+2] << 2 * BITS_IN_BYTE | st->memory[pcVal+3] << 24);
}

int isTerminate(state *st) {
    return (getInstruction(st)) == 0;
}

/* selects the instruction based on bits 27 to 25 and bits 7 to 4 in the case of multiply */
void  selectInstruction(state *st) {
    uint32_t instr = getInstruction(st);
    if (indexTobit(isBranch_bit, instr)) {
        printf("branch\n");
        decode_branch(st);
        branch(st);
    }
    else if (indexTobit(isSDT_bit, instr)) {
        printf("sdt\n");
        decode_single_data_transfer(st);
        singleDataTransfer(st);
    }
    else if (!indexTobit(isMult_bit, instr) && bitsToNum(instr,4,7)) {
        printf("multiply\n");
        decode_multiply(st);
        multiply(st);
    }
    else {
        printf("data process\n");
        decode_process_data(st);
        dataProcessing(st);
    }
}

int checkCond(state *st) {
    uint32_t instruction = getInstruction(st);
//    instruction = reverse(instruction);
    switch (bitsToNum(instruction,28,31)) {
        case EQ :
            return st->cpsrFlag->zbit;
        case NE :
            return !st->cpsrFlag->zbit;
        case GE :
            return st->cpsrFlag->nbit == st->cpsrFlag->vbit;
        case LT :
            return st->cpsrFlag->nbit != st->cpsrFlag->vbit;
        case GT :
            return !st->cpsrFlag->zbit & (st->cpsrFlag->nbit == st->cpsrFlag->vbit);
        case LE :
            return st->cpsrFlag->zbit | (st->cpsrFlag->nbit != st->cpsrFlag->vbit);
        case AL :
            return 1;
        default:
            return 0;
    }
}