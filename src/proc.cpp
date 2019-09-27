#include "proc.hpp"

proc::proc(){
    reset();
}

void proc::reset(){
    pc = 0;
    sp = 0;
    memset(r,0,sizeof(r));
    st = 0;

    //TODO: Remove memset of memory
    memset(mem,0,sizeof(mem));
}


bool proc::load(const char* filename){

}


