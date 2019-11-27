#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch-2.hpp"
#define private public //basically the devil, but oh well
#include "../src/cosproc.hpp"



uint8_t memory[65536] = { };

void MemoryWrite(uint16_t address, uint8_t value){
    //TODO: Actually put memory here
    memory[address] = value;
    printf("Wrote %X to %X\n",value,address);
}

uint8_t MemoryRead(uint16_t address){
    printf("READ: %X from %X\n",memory[address],address);
    return memory[address];
}

void reset(cosproc* proc){
    memset(proc->r,0,sizeof(proc->r));
	proc->pc = 0;
    proc->sp = 0xC399;
    memset(proc->st,0,sizeof(proc->st));
    memset(memory,0,sizeof(memory));
}

//Tests
TEST_CASE( "implied", "[addresssing]" ) {
    cosproc proc = cosproc(MemoryRead, MemoryWrite);
    reset(&proc);
    REQUIRE(proc.IMP() == 0); //Returns 0
}

TEST_CASE("immediate","[addressing]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    reset(&proc);
    REQUIRE(proc.IMM() == 1); //Increments program counter by 1 so 0 + 1 = 1
}

TEST_CASE("absolute","[addressing]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    reset(&proc);
    // [00 10 20] 
    memory[1] = 0x10;
    memory[2] = 0x20;
    REQUIRE(proc.ABS() == 0x1020);
}

TEST_CASE("indirect","[addressing]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    reset(&proc);
    /*
    0000:  00 00 10 ...
    0010:  00 20 ...
    */
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    REQUIRE(proc.IND() == 0x0020);
}


// TEST_CASE("add","[opcodes]"){
//     cosproc proc = cosproc(MemoryRead, MemoryWrite); 
//     resetMemory();
//     //Normal
//     memory[0x01] = 0x01;
//     proc.ADD(0x01);
//     REQUIRE(proc.r[0] == 0x01);
//     REQUIRE(proc.st[0] == false);
//     REQUIRE(proc.st[1] == false);
//     REQUIRE(proc.st[2] == false);
//     REQUIRE(proc.st[3] == false);
//     //Negative
//     memory[0x01] = 0x80;
//     proc.r[0] = 0;
//     proc.ADD(0x01);
//     REQUIRE(proc.r[0] == 0x80);
//     REQUIRE(proc.st[0] == false);
//     REQUIRE(proc.st[1] == true);
//     REQUIRE(proc.st[2] == false);
//     REQUIRE(proc.st[3] == false);
// }

