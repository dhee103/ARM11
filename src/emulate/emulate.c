#include "utilities.h"

#include <stdio.h>

int main(int argc, char **argv) {
//initialise and allocate memory for reg and mem
    state *st = newState();
//    loads all the instructions into memory
    if (!loader(st,argc,argv)) {
        return EXIT_FAILURE;
    }

//    fetch first instruction
//    (pc already initialised to 0)
//    decode first instruction
    while (!isTerminate(st)) {
//        printf("current instruction: ");
//        printf("%04x\n",getInstruction(st));
        if (checkCond(st)) {
            selectInstruction(st);
        }
//        increment pc
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
