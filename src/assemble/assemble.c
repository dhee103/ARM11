#include "definitions.h"

#include <stdlib.h>

int main(int argc, char **argv) {
    //start file io, check files are accessible (or create output)
    uint32_t *bin;
    source *src;
    src = loadSource(argc, argv);
    if(src==NULL){fprintf(stderr, "something went wrong\n"); return EXIT_FAILURE;}
    printf(src->buffer);


    while (src->buffer[1]!=NULL){//EOF
        fgets(src->buffer, MAX_LINE_LENGTH, src->file);
        int j=0;
        src->numLines+=1;
        src->numInstructs+=1;
        do {
            if (src->buffer[j]==":"){
                src->numLabels+=1;
                src->numInstructs-=1;
            }
        } while (src->buffer[j]!=NULL);
    }
//    uint32_t out[src->numInstructs]
    /*line by line the src file, extract shift values greater than 12 bit capacity, or rotated immediate of 8 bits.
     * build array for each
     * then build empty instruction array of appropriate length (# of instrs + # of additional variables/values)
     */
    return EXIT_SUCCESS;
}
