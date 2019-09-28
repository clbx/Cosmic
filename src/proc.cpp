#include "proc.hpp"

/**
 * Constuctor
 * 
 * Sets up instruction table
 * 
 */
proc::proc(BusWrite w, BusRead r){

    Write = (BusWrite)w;
    Read = (BusRead)r;

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


void proc::execute(uint8_t op){

}




