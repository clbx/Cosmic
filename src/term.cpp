#include <stdio.h>
#include "proc.hpp"
#include <stdlib.h>

uint8_t MemoryRead(uint16_t address){
    printf(" READ: %X\n",address);
    return 0xEA;

}

void MemoryWrite(uint16_t address, uint8_t value){
    printf("%X\n",address);
    printf("%X\n", value);
}

int main(){
     proc chip = proc(MemoryWrite,MemoryRead); 
}