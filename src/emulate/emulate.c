#include "emulate.h"
#include "getCodes.h"
#include "utilities.h"

#include <stdio.h>

state *newState(void) {
    state *st = malloc(sizeof(state));
    if (st == NULL) {
        return NULL;
    }

    st->decoded = malloc(sizeof(decoded_instr));
    if (st->decoded == NULL) {
        free(st);
        return NULL;
    }

    st->reg = malloc(NUM_REGISTERS*sizeof(uint32_t));
    if (st->reg == NULL) {
        free(st->decoded);
        free(st);
        return NULL;
    }
    memset(st->reg,0,NUM_REGISTERS*sizeof(uint32_t));

    st->memory = malloc(MEM_SIZE*sizeof(uint8_t));
    if (st->memory == NULL) {
        free(st->decoded);
        free(st->reg);
        free(st);
        return NULL;
    }
    memset(st->memory,0,MEM_SIZE*sizeof(uint8_t));

    st->cpsrFlag = malloc(sizeof(cpsr));
    if (st->cpsrFlag == NULL) {
        free(st->cpsrFlag);
        free(st->decoded);
        free(st->reg);
        free(st);
    }
    memset(st->cpsrFlag,0,4);

    return st;
}

void freeState(state *st) {
    if (st != NULL) {
        if (st->decoded != NULL) {
            free(st->decoded);
        }

        if (st->reg != NULL) {
            free(st->reg);
        }

        if (st->memory != NULL) {
            free(st->memory);
        }

        free(st);
    }
}


int main(int argc, char **argv) {
//initialise and allocate memory for reg and mem
    state *st = newState();
//    loader(st) will load all the instructions into memory
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
