#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<stdint.h>
#include<assert.h>
#include "emulate/definitions.h"

uint32_t* fileloader(char *filename);
uint32_t reversed(uint32_t bits);
uint32_t indexTobit(int index, uint32_t bits);
void  process_data(STATE *state);
void  multiply(STATE *state);
void  single_data_transfer(STATE *state);
void  branch(STATE *state);
int run(int argc, char **args); 
void decode_process_data(STATE *state, uint32_t bits);
void decode_multiply(STATE *state, uint32_t bits);
void decode_single_data_transfer(STATE *state, uint32_t bits);
void decode_branch(STATE *state, uint32_t bits);
void print_bits(uint32_t bits);
uint32_t bitsToNum(uint32_t bits, int strIndex, int endIndex);
uint32_t getCond(uint32_t bits);
void print_result(STATE *state);
void  free_state(STATE *state);
uint32_t get_CPSR(STATE *state);
int checkcond(STATE *state, uint32_t cond);
uint32_t shifter_register(STATE *state, uint32_t offset);
uint32_t getFromMem(STATE *state, uint32_t address);
void addToMem(STATE *state, uint32_t address, uint32_t value);

int run(int argc, char **args) {

  if (argc == 1) {
    fprintf(stderr, "missing file name\n");
  }

  uint32_t *i; 
  int j;
  
  STATE *state =  malloc(sizeof(STATE));
  decoded_instr *decoded = malloc(sizeof(STATE));
  CPSR *cpsr = malloc(sizeof(CPSR));
  uint32_t reg[16] = {0};
  uint8_t memory[32768] = {0};
  state -> cpsr = cpsr; 
  state -> decoded = decoded;
  state -> memory = memory;
  state -> reg = reg;

  i = fileloader(args[1]); 

  for (j = 0; i[j] != 0; j++) {
    i[j] = reversed(i[j]);
    print_bits(i[j]); 
    printf("%d\n",bitsToNum(i[j], 16, 19));

    if (checkcond(state, getCond(i[j]))) {
      if (indexTobit(27, i[j]) == 1) {

        decode_branch(state, i[j]);
        branch(state);

      } else if (indexTobit(26, i[j]) == 1) {

        decode_single_data_transfer(state, i[j]);
        single_data_transfer(state);

      } else if (indexTobit(7, i[j]) == 1 
              && indexTobit(6, i[j]) == 0 
              && indexTobit(5, i[j]) == 0 
              && indexTobit(4, i[j]) == 1) {

        decode_multiply(state, i[j]);
        multiply(state);

      } else {

        decode_process_data(state, i[j]);
        process_data(state);

      } 
    }
  }  

  print_result(state);
  free_state(state);

  return 0;

}

uint32_t* fileloader(char *filename) {

  FILE *f;
  size_t s = 4;
  size_t num;  

  f = fopen(filename, "r");
  
  fseek(f, 0L, SEEK_END);
  num = ftell(f) / 4;
  rewind(f);

  static uint32_t *j;
  j = malloc(num);
  
  fread(j, s, num, f);
  fclose(f);  

  return j;

}

uint32_t reversed(uint32_t bits) {

  return ((bits << 24 ) & 0xff000000)
       | ((bits << 8) & 0xff0000)
       | ((bits >> 8) & 0xff00)
       | ((bits >> 24) & 0xff);

}

void print_bits(uint32_t bits) {

  uint32_t mask = 1;
  mask <<= 31;
  int i;

  for (i = 0; i < 32; i++) {
    printf("%d", (mask & bits) != 0);
    bits <<= 1;
  }

  printf("\n");

}

int checkcond(STATE *state, uint32_t cond) {

  int result = 0;

  switch (cond) {
    case EQ: result = (state -> cpsr -> Z == 1); break;
    case NE: result = (state -> cpsr -> Z == 0); break;
    case GE: result = (state -> cpsr -> N == state -> cpsr -> V); break;
    case LT: result = (state -> cpsr -> N != state -> cpsr -> V); break;
    case GT: result = (state -> cpsr -> Z == 0) && (state -> cpsr -> N == state -> cpsr -> V); break;
    case LE: result = (state -> cpsr -> Z == 1) || (state -> cpsr -> N != state -> cpsr -> V); break;
    case AL: result = 1; break;
  }

  return result;

}

