#ifndef __UTILITIES_H
#define __UTILITIES_H

#include "definitions.h"

int loader(state *st, int argc, char **argv);
int output(state *st);

state *newState(void);
void freeState(state *st);

uint32_t extract(uint32_t bits, int strIndex, int endIndex);
uint32_t getInstruction(state *st);
int isTerminate(state *st);
int checkCond(state *st);
void selectInstruction(state *st);


#endif
