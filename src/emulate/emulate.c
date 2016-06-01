#include "utilities.h"

#include <stdio.h>

int main(int argc, char **argv) {
//initialises a virtual machines state comprising a simulated memory, registers, and associated instruction data
    state *st = newState();
//populates the memory with instructions and data
    if (!loader(st,argc,argv)) {
        return EXIT_FAILURE;
    }

//    termination check is run on each instruction cycle by reading the next instruction
    while (!isTerminate(st)) {
//        printf("current instruction: ");
//        printf("%04x\n",getInstruction(st));
        if (checkCond(st)) {
            selectInstruction(st);
        }
//        increment pc
//        output(st);
        st->reg[PC] += BYTES_IN_WORD;
    }

//    output register state
//    print_result(st);
//    delete state
    st->reg[PC] += PC_AHEAD_BYTES;
    output(st);
    freeState(st);
    return EXIT_SUCCESS;
}