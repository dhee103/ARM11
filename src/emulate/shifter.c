
#include <stdio.h>
#include <stdint.h>
/*#include "definitions.h"*/

uint32_t lsl(int shift_value, uint32_t Rm_value) {
  if(0 == shift_value) {
    return Rm_value;
  } else {
    /*uint32_t check_carry = 1 << 31;*/
    if(shift_value > 1) {
      int num_of_shift = shift_value - 1;
      Rm_value = Rm_value << num_of_shift; 
    }
    /*shift_uot.carry = (int)((Rm_value & check_carry) >> 31);*/
    return Rm_value << 1;
  }
}

uint32_t lsr(int shift_value, uint32_t Rm_value) {
  if(0 == shift_value) {
    return Rm_value;
  } else {
    /*uint32_t check_carry = 1;*/
    if(shift_value > 1) {
      int num_of_shift = shift_value - 1;
      Rm_value = Rm_value >> num_of_shift; 
    }
    /*shift_uot.carry = (int)(Rm_value & check_carry);*/
    return Rm_value >> 1;
  }

}

uint32_t asr(int shift_value, uint32_t Rm_value) {
  uint32_t check_bit31 = 0x80000000;
  uint32_t bit31 = check_bit31 & Rm_value;
  uint32_t bit31_extended = bit31;
   uint32_t result;
  int counter = 0;
  while(counter < shift_value) {
    bit31 = bit31 >> 1 ;
    bit31_extended = bit31_extended | bit31;
    counter++;
  }
  
  result = lsr(shift_value, Rm_value);
  result = result | bit31_extended; 
  return result;
}

uint32_t ror(int shift_value, uint32_t Rm_value) {
  uint32_t check_bit0 = 0x00000001;
  uint32_t temp;
  uint32_t result = Rm_value;
  while(shift_value > 0) {
  temp = check_bit0 & temp;
  temp = temp << 31;
  result = result >> 1;
  result = temp | result;
  shift_value--;
  }
  return result;
}

uint32_t shifter(uint8_t shift, int Rm) {
  uint8_t check_bit4 = 0x01;
  uint8_t check_shift_type = 0x06;
  uint32_t Rm_value = 0x80000003; /*register[Rm];*/
  int types = (int)((check_shift_type & shift) >> 1);
  int shift_value;
  uint32_t result;
  if(0 == (shift & check_bit4)) {
    /*read the shift value from bit 7 to 11*/
    uint8_t check_value = 0xF8;
    shift_value = (int)((shift & check_value) >> 3);
  }
  /*else {
    //read the value from the Rs register
    uint8_t check_Rs = 11110000;
    int Rs = (int)((shift & check_Rs) >> 4);
    shift_value = register[Rs];
  }*/

  switch(types) {
    case 0 :
      result = lsl(shift_value, Rm_value);
      break;
    case 1 :
      result = lsr(shift_value, Rm_value);
      break;
    case 2 :
      result = asr(shift_value, Rm_value);
      break;
    case 3 : 
      result = ror(shift_value, Rm_value);
      break;           
  }
  return result;
}

void printBits(uint32_t x) {
  int i;
  uint32_t mask = 1 << 31;
  for(i=0; i<32; ++i) {
    if((x & mask) == 0){
      printf("0");
    }
    else {
      printf("1");
  }
    x = x << 1;
  }
    printf("\n");
  }
  
int main(void) {
    printBits(shifter(0x0E, 1)); 
    
    return 0;
  }


