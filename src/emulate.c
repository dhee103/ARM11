#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<math.h>

uint32_t* fileLoader(char *filename);
uint32_t strToNum(char c[]); 

int main(int argc, char **args) {

  if (argc == 1) {
    fprintf(stderr, "file name is missing\n");
  }

  fileLoader(args[1]); 
  
  uint32_t* i;
  i = fileLoader(args[1]);
  int j;  
  
  for (j = 0; i[j] != 0 ;j++) {
    printf("Line number %d: %d\n", j, i[j]);
  }
  return 0;

}

uint32_t* fileLoader(char *filename) {

  FILE *f;
  size_t size = 32;
  size_t num;
  f = fopen(filename, "r");
  
  fseek(f, 0L, SEEK_END);
  num = ftell(f)/33;
  rewind(f);

  char c[num][32];
  static uint32_t *i;
  i = malloc(num);
  int j;

  for (j = 0; j < num; j++) {
    fread(c[j], size, 1, f);
    i[j] = strToNum(c[j]);
  }
 
  return i;

}

uint32_t strToNum(char c[]) {
  
  int i;
  uint32_t result = 0;

  for (i = 0; i < 32; i++) {
    if (c[i] == '1') {
      result = result + pow(2, 31 - i);
    }
  }

  return result;

}
