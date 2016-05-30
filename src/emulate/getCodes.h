#ifndef __GETCODES_H
#define __GETCODES_H

#include "definitions.h"

int checkCond(state *st);
int isMult(uint32_t input);
int bitAccess(int index, uint32_t instruction);
uint32_t getInstruction(state *st);
int isTerminate(state *st);
void selectInstruction(state *st);

#endif
