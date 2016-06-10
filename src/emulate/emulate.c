#include "utilities.h"

int main(int argc, char **argv) {
/*    initialises a virtual machines state comprising a simulated memory,
 *    registers, and associated instruction data
 */
    state *st = newState();
/*    populates the memory with instructions and data   */
    if (!loader(st, argc, argv)) {
        return EXIT_FAILURE;
    }

/*    termination check is run on each instruction cycle by reading the next
 *    instruction
 */
    while (!isTerminate(st)) {
        if (checkCond(st)) {
            selectInstruction(st);
        }
/*    increment pc */
        st->reg[PC] += BYTES_IN_WORD;
    }

/*    In order to simulate the pipeline the PC must be incremented after
 *    fetching the termination instruction, since the emulator does not
 *    process it, whereas in the real system, the cycle would increment the PC
 */
    st->reg[PC] += PC_AHEAD_BYTES;
/*    output register state     */
    output(st);
/*    delete state and free the memory allocated to it  */
    freeState(st);
    return EXIT_SUCCESS;
}