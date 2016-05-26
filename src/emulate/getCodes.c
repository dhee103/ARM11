#include <stdio.h>
#include <stdint.h>
#include "definitions.h"
#include "getCodes.h"

int getCond(uint32_t instruction) {
	instruction = (instruction & (condMask << condMask_shift)) >> condMask_shift;
        return (int)instruction;
}

int getOpcode(uint32_t instruction) {
	instruction = (instruction & (opcodeMask << opcodeMask_shift)) >> opcodeMask_shift;
	if ((instruction & 1) == 1) {
		instruction |= oneMask;
	}
	return (int)instruction;
}

int isMult(uint32_t instruction) {
//	uint32_t multMask = 0x9;
//	multMask <<= 28;
	return ((instruction & multMask) == multMask);
}

int getNFlag(uint32_t instruction) {
    instruction = (instruction & (NFlagMask << NFlagShift)) >> NFlagShift;
    printf("%u\n",instruction);
    return instruction;
}

int getZFlag(uint32_t instruction) {
    instruction = (instruction & (ZFlagMask << ZFlagShift)) >> ZFlagShift;
    printf("%u\n",instruction);
    return instruction;
}

void setNFlag(uint32_t instruction, int bit) {

    printf("%u\n",instruction);
}

int main(void) {
	uint32_t instruction = 0x0110A0E3;
	uint32_t inp2 = 0x022081E2;
	uint32_t inp3 = 0x910203E0;
    getCond(instruction);
	getOpcode(instruction);
	getCond(inp2);
	getOpcode(inp2);
	getCond(inp3);
	isMult(inp3);
    getNFlag(inp3);
    getZFlag(inp3);
    return 0;
}
                                                 
