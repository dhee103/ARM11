#ifndef __INSTRUCTIONS_H
#define __INSTRUCTIONS_H

#include "definitions.h"

#include <stdint.h>

void dataProcessing(state *st);
void multiply(state *st);
void singleDataTransfer(state *st);
void branch(state *st);

void setNZ(state *st, uint32_t result);
void setC(state *st,int carry_out);
void setCadd(state *st,uint32_t first_value,uint32_t second_value);
void setCsub(state *st,uint32_t first_value,uint32_t second_value);

void addToMem(state *st, uint32_t address, uint32_t value);
uint32_t getFromMem(state *st, uint32_t address);
uint32_t shifter_register(state *st, uint32_t offset);
shift_out shift(state *st);

int isGpioAddress(uint32_t address);

#endif
