#ifndef __UTILITIES_H
#define __UTILITIES_H


#include "definitions.h"

void loader(state *st, int argc, char **argv);
void print_result(state *st);
uint32_t bitsToNum(uint32_t bits, int strIndex, int endIndex);
void print_bits(uint32_t bits);
uint32_t indexTobit(int index, uint32_t bits);
uint32_t get_CPSR(state *st);
void output(state *st);

#endif
