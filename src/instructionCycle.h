#ifndef __INSTRUCTIONCYCLE_H
#define __INSTRUCTIONCYCLE_H

#include "definitions.h"

void decode_data_process(state *st);
void decode_multiply(state *st);
void decode_single_data_transfer(state *st);
void decode_branch(state *st);

#endif
