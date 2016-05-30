#ifndef __UTILITIES_H
#define __UTILITIES_H


#include "definitions.h"

int loader(state *st, int argc, char **argv);
void print_result(state *st);
uint32_t bitsToNum(uint32_t bits, int strIndex, int endIndex);
void print_bits(uint32_t bits);
uint32_t indexTobit(int index, uint32_t bits);
uint32_t get_CPSR(state *st);
int output(state *st);
void printRegister(state *st, int reg);
uint32_t reverse(uint32_t bits);
#endif
