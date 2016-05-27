#include<stdint.h>
void processing(state *str);
void setNZ(uint32_t check_value, state *toBeSet);
void setC(state *toBeSet,int carry_out);
void setCadd(state *toBeSet,uint32_t first_value,uint32_t second_value);
void setCsub(state *toBeSet,uint32_t first_value,uint32_t second_value);
