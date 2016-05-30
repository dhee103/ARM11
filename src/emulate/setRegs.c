#include "setRegs.h"

int checkCPRS (COND condition,cpsr *check) {
    int check_result = 0;
    switch(condition) {
        case EQ :
            if(1 == check->zbit) {
                check_result = 1;
            }
            break;
        case NE :
            if(0 == check->zbit) {
                check_result = 1;
            }
            break;
        case GE :
            if(check->nbit == check->vbit) {
                check_result = 1;
            }
            break;
        case LT :
            if(check->nbit != check->vbit) {
                check_result = 1;
            }
            break;
        case GT :
            if((check->nbit == check->vbit) && (0 == check->zbit)) {
                check_result = 1;
            }
            break;
        case LE :
            if((1 == check->zbit) || (check->nbit != check->vbit)) {
                check_result = 1;
            }
            break;
        default :
            check_result = 1;
            break;
    }
    return check_result;

}
