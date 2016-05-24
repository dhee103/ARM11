#include <stdio.h>
#include <stdint.h>
#include "definitions.h"
#include "getCodes.h"

int getCond(uint32_t input) {
	input = (input & (condMask << condMask_shift)) >> condMask_shift;
        return (int)input;
}

int getOpcode(uint32_t input) {
	input = (input & (opcodeMask << opcodeMask_shift)) >> opcodeMask_shift; 
	if ((input & 1) == 1) { 
		input |= oneMask;
	}
	return (int)input;
}

int isMult(uint32_t input) {
	uint32_t multMask = 0x9;
	multMask <<= 28;
	if ((input & multMask) == multMask) {
		return 1;
	} else { 
		return 0; 
	}
}

int main(void) {
        uint32_t input = 0x0110A0E3;
	uint32_t inp2 = 0x022081E2;
	uint32_t inp3 = 0x910203E0;
        getCond(input);
	getOpcode(input);
	getCond(inp2);
	getOpcode(inp2);
	getCond(inp3);
	isMult(inp3);
        return 0;
}
                                                 
