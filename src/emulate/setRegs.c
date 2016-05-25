
#include <stdio.h>
#include <stdint.h>
#include "definitions.h"

uint32_t regs[NUM_REGISTERS];

void setRegs(void) {
    int i = 0;
    while(i < NUM_REGISTERS) {
        regs[i] = 0x00000000;
    }
    return;
}
