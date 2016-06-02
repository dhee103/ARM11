#include "instructions.h"
#include "utilities.h"

#include <stdio.h>
#include <assert.h>

/*  performs the multiply instruction as described by the internal comments*/
void dataProcessing(state *st) {
/*  Initialises the necessary variables */
    decoded_instr *decoded = st->decoded;
    int carry_out = 0;
    uint32_t result = 0;
    uint32_t second_operand;

/*  Calculates the correct operand2 value based on whether the instruction is
 * immediate */

    if (decoded->isImm) {
        uint32_t imm = (decoded->operand2) & MASK_IMM;
        int rotV = (int) (((decoded->operand2) & MASK_ROT) >> SHIFT_ROT);
        second_operand = immValue(rotV, imm);
    } else {
        shift_out op2 = shift(st);
        second_operand = op2.data;
        carry_out = op2.carry;
    }

/*  Performs operations to the registers based on the opcode */
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

/*  If the flags are supposed to be set, the appropriate helper functions are
 *  called to set the appropriate flags */
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


/*  performs the multiply instruction as described by the internal comments*/
void multiply(state *st) {
    decoded_instr *decoded = st->decoded;
    uint32_t result;
/*    result = rm * rs */
    result = (st->reg[decoded->rm] * st->reg[decoded->rs]);
/*    check if accumulate */
    if (st->decoded->isAcc) {
/*    result = rm * rs + rn */
        result += (uint32_t) st->reg[decoded->rn];
    }
/*  rd = result i.e. rd = rm * rs + rn */
    st->reg[decoded->rd] = result;
/*  sets N and Z bits of CPSR as necessary */
    setNZ(st, result);
}

/*  Performs the single data transfer instruction
 *  First calculates the offset based on whether the instruction uses an
 *  immediate offset which uses pre-indexing and has a set up bit
 *  If the base register happens to be the PC then the address is updated by
 *  8 bytes due to the side effect of the pipeline
 *  It then checks whether GPIO address, if not then the address is checked.
 *  If it's not within the size of the raspberry pi then an error message is
 *  displayed.
 *  The function then loads or stores as determined by the isLoad bit
 *  Finally in the case of post indexing the contents of the base register
 *  are changed by the offset after it has been indexed */
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
        offset += PC_AHEAD_BYTES;
    }

    if (decoded->isPre) {
        address += offset;
    }

    int isGpio = isGpioAddress(address);

    if (!isGpio) {
        if (address > MEM_SIZE) {
            printf("Error: Out of bounds memory access at address 0x%08x\n",
                   address);
            return;
        }
        if (decoded->isLoad) {
            st->reg[decoded->rd] = getFromMem(st, address);

        } else {
            addToMem(st, address, st->reg[decoded->rd]);
        }
    } else if (decoded->isLoad) {
        st->reg[decoded->rd] = address;
    }

    if (!decoded->isPre) {
        st->reg[decoded->rn] += offset;
    }
}

/* performs the branch instruction
 * Gets the offset from the decoded instruction, shifts and sign extends it
 * The offset is then added to the PC
 * Due to the side effects of the pipeline we added 8 bytes
 * Additionally due to the way we designed our pipeline we also had go back
 * 4 bytes to get to the correct instruction */
void branch(state *st) {
    int32_t offset = st->decoded->offset;
    offset = signExt(offset << BRANCH_SHIFT);
    st->reg[PC] += offset + PC_AHEAD_BYTES - BRANCH_AHEAD_BYTES;
}


/* helper function which sets N and Z flags when appropriate */
void setNZ(state *st, uint32_t result) {
    st->cpsrFlag->nbit = extract(result, N_BIT, N_BIT + 1);
    st->cpsrFlag->zbit = 0;
    if (!result) {
        st->cpsrFlag->zbit = 1;
    }
}

/* helper function which sets C flag when appropriate
 * called for AND, EOR, ORR, TEQ, TST and MOV */
void setC(state *st, int carry_out) {
    st->cpsrFlag->cbit = (uint8_t) carry_out;
}

/* helper function which sets C flag when appropriate
 * called for ADD only */
void setCadd(state *st, uint32_t first_value, uint32_t second_value) {
    st->cpsrFlag->cbit = 0;
    if (first_value + second_value > MAX_32_BIT_NUM) {
        st->cpsrFlag->cbit = 1;
    }
}

/* helper function which sets C flag when appropriate
 * called for SUB, CMP and RSB */
void setCsub(state *st, uint32_t first_value, uint32_t second_value) {
//    TODO: do by adding second operand as 2's complement
    st->cpsrFlag->cbit = 0;
    if (second_value >= first_value) {
        st->cpsrFlag->cbit = 1;
    }
}

/* helper function which returns the immediate value */
uint32_t immValue(int rotate_value, uint32_t immV) {
    uint32_t result;
    int num_rotate = 2 * rotate_value;
    result = ror(num_rotate, immV).data;
    return result;
}

/* helper function which performs a rotate right and returns a shift_out */
shift_out ror(int shift_value, uint32_t Rm_value) {
    shift_out result;
    uint32_t temp;
    result.data = Rm_value;
    while(shift_value > 0) {
        temp = MASK_BIT_0 & result.data;
        temp = temp << 31;
        result.data = result.data >> 1;
        result.data = temp | result.data;
        shift_value--;
    }
    result.carry = (int)((result.data & MASK_BIT_31) >> 31);
    return result;
}

/* shifter which given a state returns a shift_out which contains the shift
 * offset and the shift carry */
shift_out shift(state *st) {
    int carryBit = 0;
    uint32_t offset = 0;
    shift_out shiftOut;
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
            default:
                fprintf(stderr,"missed all shift type cases");
        }
        if (shiftAmount == 0) {
            carryBit = 0;
        }
        shiftOut.data = offset;
        shiftOut.carry = carryBit;
    }
    return shiftOut;
}

/* Gives the appropriate message depending on whether the given address is a
 * GPIO address */
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

/*  helper function which adds the given value at the given address */
void addToMem(state *st, uint32_t address, uint32_t value) {
    for (int i = 0; i < BYTES_IN_WORD; i++) {
        st->memory[address + i] = (uint8_t) extract(value, i * BITS_IN_BYTE,
                                                    (i+1) * BITS_IN_BYTE);
    }
}

/*  helper function which loads a word from the given address */
uint32_t getFromMem(state *st, uint32_t address) {
    uint32_t result = 0;
    for (int i = 0; i < BYTES_IN_WORD; i++) {
        result |= (((uint32_t) st->memory[address + i]) << (i * BITS_IN_BYTE));
    }
    return result;
}

/* helper function for branch that extends the signed offset to a signed 32
 * bit value */
int32_t signExt(int32_t offset) {
    int32_t value = (OFFSET_BITS_MASK & offset);
    if (OFFSET_SIGN_MASK & offset) {
        value += NOT_OFFSET_BITS_MASK;
    }
    return value;
}