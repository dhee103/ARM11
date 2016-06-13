#ifndef __RUN_H
#define __RUN_H

#include "ST.h"
//need to get rid of this double reference
#include "assembler_definitions.h"

char** twoD(int rows, int cols);
char* loader(int argc, char **args, int *no_lines);
void freeStrArr(char* arr);
uint32_t shiftType(char *type);
void handle_operand(char* operand2, instruct *ins);
uint32_t handleReg(char *token);
void handleAddress(instruct *ins, char *token);
void setInstruction(instruct *ins, char line[511], uint32_t *res,
                    uint32_t currAdd, ST *symboltable);
void printRes(char *file_name, uint32_t *res, uint32_t lastAdd);
void run(int argc, char **args);
uint32_t getAddress(ST *symbolTable, char *label);
void add_Symbol(uint32_t address, char *label, ST *symbolTable);

void freeInst(instruct *ins);
void freeData(char **data);




#endif
