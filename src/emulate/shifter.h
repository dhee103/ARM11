#include<stdint.h>

shift_out lsl(int shift_value, uint32_t Rm_value);
shift_out lsr(int shift_value, uint32_t Rm_value);
shift_out asr(int shift_value, uint32_t Rm_value);
shift_out ror(int shift_value, uint32_t Rm_value);
shift_out shifter(uint8_t shift, int Rm);
/*shift_out immValue(int rotate_value, uint8_t immV);*/
