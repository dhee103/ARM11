#ifndef __ST_H
#define __ST_H

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

typedef struct symbol {

    uint32_t address;
    char *label;
    struct symbol *next;

} SYM;

typedef struct symbolTable {

    SYM *last;

} ST;


//void freeST(ST *symbolTable) {
//
//    SYM *next;
//    SYM *curr = symbolTable -> last;
//
//    while (curr != NULL) {
//        next = curr -> next;
//        free(curr);
//        curr = next;
//    }
//
//    free(symbolTable);
//
//}




#endif
