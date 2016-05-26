
#include <stdio.h>
#include <stdint.h>
#include "definitions.h"
#include "getCodes.h"
#include "shifter.h"
#include "setRegs.h"
#include <stdlib.h>


void processing(state *str) {
    uint32_t result;
    uint32_t second_operand;
    decoded_instr *str_decoded;
    int carry_out = 0;
    str_decoded = str -> decoded;
    int pass_check = checkCPRS(str_decoded -> condition, str -> cprs);

    if(1 == pass_check) {
            if(1 == str_decoded -> isImm) {
                uint32_t imm = (str_decoded -> operand2) & check_bit0_7;
                int rotV = (int)(((str_decoded -> operand2) & check_bit8_11) >> 8);
                second_operand = immValue(rotV,imm);
            } else {
                uint32_t rm = (int)(str_decoded -> operand2) & check_bit0_3;
                uint8_t shift = (uint8_t)(((str_decoded -> operand2) & check_bit4_11) >> 4);
                shift_out op2 = shifter(shift,rm);
                second_operand = op2.data;
                carry_out = op2.carry;
            }

            switch(str_decoded -> opcode) {
                case AND :
                    result = regs[str_decoded -> rn] & second_operand;
                    regs[str_decoded -> rd] = result;
                    printf("%u\n", regs[str_decoded->rd]);
                    break;
                case EOR :
                    result = regs[str_decoded -> rn] ^ second_operand;
                    regs[str_decoded -> rd] = result;
                    break;
                case SUB :
                    result = regs[str_decoded -> rn] - second_operand;
                    regs[str_decoded -> rd] = result;
                    break;
                case RSB :
                    result = second_operand - regs[str_decoded -> rn];
                    regs[str_decoded -> rd] = result;
                    break;
                case ADD :
                    result = regs[str_decoded -> rn] + second_operand;
                    regs[str_decoded -> rd] = result;
                    break;
                case TST :
                    result = regs[str_decoded -> rn] & second_operand;
                    break;
                case TEQ :
                    result = regs[str_decoded -> rn] ^ second_operand;
                    break;
                case CMP :
                    result = regs[str_decoded -> rn] - second_operand;
                    break;
                case ORR :
                    result = regs[str_decoded -> rn] | second_operand;
                    regs[str_decoded -> rd] = result;
                    break;
                case MOV :
                    /*PC = operand2*/
                    result = second_operand;
                    str -> pc = result;
                    break;
            }

            if(1 == str_decoded -> isSet) {
                    setCPRS(result,str -> cprs,carry_out);

            }

    }


    return;

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

    decoded_instr *deTest = malloc(sizeof(decoded_instr));
    /*deTest = {AL, AND, 1, 1, 0, 0, 0, 0, 0x00000002, 0x00000001, 0x00000170};*/
    cprsFlag testCprs;
    /*testCprs = {0,1,0,0};*/
    state *test = malloc(sizeof(state));
    /*test = {&deTest, testCprs, 0x00000000};*/
    testCprs.n = 0;
    testCprs.z = 1;
    testCprs.c = 0;
    testCprs.v = 0;
    deTest->condition = EQ;
    deTest->opcode = AND;
    deTest->isImm = 0;
    deTest->isSet = 0;
    deTest->rn = 0x00000001;
    deTest->rd = 0x00000002;
    deTest->operand2 = 0x00000370;
    test->cprs = testCprs;


    regs[1] = 0x80000002;
    regs[0] = 0x00000008;
    regs[3] = 0x00000001;
    test->decoded = deTest;
    processing(test);
    printf("%u\n",test->decoded->rd);
    printBits((test->decoded)->rd);

}
