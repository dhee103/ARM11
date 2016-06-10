#include"definitions.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdint.h>


uint32_t getCond (char *cond) {

  uint32_t result = al;

  if (strncmp("eq", cond, 2) == 0) {
    result = eq;
  } else if (strncmp("ne", cond, 2) == 0) {
    result = ne;
  } else if (strncmp("ge", cond, 2) == 0) {
    result = ge;
  } else if (strncmp("lt", cond, 2) == 0) {
    result = lt;
  } else if (strncmp("gt", cond, 2) == 0) {
    result = gt;
  } else if (strncmp("le", cond, 2) == 0) {
    result = le;
  }

  return result;

}

uint32_t branch(instruct *ins, uint32_t currAddress) {

  int32_t offset = ins -> expression - currAddress - PIPELINE_EFFECT;
  char type[2];
  type[0] = (ins -> mnemonic)[1];
  type[1] = (ins -> mnemonic)[2]; 
  uint32_t condition = getCond(type);

  return (condition << Bits28) | (FIVE << Bits25) | ((offset >> 2) & 0x00ffffff);

}
