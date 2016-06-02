#include "utilities.h"
#include "instructions.h"
#include "instructionCycle.h"

#include<stdio.h>

/*  Loads the binary bits in sets of 8 (as byte addressable) into the state */
int loader(state *st, int argc, char **argv) {
    if (argc == 1) {
        printf("Missing file name.\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("Could not open input file.\n");
        return 0;
    }
    uint8_t *buffer = malloc(sizeof(uint8_t));
    if (buffer == NULL) {
        return 0;
    }
    int i = 0;
    while (fread(buffer, sizeof(uint8_t), 1, fp) == 1) {
        st->memory[i] = *buffer;
        i++;
    }
    free(buffer);

    fclose(fp);
    return 1;
}

/*  helper function to output which outputs the CPSR */
int outputCPRS(state *st) {
    int cpsr = 0;
    cpsr |= st->cpsrFlag->nbit << N_BIT | st->cpsrFlag->zbit << Z_BIT
            | st->cpsrFlag->cbit << C_BIT | st->cpsrFlag->vbit << V_BIT;
    return cpsr;
}

/*  outputs to stdout the state of the registers and memory */
int output(state *st) {
    printf("Registers:\n");
    for (int i = 0; i < 13; i++) {
        printf("$%-3d", i);
        printf(": %10d (0x%08x)\n", st->reg[i], st->reg[i]);
    }
    printf("PC  ");
    printf(": %10d (0x%08x)\n", st->reg[PC], st->reg[PC]);
    printf("CPSR");
    int cpsr = outputCPRS(st);
    printf(": %10d (0x%08x)\n", cpsr, cpsr);

    printf("Non-zero memory:\n");
    for (int i = 0; i < MEM_SIZE; i += BYTES_IN_WORD) {
        if (((uint32_t *) st->memory)[i / BYTES_IN_WORD] != 0) {
            printf("0x%08x: 0x", i);
            for (int j = 0; j < BYTES_IN_WORD; j++) {
                printf("%02x", st->memory[i + j]);
            }
            printf("\n");
        }
    }
    return 1;
}

/*  creates a new state, allocates the necessary memory and initialises every
 *  value to 0 */
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

/*  frees up the memory which was previously allocated for the state */
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

/*  extracts bits in half open interval [begin,end)
 *  Consequently when extracting a single bit the inputs will be that
 *  (bits, bit_index, bit_index + 1) */
uint32_t extract(uint32_t value, int begin, int end)
{
    uint32_t mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask;
}

/* reads four adjacent memory locations to get the instruction and returns it
 * in big endian form */
uint32_t getInstruction(state *st) {
    uint32_t pcVal = st->reg[PC];
    uint32_t instr = 0;
    for (int i=0; i < BYTES_IN_WORD; i++) {
        instr |= st->memory[pcVal+i] << i*BITS_IN_BYTE;
    }
    return instr;
}

/* checks whether the instruction is the all 0 instruction which signifies
 * termination */
int isTerminate(state *st) {
    return (getInstruction(st)) == 0;
}

/* selects the instruction based on bits 27 to 25 and bits 7 to 4 in the
 * case of multiply */
void  selectInstruction(state *st) {
    uint32_t instr = getInstruction(st);
    if (extract(instr,BRANCH_BIT,BRANCH_BIT+1)) {
        decode_branch(st);
        branch(st);
    }
    else if (extract(instr,SDT_BIT,SDT_BIT+1)) {
        decode_single_data_transfer(st);
        singleDataTransfer(st);
    }
    else if (!extract(instr, I_BIT, I_BIT+1) && extract(instr,MULT_BIT_START,MULT_BIT_END)==9) {
        decode_multiply(st);
        multiply(st);
    }
    else {
        decode_data_process(st);
        dataProcessing(st);
    }
}

/* checks whether the condition holds for the current instruction */
int checkCond(state *st) {
    uint32_t instruction = getInstruction(st);
    switch (extract(instruction,COND_START,COND_END)) {
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