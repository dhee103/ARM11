#include "instructions.h"
#include "utilities.h"
#include "shifter.h"

#include <stdio.h>
#include <assert.h>

void dataProcessing(state *st) {
    decoded_instr *decoded = st->decoded;
    uint32_t result;
    uint32_t second_operand;
    int carry_out = 0;


    if (decoded->isImm) {
        uint32_t imm = (decoded->operand2) & check_bit0_7;
        int rotV = (int) (((decoded->operand2) & check_bit8_11) >> 8);
        second_operand = immValue(rotV, imm);
//            printf("Imm second op out: %i\n", second_operand);
        /*for testing*/
//            printBits(second_operand);
    } else {
        uint32_t rm = (decoded->operand2) & check_bit0_3;
//            printf("rm: %i\n", rm);
        uint8_t shift = (uint8_t) (((decoded->operand2) & check_bit4_11) >> 4);
//            printf("shift: %i\n", shift);
        shift_out op2 = shifter(shift, (int) rm, st);
//            printf("op2: %i\n", op2.data);
        second_operand = op2.data;
        carry_out = op2.carry;
        /*for testing*/
//            printf("reg second op out: %i\n", second_operand);
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
//                printf("got to add\n");
            st->reg[decoded->rd] = st->reg[decoded->rn] + second_operand;
//                printf("second op : %u\n", second_operand);
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
            /*PC = operand2*/
//                printf("got to move\n");
//                printBits(result);
//            printf("2nd op: %u\n",second_operand);
            st->reg[decoded->rd] = second_operand;
            break;
    }
//        printf("got out of first switch\n");
    if (decoded->isSet) {
        switch (decoded->opcode) {
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
//                printf("%u\n", st->reg[decoded->rn]);
//                printf("%u\n", second_operand);
                setCsub(st, second_operand, st->reg[decoded->rn]);
                break;
            case RSB :
//                    printf("third case\n");
                setCsub(st, st->reg[decoded->rn], second_operand);
                break;
            case ADD :
//                    printf("fourth case\n");
//                printf("%u\n", decoded->rn);
                setCadd(st, st->reg[decoded->rn], second_operand);
//                    printf("end of fourth case\n");
                break;

        }
//            printf("got out of second switch\n");
        setNZ(st,result);
        /*for testing*/
//            printf("C_out %d\n",(st->cpsrFlag)->cbit);
//            printf("Z_out %d\n",(st->cpsrFlag)->zbit);

    }
//        printf("finished loop\n");
    /*for testing*/
//        printf("carry_out %d\n", carry_out);

    return;
}


//void dataProcessing(state *st) {
//    decoded_instr *decoded = st->decoded;
//
//    assert(decoded->rd != PC && decoded->rn != PC && decoded->rs != PC && decoded->rm != PC);
//
//    uint32_t operand2;
//    int carry = 0;
//    if (decoded->isImm) {
//        operand2 = decoded->immValue;
//        shift_(data, ROR, shiftValue);
//    } else {
//        shift_o output = shiftReg(st);
//        operand2 = output.data;
//        carry = output.carry;
//    }
//
//    //switches on the opcode and then carries out the operation
//    uint32_t result = 0;
//    switch (decoded->opcode) {
//        case AND:
//        case TST:
//            result = st->reg[decoded->rn] & operand2;
//            break;
//        case EOR:
//        case TEQ:
//            result = st->reg[decoded->rn] ^ operand2;
//            break;
//        case SUB:
//        case CMP:
//            result = (int32_t) st->reg[decoded->rn] - (int32_t) operand2;
//            carry = st->reg[decoded->rn] >= operand2;
//            break;
//        case RSB:
//            result = (int32_t) operand2 - (int32_t) st->reg[decoded->rn];
//            carry = operand2 >= st->reg[decoded->rn];
//            break;
//        case ADD:
//            result = (int32_t) st->reg[decoded->rn]
//                     + (int32_t) operand2;
//            carry = st->reg[decoded->rn] > UINT32_MAX - operand2;
//            break;
//        case ORR:
//            result = st->reg[decoded->rn] | operand2;
//            break;
//        case MOV:
//            result = operand2;
//            break;
//    }
//
//    //switches on the opcode once again
//    //determines when to writes the result to rd
//    switch (decoded->opcode) {
//        case TST:
//        case TEQ:
//        case CMP:
//            break;
//        default:
//            st->reg[decoded->rd] = result;
//            break;
//    }
//
//    //sets the flags if needed
//    if (decoded->isSet) {
//        setFlag(st, carry, C_BIT);
//        if (result == 0) {
//            setFlag(st, 1, Z_BIT);
//        } else {
//            setFlag(st, 0, Z_BIT);
//        }
//        setFlag(st, maskBits(result, TOP_BIT, TOP_BIT), N_BIT);
//    }
//}



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
    setNZ(st,result);

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

