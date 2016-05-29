#include "instructions.h"
#include "getCodes.h"
#include "shifter.h"
#include "setRegs.h"

#include <stdio.h>


void dataProcessing(state *str) {
    uint32_t result;
    uint32_t second_operand;
    decoded_instr *str_decoded;
    int carry_out = 0;
    str_decoded = str -> decoded;
    int pass_check = checkCPRS(str_decoded -> condition, str -> cpsrFlag);
    /*for testing*/
    printf("check : %d\n", pass_check);

    if(1 == pass_check) {
        if(1 == str_decoded -> isImm) {
            uint32_t imm = (str_decoded -> operand2) & check_bit0_7;
            int rotV = (int)(((str_decoded -> operand2) & check_bit8_11) >> 8);
            second_operand = immValue(rotV,imm);
            /*for testing
            printBits(second_operand);*/
        } else {
            uint32_t rm = (int)(str_decoded -> operand2) & check_bit0_3;
            uint8_t shift = (uint8_t)(((str_decoded -> operand2) & check_bit4_11) >> 4);
            shift_out op2 = shifter(shift,rm);
            second_operand = op2.data;
            carry_out = op2.carry;
            /*for testing
            printBits(second_operand);*/
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
                /*printBits(result);*/
                regs[str_decoded -> rd] = result;
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
            /*for testing*/
            printf("C_out %d\n",(str->cpsrFlag)->cbit);
            printf("Z_out %d\n",(str->cpsrFlag)->zbit);

        }
        /*for testing*/
        printf("carry_out %d\n", carry_out);



        return;

    }
}

void multiply(state *st) {
    decoded_instr *decoded = st->decoded;
    uint32_t result;
//    rd = rm * rs
    result = (st->reg[decoded->rm] * st->reg[decoded->rs]);
//    check if accumulate
    if (st->decoded->isAcc) {
//    rd = rm * rs + rn
    result += (uint32_t)st->reg[decoded->rn];
    }
    st->reg[decoded->rd] = result;
//    TODO: set condition
    if (st->decoded->isSet) {
        if (result == 0) {
            st->cpsrFlag->zbit=1;
        }
        st->cpsrFlag->nbit=bitAccess(N_BIT,result);
    }

}

void singleDataTransfer(state *st) {
    if(st->decoded->isImm) {
//        offset interpreted as a shifted register
    }
//    offset is a unsigned 12 bit immediate offset
    if(st->decoded->isPre) {
//    offset +/- to base register BEFORE transferring data
        if (st->decoded->isUp) {
//    offset + to base register BEFORE transferring data
        }
//    offset - to base register BEFORE transferring data
    }
//    offset +/- to base register AFTER transferring data
    if (st->decoded->isUp) {
//    offset + to base register AFTER transferring data
    }
//    offset - to base register AFTER transferring data


//    pass this onto a helper function?
}

void sDThelper(state *st) {


}

void branch(state *st) {
    st->reg[PC] = (st->decoded->offset) << BRANCHSHIFT;
}


void setNZ(uint32_t check_value, state *toBeSet) {
    (toBeSet -> cpsrFlag)->nbit = 0x01;
    (toBeSet -> cpsrFlag)->zbit = 0x00;

    if(0 == (check_value & check_bit31)) {
        (toBeSet -> cpsrFlag)->nbit = 0x00;
    }

    if(0 == check_value ) {
        (toBeSet -> cpsrFlag)->zbit = 0x01;
    }

}

void setC(state *toBeSet,int carry_out) {
    (toBeSet -> cpsrFlag)->cbit = (uint8_t)carry_out;
}

void setCadd(state *toBeSet,uint32_t first_value,uint32_t second_value) {
    (toBeSet -> cpsrFlag)->cbit = 0x00;
    int first_bit31 = (int)((first_value & check_bit31)>>31);
    int second_bit31 = (int)((second_value & check_bit31)>>31);
    if((1 == first_bit31) && (1 == second_bit31)) {
        (toBeSet -> cpsrFlag)->cbit = 0x01;
    }
}

void setCsub(state *toBeSet,uint32_t first_value,uint32_t second_value) {
    (toBeSet -> cpsrFlag)->cbit = 0x00;
    int first_bit31 = (int)((first_value & check_bit31)>>31);
    int second_bit31 = (int)((second_value & check_bit31)>>31);
    if((0 == first_bit31) && (1 == second_bit31)) {
        (toBeSet -> cpsrFlag)->cbit = 0x01;
    }
}
