/*
 * Our control unit is the brain of our cores, it will contain
 *  - Instruction cache: hold instructions for Warps? Or Threads?
 *  - decoder: Decodes instructions and gets ready to send control
 *  signals throughout the Compute Unit for each thread.
 *  - dispatcher: Dispatches control signals to the rest of the Compute
 *  Unit based on decoded instructions.
 *  -
 * */
