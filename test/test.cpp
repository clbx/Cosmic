#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch-2.hpp"
#define private public //basically the devil, but oh well
#include "../src/cosproc.hpp"



uint8_t memory[256] = { };

void MemoryWrite(uint16_t address, uint8_t value){
    //TODO: Actually put memory here
    memory[address] = value;
    printf("Wrote %X to %X\n",value,address);
}

uint8_t MemoryRead(uint16_t address){
    printf("READ: %X from %X\n",memory[address],address);
    return memory[address];
}

void resetMemory(){
    for(int i = 0; i < 256; i++){
        memory[i] = 0;
    }
}


//Tests

TEST_CASE( "implied", "[addresssing]" ) {
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    proc.pc = 0;
    REQUIRE(proc.IMP() == 0);
    proc.pc = 0x300;
    REQUIRE(proc.IMP() == 0);
}

TEST_CASE("immediate","[addressing]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    proc.pc = 0;
    REQUIRE(proc.IMM() == 1);
    proc.pc = 0x300;
    REQUIRE(proc.IMM() == 0x301);
}

TEST_CASE("absolute","[addressing]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    resetMemory();
    proc.pc = 0;
    // [ 00 00 FF]
    memory[0] = 0xFF; 
    memory[2] = 0xFF;
    REQUIRE(proc.ABS() == 0x00FF);
    memory[1] = 0x0F;
    memory[2] = 0x0F;
    REQUIRE(proc.ABS() == 0x0F0F);
}

TEST_CASE("indirect","[addressing]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    resetMemory();
    proc.pc = 0;
    /*
    00:  FF 10 00 00 00 ...
    10:  00 20 00 00 00 ...
    */
    memory[0x00] = 0xFF;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    REQUIRE(proc.IND() == 0x0020);
}


TEST_CASE("add","[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    resetMemory();
    //Normal
    memory[0x01] = 0x01;
    proc.ADD(0x01);
    REQUIRE(proc.r[0] == 0x01);
    REQUIRE(proc.st[0] == false);
    REQUIRE(proc.st[1] == false);
    REQUIRE(proc.st[2] == false);
    REQUIRE(proc.st[3] == false);
    //Negative
    memory[0x01] = 0x80;
    proc.r[0] = 0;
    proc.ADD(0x01);
    REQUIRE(proc.r[0] == 0x80);
    REQUIRE(proc.st[0] == false);
    REQUIRE(proc.st[1] == true);
    REQUIRE(proc.st[2] == false);
    REQUIRE(proc.st[3] == false);
}