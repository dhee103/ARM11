#include "definitions.h"

#include <stdlib.h>

int main(int argc, char **argv) {
    //start file io, check files are accessible (or create output)
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
        //identify big values and labels
        // for labels, offset all successive lines (by -1) and have label address
        // next line (including previous offset(s))

        //could make a struct of size= #lines to be output = #instructs + #bigvars - #labels + #labels further than 12b

    }

    /*line by line the src file, extract immediate values greater than 12 bit capacity
     * build array for each
     * then build empty instruction array of appropriate length (# of instrs + # of additional variables/values)
     */
    return EXIT_SUCCESS;
}
