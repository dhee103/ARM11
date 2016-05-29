#ifndef __INSTRUCTIONS_H
#define __INSTRUCTIONS_H

#include "definitions.h"

#include <stdint.h>

void dataProcessing(state *st);
void multiply(state *st);
void singleDataTransfer(state *st);
void branch(state *st);

void setNZ(uint32_t check_value, state *toBeSet);
void setC(state *toBeSet,int carry_out);
void setCadd(state *toBeSet,uint32_t first_value,uint32_t second_value);
void setCsub(state *toBeSet,uint32_t first_value,uint32_t second_value);

#endif
