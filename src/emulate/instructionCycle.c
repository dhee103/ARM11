#include "instructionCycle.h"
#include "getCodes.h"
#include "utilities.h"
#include "shifter.h"

#include <stdio.h>

void decode_process_data(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
//    printf("instr: %04x\n",instr);
    decoded->opcode = (OPCODE) bitsToNum(instr, 21, 24);
//    printf("opcode: %i\n", st->decoded->opcode);
    decoded->isImm = bitsToNum(instr, 25, 25);
//    printf("Imm bit: %i\n", decoded->isImm);
    decoded->isSet = bitsToNum(instr,20,20);
//    printf("set bit: %i\n", decoded->isSet);
    decoded->operand2 = bitsToNum(instr, 0, 11);
//    printf("operand2: %i\n", decoded->operand2);
    decoded->rn = bitsToNum(instr, 16, 19);
    decoded->rd = bitsToNum(instr, 12, 15);
//    printf("rn: %i\n",decoded->rn);
//    printf("rd: %i\n\n",decoded->rd);
}

void decode_multiply(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    decoded->rd = bitsToNum(instr, 16, 19);
    decoded->rn = bitsToNum(instr, 12, 15);
    decoded->rs = bitsToNum(instr, 8, 11);
    decoded->rm = bitsToNum(instr, 0, 3);
    decoded->isAcc = indexTobit(instr, 21);
    decoded->isSet = indexTobit(instr, 20);
}

void decode_single_data_transfer(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    decoded->offset = bitsToNum(instr, 0, 11);
    decoded->rd = bitsToNum(instr, 16, 19);
    decoded->rn = bitsToNum(instr, 12, 15);
    decoded->isLoad = bitsToNum(instr, 20, 20);
    decoded->isUp = bitsToNum(instr, 23, 23);
    decoded->isPre = bitsToNum(instr, 24, 24);
    decoded->isImm = bitsToNum(instr, 25, 25);
    printf("imm: %i\n", decoded->isImm);
    printf("pre: %i\n", decoded->isPre);
    printf("up: %i\n", decoded->isUp);
    printf("load: %i\n", decoded->isLoad);
    printf("rn: %i\n", decoded->rn);
    printf("rd: %i\n", decoded->rd);
    printf("offset: %i\n", decoded->offset);

}

void decode_branch(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    int32_t offset = bitsToNum(instr, 0, 23);
    decoded->offset = asr(6,(lsl(8,offset).data)).data;
}
