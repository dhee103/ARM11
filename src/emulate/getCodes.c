#include <stdio.h>
#include <stdint.h>
#include "definitions.h"

int getCond(uint32_t input) {
        input = (input & (condMask << condMask_shift)) >> condMask_shift;
        printf("%u\n",input);
        return (int)input;
}

int getOpcode(uint32_t input) {
	input = (input & (opcodeMask << opcodeMask_shift)) >> opcodeMask_shift;
	printf("%u\n",input);
	return (int)input;
}

int main(void) {
        uint32_t input = 0xE2345680;
	uint32_t alt = 0xEEEEEEEE;
        getCond(input);
	getOpcode(alt);
        return 0;
}
                                                 
