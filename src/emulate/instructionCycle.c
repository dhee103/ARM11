#include "instructionCycle.h"
#include "getCodes.h"
#include "utilities.h"

void decode_process_data(state *st) {
    uint32_t instr = getInstruction(st);
    st->decoded->opcode = bitsToNum(instr, 21, 24);
    st->decoded->isImm = indexTobit(instr, 25);
    st->decoded->isSet = indexTobit(instr, 20);
    st->decoded->operand2 = bitsToNum(instr, 0, 11);
    st->decoded->rn = bitsToNum(instr, 16, 19);
    st->decoded->rd = bitsToNum(instr, 12, 15);

}

void decode_multiply(state *st) {
    uint32_t instr = getInstruction(st);
    st->decoded->rd = bitsToNum(instr, 16, 19);
    st->decoded->rn = bitsToNum(instr, 12, 15);
    st->decoded->rs = bitsToNum(instr, 8, 11);
    st->decoded->rm = bitsToNum(instr, 0, 3);
    st->decoded->isAcc = indexTobit(instr, 21);
    st->decoded->isSet = indexTobit(instr, 20);

}

void decode_single_data_transfer(state *st) {
    uint32_t instr = getInstruction(st);
    st->decoded->offset = bitsToNum(instr, 0, 11);
    st->decoded->rd = bitsToNum(instr, 16, 19);
    st->decoded->rn = bitsToNum(instr, 12, 15);
    st->decoded->isLoad = indexTobit(instr, 20);
    st->decoded->isUp = indexTobit(instr, 23);
    st->decoded->isPre = indexTobit(instr, 24);
    st->decoded->isImm = indexTobit(instr, 25);
}

void decode_branch(state *st) {
    uint32_t instr = getInstruction(st);
    st->decoded->offset = bitsToNum(instr, 0, 23);
}
