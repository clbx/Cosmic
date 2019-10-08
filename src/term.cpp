#include <stdio.h>
#include "cosproc.hpp"
#include <stdlib.h>

uint8_t MemoryRead(uint16_t address){
    printf(" READ: %X\n",address);
    return 0x00;

}

void MemoryWrite(uint16_t address, uint8_t value){
    printf("%X\n",address);
    printf("%X\n", value);
}

int main(){
     cosproc chip = cosproc(MemoryRead, MemoryWrite);
     chip.run(5); 
}