#include"definitions.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>


uint32_t single_data_process(instruct *ins) {

  uint32_t condition = Cond;
  uint32_t offset = ins -> operand2;
  uint32_t rd = ins -> rd;
  uint32_t rn = ins -> rn;
  uint32_t p = (ins -> p) << P;
  uint32_t l = 1;

  if ((ins -> mnemonic)[0] == 's') {
    l = 0; 
  }
  
  return p | (1 << Bits26) | (rd << Bits12) | (condition << Bits28) | abs(offset) | (l << Bits20) | (rn << Bits16) 
         | (ins -> u ? U : 0) | ((ins -> imm == 0) << Bits25); 

}
