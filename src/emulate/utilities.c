#include "utilities.h"

#include<stdio.h>

uint32_t reverse(uint32_t bits);

uint32_t indexTobit(int index, uint32_t bits);

uint32_t reverse(uint32_t bits) {

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
    uint32_t result = 0;
    uint32_t mask = 1;
    for (int i = strIndex; i <= endIndex; i++) {
        result = (uint32_t) (result + indexTobit(i, bits) * (mask << (i-strIndex)));
    }
    return result;
}

uint32_t indexTobit(int index, uint32_t bits) {

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

int output(state *st) {
    printf("Registers:\n");
    for (int i = 0; i < 13; i++) {
        printf("$%-3d", i);
        printf(": %10d (0x%08x)\n", st->reg[i], st->reg[i]);
    }
    printf("PC  ");
    printf(": %10d (0x%08x)\n", st->reg[PC], st->reg[PC]);
    printf("CPSR");
    printf(": %10d (0x%08x)\n", st->reg[CPSR], st->reg[CPSR]);


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
