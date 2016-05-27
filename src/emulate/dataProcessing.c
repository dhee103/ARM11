
#include <stdio.h>
#include <stdint.h>
#include "definitions.h"
#include "getCodes.h"
#include "shifter.h"
#include "setRegs.h"
#include <stdlib.h>

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

void setNZ(uint32_t check_value, state *toBeSet) {
        (toBeSet -> cprs).n = 0x01;
        (toBeSet -> cprs).z = 0x00;

        if(0 == (check_value & check_bit31)) {
            (toBeSet -> cprs).n = 0x00;
        }

        if(0 == check_value ) {
            (toBeSet -> cprs).z = 0x01;
        }

}

void setC(state *toBeSet,int carry_out) {
        (toBeSet -> cprs).c = (uint8_t)carry_out;
}

void setCadd(state *toBeSet,uint32_t first_value,uint32_t second_value) {
    (toBeSet -> cprs).c = 0x00;
    int first_bit31 = (int)((first_value & check_bit31)>>31);
    int second_bit31 = (int)((second_value & check_bit31)>>31);
    if((1 == first_bit31) && (1 == second_bit31)) {
        (toBeSet -> cprs).c = 0x01;
    }
}

void setCsub(state *toBeSet,uint32_t first_value,uint32_t second_value) {
     (toBeSet -> cprs).c = 0x00;
    int first_bit31 = (int)((first_value & check_bit31)>>31);
    int second_bit31 = (int)((second_value & check_bit31)>>31);
    if((0 == first_bit31) && (1 == second_bit31)) {
        (toBeSet -> cprs).c = 0x01;
    }
}

void processing(state *str) {
    uint32_t result;
    uint32_t second_operand;
    decoded_instr *str_decoded;
    int carry_out = 0;
    str_decoded = str -> decoded;
    int pass_check = checkCPRS(str_decoded -> condition, str -> cprs);
    printf("check : %d\n", pass_check);

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
                printBits(second_operand);
            }


            switch(str_decoded -> opcode) {
                case AND :
                    result = regs[str_decoded -> rn] & second_operand;
                    regs[str_decoded -> rd] = result;
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
                    switch(str_decoded -> opcode) {
                case AND :
                case EOR :
                case ORR :
                case TEQ :
                case TST :
                case MOV :
                    setC(str, carry_out);
                    break;

                case SUB :
                case CMP :
                    setCsub(str,regs[str_decoded -> rn],second_operand);
                    break;
                case RSB :
                    setCsub(str,second_operand,regs[str_decoded -> rn]);
                    break;
                case ADD :
                    setCadd(str,regs[str_decoded -> rn],second_operand);
                    break;

            }
            setNZ(result,str);
                    printf("C_out %d\n",(str->cprs).c );
                    printf("Z_out %d\n",(str->cprs).z );

    }
    printf("carry_out %d\n", carry_out);



    return;

}
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


    regs[1] = 0x02050002;
    regs[0] = 0x0000000F;
    regs[3] = 0x00000001;
    test->decoded = deTest;
    processing(test);
    printf("C %d\n",(test->cprs).c );
    printBits(regs[(test->decoded)->rn]);
    printBits(regs[(test->decoded)->rd]);

}
