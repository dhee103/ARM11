#include "instructions.h"
#include "utilities.h"
#include "shifter.h"

#include <stdio.h>

void dataProcessing(state *st) {
    uint32_t result;
    uint32_t second_operand;
    int carry_out = 0;


    if (1 == st->decoded->isImm) {
        uint32_t imm = (st->decoded->operand2) & check_bit0_7;
        int rotV = (int) (((st->decoded->operand2) & check_bit8_11) >> 8);
        second_operand = immValue(rotV, imm);
//            printf("Imm second op out: %i\n", second_operand);
        /*for testing*/
//            printBits(second_operand);
    } else {
        uint32_t rm = (st->decoded->operand2) & check_bit0_3;
//            printf("rm: %i\n", rm);
        uint8_t shift = (uint8_t) (((st->decoded->operand2) & check_bit4_11) >> 4);
//            printf("shift: %i\n", shift);
        shift_out op2 = shifter(shift, (int) rm, st);
//            printf("op2: %i\n", op2.data);
        second_operand = op2.data;
        carry_out = op2.carry;
        /*for testing*/
//            printf("reg second op out: %i\n", second_operand);
    }

    switch (st->decoded->opcode) {
        case AND :
            result = st->reg[st->decoded->rn] & second_operand;
            st->reg[st->decoded->rd] = result;
            break;
        case EOR :
            result = st->reg[st->decoded->rn] ^ second_operand;
            st->reg[st->decoded->rd] = result;
            break;
        case SUB :
            result = st->reg[st->decoded->rn] - second_operand;
            st->reg[st->decoded->rd] = result;
            break;
        case RSB :
            result = second_operand - st->reg[st->decoded->rn];
            st->reg[st->decoded->rd] = result;
            break;
        case ADD :
//                printf("got to add\n");
            result = st->reg[st->decoded->rn] + second_operand;
//                printf("second op : %u\n", second_operand);
            st->reg[st->decoded->rd] = result;
            break;
        case TST :
            result = st->reg[st->decoded->rn] & second_operand;
            break;
        case TEQ :
            result = st->reg[st->decoded->rn] ^ second_operand;
            break;
        case CMP :
            result = st->reg[st->decoded->rn] - second_operand;
            break;
        case ORR :
            result = st->reg[st->decoded->rn] | second_operand;
            st->reg[st->decoded->rd] = result;
            break;
        case MOV :
            /*PC = operand2*/
//                printf("got to move\n");
            result = second_operand;
//                printBits(result);
            st->reg[st->decoded->rd] = result;
            break;
    }
//        printf("got out of first switch\n");
    if (1 == st->decoded->isSet) {
        switch (st->decoded->opcode) {
            case AND :
            case EOR :
            case ORR :
            case TEQ :
            case TST :
            case MOV :
//                    printf("first case\n");
                setC(st, carry_out);
                break;

            case SUB :
            case CMP :
//                    printf("second case\n");
//                printf("%u\n", st->reg[st->decoded->rn]);
//                printf("%u\n", second_operand);
                setCsub(st, second_operand, st->reg[st->decoded->rn]);
                break;
            case RSB :
//                    printf("third case\n");
                setCsub(st, st->reg[st->decoded->rn], second_operand);
                break;
            case ADD :
//                    printf("fourth case\n");
//                printf("%u\n", st->decoded->rn);
                setCadd(st, st->reg[st->decoded->rn], second_operand);
//                    printf("end of fourth case\n");
                break;

        }
//            printf("got out of second switch\n");
        setNZ(result, st);
        /*for testing*/
//            printf("C_out %d\n",(st->cpsrFlag)->cbit);
//            printf("Z_out %d\n",(st->cpsrFlag)->zbit);

    }
//        printf("finished loop\n");
    /*for testing*/
//        printf("carry_out %d\n", carry_out);

    return;
}


void multiply(state *st) {
    decoded_instr *decoded = st->decoded;
    uint32_t result;
//    rd = rm * rs
    result = (st->reg[decoded->rm] * st->reg[decoded->rs]);
//    check if accumulate
    if (st->decoded->isAcc) {
//    rd = rm * rs + rn
        result += (uint32_t) st->reg[decoded->rn];
    }
    st->reg[decoded->rd] = result;
    if (st->decoded->isSet) {
        if (result == 0) {
            st->cpsrFlag->zbit = 1;
        }
        st->cpsrFlag->nbit = extract(result, N_BIT, N_BIT+1);
    }
}

