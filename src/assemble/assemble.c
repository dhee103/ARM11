#include "run.c"

int main(int argc, char **args) {

  instruct *ins = malloc(sizeof(instruct));
  ST *symboltable = malloc(sizeof(ST));

  int i = 0, *num = &i, no_lines;
  uint32_t currAddress = 0;
  char *data, *save, *label;

  data = loader(argc, args, num);
  freeStrArr(data);
  label = (char *) malloc(MAX_CHARS * sizeof(char));

  char **array = twoD(*num, MAX_CHARS);
  no_lines = *num;
  array[0] = strtok_r(data, "\n", &save);

  for (i = 1; i < no_lines; i++) {
    array[i] = strtok_r(NULL, "\n",&save);
  }

  for (i = 0; i < no_lines ; i++) {

    if (strstr(array[i], ":") != NULL) {
      label = strdup(array[i]);
      label[strlen(label) - 1] = '\0';
      add_Symbol(currAddress * 4, label, symboltable);
    } else {
      currAddress++;
    }
  }

  ins -> lastAdd = currAddress;
  uint32_t *res = (uint32_t*) malloc(currAddress * 2 * sizeof(uint32_t));
  assert(res != NULL);
  currAddress = 0;


  for (i = 0; i < no_lines; i++) {
    if (strstr(array[i], ":") == NULL) {
      setInstruction(ins, array[i], res, currAddress, symboltable);
      currAddress++;
    }
  }

  printRes(args[2], res, ins -> lastAdd);
    return 0;
}