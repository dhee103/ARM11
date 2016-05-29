
#include <stdio.h>
#include <stdint.h>
#include "definitions.h"
#include "setRegs.h"

int checkCPRS (COND condition,cprsFlag *check) {
    int check_result = 0;
    switch(condition) {
        case EQ :
            if(1 == check->z) {
                check_result = 1;
            }
            break;
        case NE :
            if(0 == check->z) {
                check_result = 1;
            }
            break;
        case GE :
            if(check->n == check->v) {
                check_result = 1;
            }
            break;
        case LT :
            if(check->n != check->v) {
                check_result = 1;
            }
            break;
        case GT :
            if((check->n == check->v) && (0 == check->z)) {
                check_result = 1;
            }
            break;
        case LE :
            if((1 == check->z) || (check->n != check->v)) {
                check_result = 1;
            }
            break;
        default :
            check_result = 1;
            break;
    }
    return check_result;

    }