int32_t signExt(int32_t offset) {
    int32_t value = (OFFSET_BITS_MASK & offset);
    if (OFFSET_SIGN_MASK & offset) {
        value += NOT_OFFSET_BITS_MASK;
    }
    return value;
}

void branch(state *st) {
    int32_t offset = st->decoded->offset;
    offset = signExt(offset << BRANCH_SHIFT);
    st->reg[PC] += offset + PC_AHEAD_BYTES - BRANCH_AHEAD_BYTES;
}

void singleDataTransfer(state *st) {
    uint32_t offset = st->decoded->offset;
    uint32_t base_reg = st->decoded->rn;
    uint32_t src_reg = st->decoded->rd;

    if (st->decoded->isImm) {
        offset = shifter_register(st, offset);
    }

    uint32_t base_offset = 0 ;

//    if (st->decoded->isUp) {
//        base_offset = base_reg + offset;
//    }
//    else {
//        base_offset = base_reg - offset;
//    }->decoded-
//
//    if (st->decoded->isPre) {
//        memory_address = base_offset;
//    }
//    else {
//        memory_address = base_reg;
//        st->decoded->rn = base_reg;
//    }


    if(st->decoded->isPre) {
        if(st->decoded->isUp) {
            base_offset = base_reg + offset;
        }
        else {
            base_offset = base_reg - offset;
        }
    }

    uint32_t memory_address = st->reg[base_offset];

    if (st->decoded->isLoad) {
        st->reg[src_reg] = *((uint32_t *) &st->memory[memory_address]);
//                getFromMem(st, memory_address);
//
//
    }
    else {
        addToMem(st, memory_address, (st->reg)[src_reg]);
    }

    if (!st->decoded->isPre) {
        if(st->decoded->isUp) {
            st->decoded->rn += offset;
        }
        else {
            st->decoded->rn -= offset;
        }
    }
}

void addToMem(state *st, uint32_t address, uint32_t value) {
    for (int i = 0; i < 4; i++) {
        (st->memory)[address + i] = (uint8_t) extract(value, 0 + i * 8, 8 + i * 8);
    }
}

uint32_t getFromMem(state *st, uint32_t address) {
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        result = result | (((uint32_t) (st->memory)[address + i]) << (i * 8));
    }
    return result;
}

uint32_t shifter_register(state *st, uint32_t offset) {
    uint32_t rm = (st->reg)[offset & 31];
    uint32_t type = (offset & 96) >> 5;
    uint32_t shifter = offset >> 7;

    uint32_t result;
    switch (type) {
        case LSL:
            result = shifter >> rm;
            break;
        case LSR:
            result = shifter << rm;
            break;
        case ROR:
            result = shifter >> rm;
            shifter = shifter >> (32 - rm);
            shifter = shifter << (32 - rm);
            result = result | shifter;
            break;
        case ASR:
            if (shifter >> 31 == 1) {
//                result = (pow(2, rm) - 1);
                result = (uint32_t) ((1 << rm) - 1);
                result = result << (32 - rm);
                result = result | (shifter >> rm);
            }
            else {
                result = shifter << rm;
            }
            break;
    }
    return result;
}

void setNZ(uint32_t check_value, state *st) {
    st->cpsrFlag->nbit = 1;
    st->cpsrFlag->zbit = 0;

    if (0 == (check_value & check_bit31)) {
        st->cpsrFlag->nbit = 0;
    }

    if (0 == check_value) {
        st->cpsrFlag->zbit = 1;
    }
}

void setC(state *st, int carry_out) {
    st->cpsrFlag->cbit = (uint8_t) carry_out;
}

void setCadd(state *st, uint32_t first_value, uint32_t second_value) {
    st->cpsrFlag->cbit = 0;
    int first_bit31 = (int) ((first_value & check_bit31) >> 31);
    int second_bit31 = (int) ((second_value & check_bit31) >> 31);
    if ((1 == first_bit31) && (1 == second_bit31)) {
        st->cpsrFlag->cbit = 1;
    }
}

void setCsub(state *st, uint32_t first_value, uint32_t second_value) {
    st->cpsrFlag->cbit = 0;
    if (second_value > first_value) {
        st->cpsrFlag->cbit = 1;
    }
}