#include "instructions.h"
#include "utilities.h"
#include "shifter.h"

#include <stdio.h>
#include <assert.h>

void dataProcessing(state *st) {
    decoded_instr *decoded = st->decoded;
    uint32_t result = 0;
    uint32_t second_operand;
    int carry_out = 0;


    if (decoded->isImm) {
        uint32_t imm = (decoded->operand2) & check_bit0_7;
        int rotV = (int) (((decoded->operand2) & check_bit8_11) >> 8);
        second_operand = immValue(rotV, imm);
    } else {
        shift_out op2 = shift(st);
        second_operand = op2.data;
        carry_out = op2.carry;
    }

    switch (decoded->opcode) {
        case AND :
            st->reg[decoded->rd] = st->reg[decoded->rn] & second_operand;
            break;
        case EOR :
            st->reg[decoded->rd] = st->reg[decoded->rn] ^ second_operand;
            break;
        case SUB :
            st->reg[decoded->rd] = st->reg[decoded->rn] - second_operand;
            break;
        case RSB :
            st->reg[decoded->rd] = second_operand - st->reg[decoded->rn];
            break;
        case ADD :
            st->reg[decoded->rd] = st->reg[decoded->rn] + second_operand;
            break;
        case TST :
            result = st->reg[decoded->rn] & second_operand;
            break;
        case TEQ :
            result = st->reg[decoded->rn] ^ second_operand;
            break;
        case CMP :
            result = st->reg[decoded->rn] - second_operand;
            break;
        case ORR :
            st->reg[decoded->rd] = st->reg[decoded->rn] | second_operand;
            break;
        case MOV :
            st->reg[decoded->rd] = second_operand;
            break;
    }
    if (decoded->isSet) {
        switch (decoded->opcode) {
            case AND :
            case EOR :
            case ORR :
            case TEQ :
            case TST :
            case MOV :
                setC(st, carry_out);
                break;
            case SUB :
            case CMP :
                setCsub(st, second_operand, st->reg[decoded->rn]);
                break;
            case RSB :
                setCsub(st, st->reg[decoded->rn], second_operand);
                break;
            case ADD :
                setCadd(st, st->reg[decoded->rn], second_operand);
                break;
        }
        setNZ(st, result);
    }
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
    setNZ(st, result);

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

shift_out shift(state *st) {
    shift_out shiftOut;
    int carryBit = 0;
    uint32_t offset = 0;
    shiftOut.data = offset;
    shiftOut.carry = carryBit;
    uint32_t instr = getInstruction(st);
    uint32_t shiftAmount = extract(instr, SHIFT_AMOUNT_START, SHIFT_AMOUNT_END);
    uint32_t rm = extract(instr, RM_START, RM_END);
    uint32_t shiftType = extract(instr, SHIFT_T_START, SHIFT_T_END);

    if (!extract(instr, OP_REG_BIT, OP_REG_BIT + 1)) {
        int signBit = extract(instr, MS_BIT, MS_BIT + 1);
        uint32_t rightBits;
        uint32_t leftBits;
        switch (shiftType) {
            case LSL:
                offset = st->reg[rm] << shiftAmount;
                carryBit = BITS_IN_WORD - shiftAmount;
                break;
            case LSR:
                offset = st->reg[rm] >> shiftAmount;
                carryBit = shiftAmount - 1;
                break;
            case ASR:
                offset = st->reg[rm] >> shiftAmount;
                if (signBit) {
                    uint32_t mask = MAX_MASK << (MAX_SHIFT - shiftAmount);
                    offset |= mask;
                }
                carryBit = shiftAmount - 1;
                break;
            case ROR:
                rightBits = st->reg[rm] >> shiftAmount;
                leftBits = st->reg[rm] << (BITS_IN_WORD - shiftAmount);
                offset = leftBits | rightBits;
                carryBit = shiftAmount - 1;
                break;
        }
        if (shiftAmount == 0) {
            carryBit = 0;
        }
        shiftOut.data = offset;
        shiftOut.carry = carryBit;
    }
    return shiftOut;

}

void singleDataTransfer(state *st) {
    decoded_instr *decoded = st->decoded;

    assert(decoded->rm != PC && decoded->rd != PC);
    assert(decoded->isPre || decoded->rm != decoded->rn);

    int32_t offset;
    if (decoded->isImm) {
        offset = shift(st).data;
    } else {
        offset = decoded->offset;
    }

    if (!decoded->isUp) {
        offset = -offset;
    }

    uint32_t address = st->reg[decoded->rn];

    if (PC == decoded->rn) {
        address += 8;
    }

    if (decoded->isPre) {
        address += offset;

    }


    int isGpio = isGpioAddress(address);
    if (address > MEM_SIZE && !isGpio) {
        printf("Error: Out of bounds memory access at address 0x%08x\n",
               address);
        return;
    }

    if (!isGpio) {
        if (decoded->isLoad) {
            st->reg[decoded->rd] = getFromMem(st, address);

        } else {
            addToMem(st, address, (st->reg)[decoded->rd]);
        }
    } else if (decoded->isLoad) {
        st->reg[decoded->rd] = address;
    }

    if (!decoded->isPre) {
        if (decoded->rn == PC) {
            offset += PC_AHEAD_BYTES;
        }
        st->reg[decoded->rn] += offset;
    }
}

int isGpioAddress(uint32_t address) {
    switch (address) {
        case PIN_OFF:
            printf("PIN OFF\n");
            break;
        case PIN_ON:
            printf("PIN ON\n");
            break;
        case PIN_0_9:
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            break;
        case PIN_10_19:
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            break;
        case PIN_20_29:
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            break;
        default:
            return 0;
    }
    return 1;
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

void setNZ(state *st, uint32_t result) {
    st->cpsrFlag->nbit = extract(result, N_BIT, N_BIT + 1);
    st->cpsrFlag->zbit = 0;
    if (0 == result) {
        st->cpsrFlag->zbit = 1;
    }
}

void setC(state *st, int carry_out) {
    st->cpsrFlag->cbit = (uint8_t) carry_out;
}

void setCadd(state *st, uint32_t first_value, uint32_t second_value) {
    st->cpsrFlag->cbit = 0;
    if (first_value + second_value > MAX_32_BIT_NUM) {
        st->cpsrFlag->cbit = 1;
    }
}

void setCsub(state *st, uint32_t first_value, uint32_t second_value) {
//    TODO: do by adding second operand as 2's complement
    st->cpsrFlag->cbit = 0;
    if (second_value >= first_value) {
        st->cpsrFlag->cbit = 1;
    }
}