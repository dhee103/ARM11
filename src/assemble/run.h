#ifndef __RUN_H
#define __RUN_H

#include "definitions.h"
#include "ST.c"

char** twoD(int rows, int cols);
char* loader(int argc, char **args, int *no_lines);
void freeStrArr(char* arr);
void handle_operand(char* operand2, instruct *ins);
uint32_t handleReg(char *token);
void handleAddress(instruct *ins, char *token);
void setInstruction(instruct *ins, char line[511], uint32_t *res,
                    uint32_t currAdd, ST *symboltable);
void printRes(char *file_name, uint32_t *res, uint32_t lastAdd);

#endif