//void singleDataTransfer(state *st) {
//    decoded_instr *decoded = st->decoded;
//    uint32_t offset = decoded->offset;
//    uint32_t base_reg = decoded->rn;
//    uint32_t src_reg = decoded->rd;
//
//    if (decoded->isImm) {
//        offset = shifter_register(st, offset);
//    }
//
//    uint32_t base_offset = 0 ;
//
//    if(decoded->isPre) {
//        if(decoded->isUp) {
//            base_offset = base_reg + offset;
//        }
//        else {
//            base_offset = base_reg - offset;
//        }
//    }
//
//    uint32_t memory_address = st->reg[base_offset];
//    int isGPIO = isGpioAddress(memory_address);
//    if (memory_address > MEM_SIZE && !isGPIO) {
//        printf("Error: Out of bounds memory access at address 0x%08x\n",memory_address);
//        return;
//    }
//    if (!isGPIO) {
//        if (decoded->isLoad) {
//            st->reg[src_reg] = *((uint32_t *) &st->memory[memory_address]);
////                getFromMem(st, memory_address);
//        }
//        else {
//            addToMem(st, memory_address, (st->reg)[src_reg]);
//        }
//    }
//    else if (decoded->isLoad) {
//        st->reg[src_reg] = memory_address;
//    }
//
//    if (!decoded->isPre) {
//        if(decoded->isUp) {
//            decoded->rn += offset;
//        }
//        else {
//            decoded->rn -= offset;
//        }
//    }
//}

void singleDataTransfer(state *st) {
    decoded_instr *decoded = st->decoded;

    assert(decoded->rm != PC && decoded->rd != PC);
    assert(decoded->isPre || decoded->rm != decoded->rn);

    int32_t offset;
    if (decoded->isImm) {
        uint32_t instr = getInstruction(st);
        uint32_t rm = extract(instr,0,4);
        uint32_t shiftType = extract(instr,5,7);
        if (!extract(instr,4,5)) {
            uint32_t shiftAmount = extract(instr,7,12);
            int signBit = extract(instr,MS_BIT,MS_BIT+1);
            uint32_t signMask = signBit << 31;
            uint32_t rightBits;
            uint32_t leftBits;
            switch (shiftType) {
                case LSL:
                    offset = st->reg[rm] << shiftAmount;
                    break;
                case LSR:
                    offset = st->reg[rm] >> shiftAmount;

                    break;
                case ASR:
                    offset = st->reg[rm] >> shiftAmount;
                    if (signBit) {
//                        generate mask with shiftAmount number of 1's at MSB
                        uint32_t mask = 0xFFFFFFFF << (32-shiftAmount);
                        offset |= mask;
                    }
                    break;
                case ROR:
                    rightBits = st->reg[rm] >> shiftAmount;
                    leftBits = st->reg[rm] << (32 - shiftAmount);
                    offset = leftBits | rightBits;
                    break;
            }
        }


    } else {
        offset = extract(getInstruction(st),SDT_OFFSET_START,SDT_OFFSET_END);
//        printf("offset: %u\n",offset);
    }

    if (!decoded->isUp) {
        offset = -offset;
    }

    uint32_t address = st->reg[decoded->rn];
    if (decoded->isPre) {
        address += offset;
//        printf("address: %u\n",address);
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
//                    *((uint32_t *) &st->memory[address]);
        } else {
//            *((uint32_t *) &st->memory[address]) = st->reg[decoded->rd];
            addToMem(st, address, (st->reg)[decoded->rd]);
        }
    } else if (decoded->isLoad) {
        st->reg[decoded->rd] = address;
    }

    if (!decoded->isPre) {
//        address += offset;
        if (decoded->rn == PC) {
            offset += PC_AHEAD_BYTES;
        }
        st->reg[decoded->rn] += offset;
    }
}

int isGpioAddress(uint32_t address) {
    switch (address) {
        case 0x20200028:
            printf("PIN OFF\n");
            break;
        case 0x2020001C:
            printf("PIN ON\n");
            break;
        case 0x20200000:
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            break;
        case 0x20200004:
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            break;
        case 0x20200008:
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

uint32_t shifter_register(state *st, uint32_t offset) {
    uint32_t rm = st->reg[extract(offset,0,4)];
    uint32_t type = extract(offset,5,7);
//            (offset & 96) >> 5;
    uint32_t val = extract(offset,7,12);
//            offset >> 7;

    uint32_t result;
    switch (type) {
        case LSL:
            result = val << rm;
            break;
        case LSR:
            result = val >> rm;
            break;
        case ROR:
            result = val >> rm;
            val = val << (32 - rm);
            result |= val;
            break;
        case ASR:
            if (extract(val,MS_BIT,MS_BIT+1)) {
                result = (uint32_t) ((1 << rm) - 1);
                result = result << (32 - rm);
                result = result | (val >> rm);
            }
            else {
                result = val >> rm;
            }
            break;
    }
    return result;
}

void setNZ(state *st, uint32_t result) {
    st->cpsrFlag->nbit = extract(result,N_BIT,N_BIT+1);
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