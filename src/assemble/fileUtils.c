#include "fileUtils.h"
#include "definitions.h"

#include <stdlib.h>
#include <memory.h>

source * loadSource(int argc, char **argv){
    source *src= malloc(sizeof(source));
    if (src == NULL) {
        return NULL;
    }
    src->file=loadIn(argc, argv);
    src->buffer = malloc(MAX_LINE_LENGTH);
    fgets(src->buffer, MAX_LINE_LENGTH, src->file);

    return src;
}

FILE * loadIn(int argc, char **argv){ //original design for using only a file
// rather than a struct, now reused to populate the struct
    printf("%d \n", argc);
    if(argc!=3){fprintf(stderr,"Insufficient arguments.\n"); return EXIT_FAILURE;}
    FILE *source;                   //source file is opened
    source = fopen(argv[0], "r");
    if (source == NULL ){fprintf(stderr, "failed to open source file\n"); return EXIT_FAILURE;}
    return source;
}

