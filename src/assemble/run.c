#include "run.h"

#include "ST.c"
#include"single_data_process.c"
#include"branch.c"
#include"multiply.c"
#include"special.c"

char** twoD(int rows, int cols) {

  char** res = (char**) malloc(rows * sizeof(char*));
  assert(res != NULL);
  res[0] = (char*) malloc(rows * cols * sizeof(char));
  assert(res[0] != NULL);
  int i;
  for (i = 1; i < rows; i++) {
    res[i] = res[i - 1] + cols;
  }
  return res;
}

char* loader(int argc, char **args, int *no_lines) {
  if (argc == 1) {
    perror("The name is missing.");
    exit(EXIT_FAILURE);
  }

  FILE *f;
  f = fopen(args[1], "r");

  if (f == NULL) {
    perror("File does not exist.");
    exit(EXIT_FAILURE);
  }

  int num = 0;
  int numSpace = 1;

  while(!feof(f)) {
    if (getc(f) == '\n' && numSpace == 0) {
      num++;
      numSpace = 1;
    } else {
      numSpace = 0;
    }
  }
  rewind(f);

  *no_lines = num;
  char* data = (char*) malloc(num * MAX_CHARS * sizeof(char));
  assert(data != NULL);

  fread(data, num * MAX_CHARS, 1, f);
  if (ferror(f)) {
    perror("Errors in reading the file.");
    exit(EXIT_FAILURE);
  }
  return data;
}

void freeStrArr(char* arr) {
  assert(arr != NULL);
  free(arr);
}

void handle_operand(char* operand2, instruct *ins) {
  char *eptr;
  char *save;

  if (operand2[0] == 'r') {
    ins -> operand2 = atoi(strtok_r(operand2, "r", &save));
    ins -> imm = 0;
  } else if (strstr(operand2, "x") != NULL) {
    ins -> operand2 = strtol(operand2, &eptr, 16);
    ins -> imm = 1;
  } else {
    ins -> operand2 = strtol(operand2, &eptr, 10);
    ins -> imm = 1;
  }
  if (abs(ins -> operand2) != ins -> operand2) {
    ins -> u = 0;
  }
}

uint32_t handleReg(char *token) {
  uint32_t res;
  res = token[1] - '0';
  if (token[2] != '\0') {
    res = res * 10 + (token[2] - '0');
  }
  return res;
}

void handleAddress(instruct *ins, char *token) {
  char *str[2], *save;
  str[0] = strtok_r(token, "]", &save);
  str[1] = strtok_r(NULL, "], #", &save);
  if (str[1] == NULL) {
    ins -> p = 1;
    str[0] = strtok_r(str[0], ", #", &save);
    str[1] = strtok_r(NULL, ", #", &save);
  } else {
    ins -> p = 0;
  }
  ins -> imm = 1;
  ins -> rn = handleReg(str[0]);
  if (str[1] != NULL) {
    handle_operand(str[1], ins);
  }
}

void setInstruction(instruct *ins, char line[511], uint32_t *res,
                    uint32_t currAdd, ST *symboltable) {
  char *token;
  char *save;
  token = strtok_r(line, " ,#\n", &save);
  ins -> mnemonic = strdup(token);
  switch (token[0]) {
    case 'b': token = strtok_r(NULL, " ,#\n", &save);
          ins -> expression = getAddress(symboltable, token);
          res[currAdd] = branch(ins, currAdd * 4);
          break;
    case 'c':
    case 't': token = strtok_r(NULL, " ,#\n", &save);
          ins -> rn = handleReg(token);;
          token = strtok_r(NULL, " ,#\n", &save);
          handle_operand(token, ins);
          res[currAdd] = assembler_dataProcessing(ins);
          break;
    case 'm': if (token[1] == 'u' || token[1] == 'l') {
        token = strtok_r(NULL, " ,#\n", &save);
        ins -> rd = handleReg(token);
        token = strtok_r(NULL, " ,#\n", &save);
        ins -> rm = handleReg(token);
        token = strtok_r(NULL, " ,#\n", &save);
        ins -> rs = handleReg(token);
        ins -> imm = 0;
        if ((ins -> mnemonic)[1] == 'l') {
          token = strtok_r(NULL, " ,#\n", &save);
          ins -> rn = handleReg(token);
        }
        res[currAdd] = assembler_multiply(ins);
      } else if (token[1] == 'o') {
        token = strtok_r(NULL, " ,#\n", &save);
        ins -> rd = handleReg(token);;
        token = strtok_r(NULL, " ,#\n", &save);
        handle_operand(token, ins);
        res[currAdd] = assembler_dataProcessing(ins);
      }
          break;
    case 'o':
    case 'e':
    case 'a': if (token[4] == 'q') {
        res[currAdd] = assembler_special(ins);
        break;
      }
    case 'r':
    case 's': if (token[1] != 't') {
        token = strtok_r(NULL, " ,#\n", &save);
        ins -> rd = handleReg(token);
        token = strtok_r(NULL, " ,#\n", &save);
        ins -> rn = handleReg(token);
        token = strtok_r(NULL, " ,#\n", &save);
        handle_operand(token, ins);
        res[currAdd] = assembler_dataProcessing(ins);
        break;
      }
    case 'l': if (token[1] != 's') {
        ins -> rn = 0;
        ins -> u = 1;
        ins -> operand2 = 0;
        token = strtok_r(NULL, " ,#\n", &save);
        ins -> rd = handleReg(token);
        token = strtok_r(NULL, "\n", &save);
        int i, shift = 1;
        if (token[0] == ' ')  {
          shift = 2;
        }
        for (i = 0; i < strlen(token); i++) {
          token[i] = token[i + shift];
        }
        if (token[0] != 'r') {
          handle_operand(token, ins);
          if ((ins -> operand2) > 0xff) {
            ins -> rn = PC;
            ins -> lastAdd = ins -> lastAdd + 1;
            ins -> p = 1;
            res[ins -> lastAdd - 1] = ins -> operand2;
            ins -> operand2 = (ins -> lastAdd - 1) * 4 - currAdd * 4
                              - PIPELINE_EFFECT;
          } else {
            ins -> mnemonic = strdup("mov");
            res[currAdd] = assembler_dataProcessing(ins);
            break;
          }
        } else {
          handleAddress(ins, token);
        }
        res[currAdd] = single_data_process(ins);

      } else {
        token = strtok_r(NULL, " ,#\n", &save);
        ins -> rd = handleReg(token);
        token = strtok_r(NULL, " ,#\n", &save);
        handle_operand(token, ins);
        res[currAdd] = assembler_special(ins);
      }
          break;
  }
}

void printRes(char *file_name, uint32_t *res, uint32_t lastAdd) {
  FILE *f;
  f = fopen(file_name, "w");
  assert(f != NULL);

  fwrite(res, 4, lastAdd, f);
  if (ferror(f)) {
    perror("errors with writing the file.");
    exit(EXIT_FAILURE);
  }
}


