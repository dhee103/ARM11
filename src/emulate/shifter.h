#ifndef __SHIFTER_H
#define __SHIFTER_H

#include "definitions.h"

shift_out lsl(int shift_value, uint32_t Rm_value);
shift_out lsr(int shift_value, uint32_t Rm_value);
shift_out asr(int shift_value, uint32_t Rm_value);
shift_out ror(int shift_value, uint32_t Rm_value);
shift_out shifter(uint8_t shift, int Rm, state *st);
uint32_t immValue(int rotate_value, uint32_t immV);

#endif
