#include "utilities.h"

#include<stdio.h>
#include<math.h>

uint32_t *fileloader(char *filename);

uint32_t reversed(uint32_t bits);

uint32_t indexTobit(int index, uint32_t bits);
//void  multiply(state *st);
//void  single_data_transfer(state *st);
//void  branch(state *st);
//int run(int argc, char **args);
//void decode_process_data(state *st, uint32_t bits);
//void decode_multiply(state *st, uint32_t bits);
//void decode_single_data_transfer(state *st, uint32_t bits);
//void decode_branch(state *st, uint32_t bits);
//void print_bits(uint32_t bits);
//uint32_t bitsToNum(uint32_t bits, int strIndex, int endIndex);

//void print_result(state *st);
//
//void free_state(state *st);
//
//uint32_t get_CPSR(state *st);


//int run(int argc, char **args) {
//
//    if (argc == 1) {
//        fprintf(stderr, "missing file name\n");
//    }
//
//    uint32_t *i;
//    int j;
//
//    state *st = malloc(sizeof(state));
//    decoded_instr *decoded = malloc(sizeof(state));
//    cpsr *cpsrFl = malloc(sizeof(cpsr));
//    uint32_t reg[12] = {0};
//    uint8_t memory[2] = {0};
//    st->cpsrFlag = cpsrFl;
//    st->decoded = decoded;
//    st->memory = memory;
//    st->reg = reg;
//
//    i = fileloader(args[1]);
//
////    for (j = 0; i[j] != 0; j++) {
////        i[j] = reversed(i[j]);
////        print_bits(i[j]);
////        printf("%d\n", bitsToNum(i[j], 16, 19));
////
////        if (checkCond(st)) {
////            if (indexTobit(27, i[j]) == 1) {
////
////                decode_branch(st);
////                branch(st);
////
////            } else if (indexTobit(26, i[j]) == 1) {
////
////                decode_single_data_transfer(st);
////                singleDataTransfer(st);
////
////            } else if (indexTobit(7, i[j]) == 1
////                       && indexTobit(6, i[j]) == 0
////                       && indexTobit(5, i[j]) == 0
////                       && indexTobit(4, i[j]) == 1) {
////
////                decode_multiply(st);
////                multiply(st);
////
////            } else {
////
////                decode_process_data(st);
////                dataProcessing(st);
////
////            }
////        }
////    }
////
////    print_result(st);
////    freeState(st);
////
////    return 0;
////
////}
//}
uint32_t *fileloader(char *filename) {

    FILE *f;
    size_t s = 4;
    size_t num;

    f = fopen(filename, "rb");

    fseek(f, 0L, SEEK_END);
    num = ftell(f) / 4;
    rewind(f);

    static uint32_t *j;
    j = malloc(num);

    fread(j, s, num, f);
    fclose(f);

    return j;

}

uint32_t reversed(uint32_t bits) {

    return ((bits << 24) & 0xff000000)
           | ((bits << 8) & 0xff0000)
           | ((bits >> 8) & 0xff00)
           | ((bits >> 24) & 0xff);

}

void print_bits(uint32_t bits) {

    uint32_t mask = 1;
    mask <<= 31;
    int i;

    for (i = 0; i < 32; i++) {
        printf("%d", (mask & bits) != 0);
        bits <<= 1;
    }

    printf("\n");

}


uint32_t bitsToNum(uint32_t bits, int strIndex, int endIndex) {

    int i;
    uint32_t result = 0;

    uint32_t mask = 1;
    for (i = strIndex; i <= endIndex; i++) {
        result = (uint32_t) (result + indexTobit(i, bits) * (mask << (i-strIndex)));
    }

    return result;

}

uint32_t indexTobit(int index, uint32_t bits) {

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

    return (mask & bits) != 0;

}

uint32_t get_CPSR(state *st) {
    uint32_t cpsr = (uint32_t) (st->cpsrFlag->nbit) << 3 | (st->cpsrFlag->zbit) << 2
                    | (st->cpsrFlag->cbit) << 1
                    | (st->cpsrFlag->vbit);
    return cpsr;

}

void print_result(state *st) {

    int i;
    FILE *fo;
    fo = fopen("result.out", "w");

    fprintf(fo, "Registers:\n");

    for (i = 0; i < 13; i++) {
        fprintf(fo, "$%d  :\t%d (0x%x)\n", i, st->reg[i], st->reg[i]);
    }

    fprintf(fo, "CPSR:\t%d (0x%x)\n", get_CPSR(st), get_CPSR(st));
    fprintf(fo, "PC  :\t%d (0x%x)\n", st->reg[PC], st->reg[PC]);

    fprintf(fo, "Non-zero memory:\n");

    for (i = 0; (st->memory)[i] != 0; i++) {
        fprintf(fo, "0x%x:  0x%x\n", i * 4, st->memory[i]);
    }

    fclose(fo);

}

void output(state *st) {
    printf("Registers:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printRegister(st, i);

    }

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

void printRegister(state *st, int reg) {
    if (reg == PC_REG) {
        printf("PC  ");
    } else if (reg == CPSR_REG) {
        printf("CPSR");
    } else {
        printf("$%-3d", reg);
    }
    printf(": %10d (0x%08x)\n", state->registers[reg], state->registers[reg]);
}

void loader(state *st, int argc, char **argv) {
    if (argc == 1) {
        printf("Missing file name.\n");
        return;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("Could not open input file.\n");
        return;
    }

    uint8_t *buffer = malloc(sizeof(uint8_t));
    if (buffer == NULL) {
        return;
    }
    int i = 0;
    while (fread(buffer, sizeof(uint8_t), 1, fp) == 1) {
        st->memory[i] = *buffer;
        i++;
    }
    free(buffer);

    fclose(fp);
    return;
}