/* 0x70-0x73 */
TEST_CASE("jmp", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    //Imm
    // [70 00 10]
    reset(&proc);
    memory[0x00] = 0x70;
    memory[0x02] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Abs
    /*
    0000: 71 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x71;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(proc.pc == 0x0020);
    //Ind
    /*
    0000: 72 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x72;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.cycle();
    REQUIRE(proc.pc == 0x0030);
    //Reg
    // [73 00]
    reset(&proc);
    memory[0x00] = 0x73;
    proc.r[1] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
}

/* 0x74-0x77 */
TEST_CASE("jzs", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    //Imm
    //Not zero
    // [74 00 10]
    reset(&proc);
    memory[0x00] = 0x74;
    memory[0x02] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Zero
    // [74 00 10]
    reset(&proc);
    memory[0x00] = 0x74;
    memory[0x02] = 0x10;
    proc.st[0] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Abs
    //Not zero
    /*
    0000: 75 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x75;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Zero
    /*
    0000: 75 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x75;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.st[0] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0020);
    //Ind
    //Not zero
    /*
    0000: 76 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x76;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Ind
    //Zero
    /*
    0000: 76 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x76;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.st[0] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0030);
    //Reg
    //Not zero
    // [77 00]
    reset(&proc);
    memory[0x00] = 0x77;
    proc.r[1] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0002);
    //Zero
    // [77 00]
    reset(&proc);
    memory[0x00] = 0x77;
    proc.r[1] = 0x10;
    proc.st[0] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
}

/* 0x78-0x7B */
TEST_CASE("jnz", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    //Imm
    //Not zero
    // [78 00 10]
    reset(&proc);
    memory[0x00] = 0x78;
    memory[0x02] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Zero
    // [78 00 10]
    reset(&proc);
    memory[0x00] = 0x78;
    memory[0x02] = 0x10;
    proc.st[0] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Abs
    //Not zero
    /*
    0000: 79 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x79;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(proc.pc == 0x0020);
    //Zero
    /*
    0000: 79 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x79;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.st[0] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Ind
    //Not zero
    /*
    0000: 7A 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x7A;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.cycle();
    REQUIRE(proc.pc == 0x0030);
    //Ind
    //Zero
    /*
    0000: 7A 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x7A;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.st[0] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Reg
    //Not zero
    // [7B 00]
    reset(&proc);
    memory[0x00] = 0x7B;
    proc.r[1] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Zero
    // [7B 00]
    reset(&proc);
    memory[0x00] = 0x7B;
    proc.r[1] = 0x10;
    proc.st[0] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0002);
}

/* 0x7C-0x7F */
TEST_CASE("jcs", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    //Imm
    //Not carry
    // [7C 00 10]
    reset(&proc);
    memory[0x00] = 0x7C;
    memory[0x02] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Carry
    // [7C 00 10]
    reset(&proc);
    memory[0x00] = 0x7C;
    memory[0x02] = 0x10;
    proc.st[2] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Abs
    //Not carry
    /*
    0000: 7D 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x7D;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Carry
    /*
    0000: 7D 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x7D;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.st[2] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0020);
    //Ind
    //Not carry
    /*
    0000: 7E 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x7E;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Ind
    //Carry
    /*
    0000: 7E 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x7E;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.st[2] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0030);
    //Reg
    //Not carry
    // [7F 00]
    reset(&proc);
    memory[0x00] = 0x7F;
    proc.r[1] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0002);
    //Carry
    // [7F 00]
    reset(&proc);
    memory[0x00] = 0x7F;
    proc.r[1] = 0x10;
    proc.st[2] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
}

/* 0x80-0x83 */
TEST_CASE("jnc", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    //Imm
    //Not carry
    // [80 00 10]
    reset(&proc);
    memory[0x00] = 0x80;
    memory[0x02] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Carry
    // [80 00 10]
    reset(&proc);
    memory[0x00] = 0x80;
    memory[0x02] = 0x10;
    proc.st[2] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Abs
    //Not carry
    /*
    0000: 81 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x81;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(proc.pc == 0x0020);
    //Carry
    /*
    0000: 81 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x81;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.st[2] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Ind
    //Not carry
    /*
    0000: 82 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x82;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.cycle();
    REQUIRE(proc.pc == 0x0030);
    //Ind
    //Carry
    /*
    0000: 82 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x82;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.st[2] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Reg
    //Not carry
    // [83 00]
    reset(&proc);
    memory[0x00] = 0x83;
    proc.r[1] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Carry
    // [83 00]
    reset(&proc);
    memory[0x00] = 0x83;
    proc.r[1] = 0x10;
    proc.st[2] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0002);
}

/* 0x84-0x87 */
TEST_CASE("jos", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    //Imm
    //Not overflow
    // [84 00 10]
    reset(&proc);
    memory[0x00] = 0x84;
    memory[0x02] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Overflow
    // [84 00 10]
    reset(&proc);
    memory[0x00] = 0x84;
    memory[0x02] = 0x10;
    proc.st[3] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Abs
    //Not overflow
    /*
    0000: 85 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x85;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Overflow
    /*
    0000: 85 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x85;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.st[3] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0020);
    //Ind
    //Not overflow
    /*
    0000: 86 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x86;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Ind
    //Overflow
    /*
    0000: 86 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x86;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.st[3] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0030);
    //Reg
    //Not overflow
    // [87 00]
    reset(&proc);
    memory[0x00] = 0x87;
    proc.r[1] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0002);
    //Overflow
    // [87 00]
    reset(&proc);
    memory[0x00] = 0x87;
    proc.r[1] = 0x10;
    proc.st[3] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
}

/* 0x88-0x8B */
TEST_CASE("jns", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    //Imm
    //Not negative
    // [88 00 10]
    reset(&proc);
    memory[0x00] = 0x88;
    memory[0x02] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Negative
    // [88 00 10]
    reset(&proc);
    memory[0x00] = 0x88;
    memory[0x02] = 0x10;
    proc.st[1] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
    //Abs
    //Not negative
    /*
    0000: 89 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x89;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Negative
    /*
    0000: 89 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x89;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    proc.st[1] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0020);
    //Ind
    //Not negative
    /*
    0000: 8A 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x8A;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.cycle();
    REQUIRE(proc.pc == 0x0003);
    //Ind
    //Negative
    /*
    0000: 8A 00 10 ...
    0010: 00 20 ...
    0020: 00 30 ...
    */
    reset(&proc);
    memory[0x00] = 0x8A;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x21] = 0x30;
    proc.st[1] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0030);
    //Reg
    //Not negative
    // [8B 00]
    reset(&proc);
    memory[0x00] = 0x8B;
    proc.r[1] = 0x10;
    proc.cycle();
    REQUIRE(proc.pc == 0x0002);
    //Negative
    // [8B 00]
    reset(&proc);
    memory[0x00] = 0x8B;
    proc.r[1] = 0x10;
    proc.st[1] = 1;
    proc.cycle();
    REQUIRE(proc.pc == 0x0010);
}