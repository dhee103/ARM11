#ifndef ARM11_12_FILEUTILS_H
#define ARM11_12_FILEUTILS_H

#include <stdint.h>
#include <stdio.h>

FILE * loadIn(int argc, char **argv);

typedef struct sourceData{ //idea for source data sctruct, allowing
// for access of files, and useful metadata
    char *buffer;
    FILE *file;         //contains the input assembly file, and populates the
    // numlines; during the first loop, populates values and labels
    uint32_t *values;
    uint32_t *labels;
    int numLines;
    int numInstructs;
    int numLabels;
} source;



source * loadSource(int argc, char **argv);

typedef struct instruction {    //using anans (first) instruction struct
    char *mnemonic;
    char *rd;
    char *rn;
    char *operand2;

} instruct;

#endif //ARM11_12_FILEUTILS_H
