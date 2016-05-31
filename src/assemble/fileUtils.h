#ifndef ARM11_12_FILEUTILS_H
#define ARM11_12_FILEUTILS_H

#include <stdint.h>
#include <stdio.h>

FILE * loadIn(int argc, char **argv);

typedef struct sourceData{
    char *buffer;
    FILE *file;         //contains the input assembly file, and populates the numlines; during the first loop, populate
    // values/labels
    uint32_t *values;
    uint32_t *labels;
    int numLines;
    int numInstructs;
} source;

source * loadSource(int argc, char **argv);

typedef struct instruction {
    char *mnemonic;
    char *rd;
    char *rn;
    char *operand2;

} instruct;

#endif //ARM11_12_FILEUTILS_H
