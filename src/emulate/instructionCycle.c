#include "instructionCycle.h"
#include "utilities.h"
#include "shifter.h"

#include <stdio.h>

void decode_process_data(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    decoded->opcode = (OPCODE) extract(instr, OPCODE_START, OPCODE_END);
    decoded->isImm = extract(instr, I_BIT, I_BIT+1);
    decoded->isSet = extract(instr,S_BIT,S_BIT+1);
    decoded->operand2 = extract(instr, OPERAND2_START, OPERAND2_END);
    decoded->rn = extract(instr, RN_START, RN_END);
    decoded->rd = extract(instr, RD_START, RD_END);
//    printf("instr: %04x\n",instr);
//    printf("opcode: %i\n", st->decoded->opcode);
//    printf("Imm bit: %i\n", decoded->isImm);
//    printf("set bit: %i\n", decoded->isSet);
//    printf("operand2: %i\n", decoded->operand2);
//    printf("rn: %i\n",decoded->rn);
//    printf("rd: %i\n\n",decoded->rd);
}

void decode_multiply(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    decoded->rd = extract(instr, M_RD_START, M_RD_END);
    decoded->rn = extract(instr, M_RN_START, M_RN_END);
    decoded->rs = extract(instr, RS_START, RS_END);
    decoded->rm = extract(instr, RM_START, RM_END);
    decoded->isAcc = extract(instr, A_BIT, A_BIT+1);
    decoded->isSet = extract(instr, S_BIT, S_BIT+1);
}

void decode_single_data_transfer(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    decoded->offset = extract(instr, SDT_OFFSET_START, SDT_OFFSET_END);
    decoded->rd = extract(instr, RD_START, RD_END);
    decoded->rn = extract(instr, RN_START, RN_END);
    decoded->isLoad = extract(instr, L_BIT, L_BIT+1);
    decoded->isUp = extract(instr, U_BIT, U_BIT+1);
    decoded->isPre = extract(instr, P_BIT, P_BIT+1);
    decoded->isImm = extract(instr, I_BIT, I_BIT+1);
//    printf("imm: %i\n", decoded->isImm);
//    printf("pre: %i\n", decoded->isPre);
//    printf("up: %i\n", decoded->isUp);
//    printf("load: %i\n", decoded->isLoad);
//    printf("rn: %i\n", decoded->rn);
//    printf("rd: %i\n", decoded->rd);
//    printf("offset: %i\n", decoded->offset);
}

//TODO: Sort this out
void decode_branch(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    int32_t offset = extract(instr, BR_OFFSET_START, BR_OFFSET_END);
    decoded->offset = asr(6,(lsl(8,offset).data)).data;
}