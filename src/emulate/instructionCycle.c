#include "instructionCycle.h"
#include "utilities.h"

/*  The following functions are used to decode the different types of
 *  instructions
 */

void decode_data_process(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    decoded->opcode = (OPCODE) extract(instr, OPCODE_START, OPCODE_END);
    decoded->isImm = extract(instr, I_BIT, I_BIT+1);
    decoded->isSet = extract(instr,S_BIT,S_BIT+1);
    decoded->operand2 = extract(instr, OPERAND2_START, OPERAND2_END);
    decoded->rn = extract(instr, RN_START, RN_END);
    decoded->rd = extract(instr, RD_START, RD_END);
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
}

void decode_branch(state *st) {
    decoded_instr *decoded = st->decoded;
    memset(decoded, 0, sizeof(decoded_instr));
    uint32_t instr = getInstruction(st);
    decoded->offset = extract(instr, BR_OFFSET_START, BR_OFFSET_END);
}