uint32_t bitsToNum(uint32_t bits, int strIndex, int endIndex) {

  int i;
  uint32_t result = 0;
  
  for (i = strIndex; i <= endIndex; i++) {
    result = result + indexTobit(i, bits) * pow(2, i - strIndex);
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

  return bitsToNum(bits, 28, 31);

}

void print_result(STATE *state) {

  int i;
  FILE *fo;
  fo = fopen("result.out", "w");

  fprintf(fo,"registers:\n");

  for (i = 0; i < 13; i++) {
    fprintf(fo, "$%d  :\t%d (0x%x)\n", i, (state -> reg)[i], (state -> reg)[i]);
  }

  fprintf(fo, "CPSR:\t%d (0x%x)\n", get_CPSR(state), get_CPSR(state));
  fprintf(fo, "PC  :\t%d (0x%x)\n", state -> PC, state -> PC);

  fprintf(fo,"Non-zero memory:\n");

  for (i = 0; (state -> memory)[i] != 0; i++) {
    fprintf(fo, "0x%x:  0x%x\n", i * 4, (state -> memory)[i]);
  }

  fclose(fo);

}

uint32_t get_CPSR(STATE *state) {

  return (state -> cpsr -> N) * 8
       + (state -> cpsr -> Z) * 4
       + (state -> cpsr -> C) * 2
       + (state -> cpsr -> V); 

}

void  process_data(STATE *state) {



}

void  multiply(STATE *state) {



}

void  single_data_transfer(STATE *state) {

  uint32_t offset = state -> decoded -> offset;
  uint32_t rb = state -> decoded -> rn;
  uint32_t rd = state -> decoded -> rd;

  if (state -> decoded -> isImm) {
    offset = shifter_register(state, offset);
  }

  uint32_t memory_address;
  uint32_t base_offset;

  if (state -> decoded -> isUp) {

    base_offset = rb + offset;

  } else {

    base_offset = rb - offset;

  }

  if (state -> decoded -> isPre) {

    memory_address = base_offset;

  } else {

    memory_address = rb; 
    state -> decoded -> rn = rb;

  }

  if (state -> decoded -> isLoad) {

    addToMem(state, memory_address, (state -> reg) [rd]);

  } else {

    (state -> reg) [rd] = getFromMem(state, memory_address);

  }

}

void  branch(STATE *state) {

  state -> PC = (state -> decoded -> offset) << 2;

}

void decode_process_data(STATE *state, uint32_t bits) {

  state -> decoded -> opcode = bitsToNum(bits, 21, 24);
  state -> decoded -> isImm = indexTobit(bits, 25);
  state -> decoded -> isSet = indexTobit(bits, 20);
  state -> decoded -> operand2 = bitsToNum(bits, 0, 11); 
  state -> decoded -> rn = bitsToNum(bits, 16, 19);
  state -> decoded -> rd = bitsToNum(bits, 12, 15); 
 
}

void decode_multiply(STATE *state, uint32_t bits) {

  state -> decoded -> rd = bitsToNum(bits, 16, 19);
  state -> decoded -> rn = bitsToNum(bits, 12, 15);
  state -> decoded -> rs = bitsToNum(bits, 8, 11);
  state -> decoded -> rm = bitsToNum(bits, 0, 3);
  state -> decoded -> isAcc = indexTobit(bits, 21);
  state -> decoded -> isSet = indexTobit(bits, 20);

}

void decode_single_data_transfer(STATE *state, uint32_t bits) {

  state -> decoded -> offset = bitsToNum(bits, 0, 11);
  state -> decoded -> rd = bitsToNum(bits, 16, 19);
  state -> decoded -> rn = bitsToNum(bits, 12, 15);  
  state -> decoded -> isLoad = indexTobit(bits, 20);
  state -> decoded -> isUp = indexTobit(bits, 23);
  state -> decoded -> isPre = indexTobit(bits, 24);
  state -> decoded -> isImm = indexTobit(bits, 25);

}

void decode_branch(STATE *state, uint32_t bits) {

  state -> decoded -> offset = bitsToNum(bits,  0, 23);

}

void  free_state(STATE *state) {

  assert(state != NULL);

  free(state);

}

void addToMem(STATE *state, uint32_t address, uint32_t value) {

  int i;

  for (i = 0; i < 4; i++) {
    (state -> memory) [address + i] = bitsToNum(value, 0 + i * 8, 7 + i * 8);
  }  

}

uint32_t getFromMem(STATE *state, uint32_t address) {

  uint32_t result = 0;
  int i;

  for (i = 0; i < 4; i++) {
    result = result | (((uint32_t) (state -> memory) [address + i]) << (i * 8));
  }

  return result;

}

uint32_t shifter_register(STATE *state, uint32_t offset) {

  uint32_t rm = (state -> reg)[offset & 31];
  uint32_t type = (offset & 96) >> 5;
  uint32_t shifter = offset >> 7; 

  uint32_t result;

  switch (type) {

    case LSL: result = shifter >> rm; break;
    case LSR: result = shifter << rm;break;
    case ROR: result = shifter >> rm; 
              shifter = shifter >> (32 - rm);
              shifter = shifter << (32 - rm);
              result = result | shifter;
              break;
    case ASR: if (shifter >> 31 == 1) {
                result = (pow(2, rm) - 1);
                result = result << (32 - rm);
                result = result | (shifter >> rm);
              } else {
                result = shifter << rm;        
              } 
              break;

  }

  return result;

}
