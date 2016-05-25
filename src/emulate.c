#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<stdint.h>
#include<emulate/definitions.h>

uint32_t CPSR;
uint32_t PC;

uint32_t* fileloader(char *filename);
uint32_t strToBit(char *c);
uint32_t indexTobit(int index, uint32_t bits);
void  process_data(uint32_t bits);
void  multiply(uint32_t bits);
void  single_data_transfer(uint32_t bits);
void  branch(uint32_t bits);

int main(int argc, char **args) {

  if (argc == 1) {
    fprintf(stderr, "missing file name\n");
  }

  uint32_t *i; 
  int j;
  
  i = fileloader(args[1]); 

  for (j = 0; i[j] != 0; j++) {
    printf("number %d: %d %d\n", j, i[j], getCond(i[j]));
    if (CPSR == getCond(i[j])) {
      if (indexTobit(27, i[j]) == 1) {
        branch(i[j]);
      } else if (indexTobit(26, i[j]) == 1) {
        single_data_transfer(i[j]);
      } else if (indexTobit(7, i[j]) == 1 
              && indexTobit(6, i[j]) == 0 
              && indexTobit(5, i[j]) == 0 
              && indexTobit(4, i[j]) == 1) {
        multiply(i[j]);
      } else {
        process_data(i[j]);
      } 
    }
  }  

  return 0;

}

uint32_t* fileloader(char *filename) {

  FILE *f;
  size_t s = 32;
  size_t num;  

  f = fopen(filename, "r");
  
  fseek(f, 0L, SEEK_END);
  num = ftell(f) / 32;
  rewind(f);

  int i;
  static uint32_t *j;
  j = malloc(num);

  char c[32];
  
  for (i = 0; i < num; i++) {
    fread(c, s, 1, f);
    j[i] = strToBit(c);
  }
  
  return j;

}

uint32_t strToBit(char *c) {

  int i;
  uint32_t result;

  for (i = 0; i < 32; i++) {
    if (c[i] == '1') {
      result = result + pow(2, 31 - i);
    }
  }

  return result;

}

uint32_t indexTobit(int index, uint32_t bits) {

  if (0 <= index && index <= 7) {
    index = index + 24;
  } else if (8 <= index && index <= 15) {
    index = index + 8;
  } else if (16 <= index && index <= 23) {
    index = index - 8;
  } else if (24 <= index && index <= 31) {
    index = index - 24;
  }  

  uint32_t mask = 1;
  mask = mask << index;

  return (mask & bits) != 0;

}

uint32_t getCond(uint32_t bits) {

  return indexTobit(31, bits) * 8 + indexTobit(30, bits) * 4 +
  indexTobit(29, bits) * 2 + indexTobit(28, bits);

}

void  process_data(uint32_t bits) {



}

void  multiply(uint32_t bits) {



}

void  single_data_transfer(uint32_t bits) {



}

void  branch(uint32_t bits) {

  bits <<= 8;
  bits =>> 6;
  PC = bits;

}
