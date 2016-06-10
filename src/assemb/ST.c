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

void add_Symbol(uint32_t address, char *label, ST *symbolTable) {

  SYM *sym = malloc(sizeof(SYM));
  sym -> label = label;
  sym -> address = address;
  sym -> next = symbolTable -> last;
  symbolTable -> last = sym;

}

void freeST(ST *symbolTable) {
 
  SYM *next;
  SYM *curr = symbolTable -> last;  

  while (curr != NULL) {
    next = curr -> next;
    free(curr);
    curr = next;
  }

  free(symbolTable);

}


uint32_t getAddress(ST *symbolTable, char *label) {

  SYM *sym = symbolTable -> last;

  while (sym != NULL) {
    if (strcmp(sym -> label, label) == 0) {
      return sym -> address;
    }
    sym = sym -> next;
  }

  perror("label not found.");
  exit(EXIT_FAILURE);

}
