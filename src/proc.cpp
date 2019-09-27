#include "proc.hpp"

/**
 * Constuctor
 * 
 * Sets up instruction table
 * 
 */
proc::proc(){
    //TODO: Setup Instruction Table Here.
    reset();
}

/**
 * Reset the processor to beginning state
 */
void proc::reset(){
    pc = 0;
    sp = 0;
    memset(r,0,sizeof(r));
    st = 0;
}





