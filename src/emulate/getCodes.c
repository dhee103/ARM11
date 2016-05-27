#include <stdio.h>
#include <stdint.h>
#include "definitions.h"
#include "getCodes.h"
#include "instructions.c"
#include "instructionCycle.c"

int getCond(uint32_t instruction) {
	instruction = (instruction & (condMask << condMask_shift)) >> condMask_shift;
	return (int)instruction;
}

int checkCond(state *st) {
    uint32_t instruction = getInstruction(st);
    switch(getCond(instruction)) {
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
            return EXIT_SUCCESS;
        default:
            return EXIT_FAILURE;
    }
}

int getOpcode(uint32_t instruction) {
	instruction = (instruction & (opcodeMask << opcodeMask_shift)) >> opcodeMask_shift;
	if ((instruction & 1) == 1) {
		instruction |= oneMask;
	}
	return (int)instruction;
}

int bitAccess(int index, uint32_t instruction) {

    if (0 <= index && index <= 7) {
        index = index + 24;
    } else if (8 <= index && index <= 15) {
        index = index + 8;
    } else if (16 <= index && index <= 23) {
        index = index - 8;
    } else if (24 <= index && index <= 31) {
        index = index - 24;
    }

    uint32_t mask = 1;
    mask = mask << index;

    return (mask & instruction) != 0;

}

int isMult(uint32_t instruction) {
//	uint32_t multMask = 0x9;
//	multMask <<= 28;
	return ((instruction & multMask) == multMask);
}

uint32_t getInstruction(state *st) {
//    make sure it's a multiple of 4 (valid alignment)
    uint32_t pcVal = st->reg[PC];
    uint32_t instr = 0;
    for (int i=0; i < 4; i++) {
        instr |= st->memory[pcVal+i] << i*BITS_IN_BYTE;
    }
    return instr;
//            (st->memory[pcVal] | st->memory[pcVal+1] << BITS_IN_BYTE | st->memory[pcVal+2] << 2 * BITS_IN_BYTE | st->memory[pcVal+3] << 24);
}


int isTerminate(state *st) {
    return (getInstruction(st)) == 0;
}



/* selects the instruction based on bits 27 to 25 and bits 7 to 4 in the case of multiply */
void selectInstruction(state *st) {
    uint32_t instr = getInstruction(st);
    if (bitAccess(isBranch_bit, instr)) {
        decode_process_data(st);
        dataProcessing(st);
    }
    else if(bitAccess(isSDT_bit,instr)) {
        decode_single_data_transfer(st);
        singleDataTransfer(st);
    }
    else if(!bitAccess(isMult_bit,instr) && isMult(instr)) {
        decode_multiply(st);
        multiply(st);
    }
    else {
        decode_branch(st);
        branch(st);
    }
}

//int main(void) {
//	uint32_t instruction = 0x0110A0E3;
//	uint32_t inp2 = 0x022081E2;
//	uint32_t inp3 = 0x910203E0;
//	getCond(instruction);
//	getOpcode(instruction);
//	getCond(inp2);
//	getOpcode(inp2);
//	getCond(inp3);
//	isMult(inp3);
//	return 0;
//}
