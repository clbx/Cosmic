#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch-2.hpp"
#define private public //basically the devil, but oh well
#include "../src/cosproc.hpp"



uint8_t memory[65536] = { };

void MemoryWrite(uint16_t address, uint8_t value){
    //TODO: Actually put memory here
    memory[address] = value;
    //printf("Wrote %X to %X\n",value,address);
}

uint8_t MemoryRead(uint16_t address){
    //printf("READ: %X from %X\n",memory[address],address);
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

/* 0x30-0x3B */
TEST_CASE("mov", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite);
    //Imm -> Abs
    /*
    0000: 30 01 00 10 ...
    0010: ...
    */
    reset(&proc);
    memory[0x00] = 0x30;
    memory[0x01] = 0x01;
    memory[0x03] = 0x10;
    proc.cycle();
    REQUIRE(memory[0x0010] == 0x01);
    //Abs -> Abs
    /*
    0000: 31 00 10 00 11 ...
    0010: 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x31;
    memory[0x02] = 0x10;
    memory[0x04] = 0x11;
    memory[0x10] = 0x01;
    proc.cycle();
    REQUIRE(memory[0x0011] == 0x01);
    //Ind -> Abs
    /*
    0000: 32 00 10 00 12 ...
    0010: 00 20 ...
    0020: 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x32;
    memory[0x02] = 0x10;
    memory[0x04] = 0x12;
    memory[0x11] = 0x20;
    memory[0x20] = 0x01;
    proc.cycle();
    REQUIRE(memory[0x0012] == 0x01);
    //Reg -> Abs
    /*
    0000: 33 00 00 10 ...
    */
    reset(&proc);
    proc.r[0] = 0x01;
    memory[0x00] = 0x33;
    memory[0x03] = 0x10;
    proc.cycle();
    REQUIRE(memory[0x0010] == 0x01);
    //Imm -> Ind
    /*
    0000: 34 01 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x34;
    memory[0x01] = 0x01;
    memory[0x03] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(memory[0x0020] == 0x01);
    //Abs -> Ind
    /*
    0000: 35 00 12 00 10 ...
    0010: 00 20 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x35;
    memory[0x02] = 0x12;
    memory[0x04] = 0x10;
    memory[0x11] = 0x20;
    memory[0x12] = 0x01;
    proc.cycle();
    REQUIRE(memory[0x0020] == 0x01);
    //Ind -> Ind
    /*
    0000: 36 00 10 00 12 ...
    0010: 00 20 00 21 ...
    0020: 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x36;
    memory[0x02] = 0x10;
    memory[0x04] = 0x12;
    memory[0x11] = 0x20;
    memory[0x13] = 0x21;
    memory[0x20] = 0x01;
    proc.cycle();
    REQUIRE(memory[0x0021] == 0x01);
    //Reg -> Ind
    /*
    0000: 37 00 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    proc.r[0] = 0x01;
    memory[0x00] = 0x37;
    memory[0x03] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(memory[0x0020] == 0x01);
    //Imm -> Reg
    /*
    0000: 38 01 00 ...
    */
    reset(&proc);
    memory[0x00] = 0x38;
    memory[0x01] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x01);
    //Abs -> Reg
    /*
    0000: 39 00 10 00 ...
    0010: 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x39;
    memory[0x02] = 0x10;
    memory[0x10] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x01);
    //Ind -> Reg
    /*
    0000: 3A 00 10 00 ...
    0010: 00 20 ...
    0020: 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x3A;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x20] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x01);
    //Reg -> Reg
    /*
    0000: 3B 00 01 ...
    */
    reset(&proc);
    proc.r[0] = 0x01;
    memory[0x00] = 0x3B;
    memory[0x02] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[1] == 0x01);
}

/* 0x40-0x4B */
TEST_CASE("movx", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite);
    //Imm -> Abs
    /*
    0000: 40 FF 01 00 10 ...
    0010: ...
    */
    reset(&proc);
    memory[0x00] = 0x40;
    memory[0x01] = 0xFF;
    memory[0x02] = 0x01;
    memory[0x04] = 0x10;
    proc.cycle();
    REQUIRE(memory[0x0010] == 0xFF);
    REQUIRE(memory[0x0011] == 0x01);
    //Abs -> Abs
    /*
    0000: 41 00 10 00 12 ...
    0010: FF 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x41;
    memory[0x02] = 0x10;
    memory[0x04] = 0x12;
    memory[0x10] = 0xFF;
    memory[0x11] = 0x01;
    proc.cycle();
    REQUIRE(memory[0x0012] == 0xFF);
    REQUIRE(memory[0x0013] == 0x01);
    //Ind -> Abs
    /*
    0000: 42 00 10 00 12 ...
    0010: 00 20 ...
    0020: FF 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x42;
    memory[0x02] = 0x10;
    memory[0x04] = 0x12;
    memory[0x11] = 0x20;
    memory[0x20] = 0xFF;
    memory[0x21] = 0x01;
    proc.cycle();
    REQUIRE(memory[0x0012] == 0xFF);
    REQUIRE(memory[0x0013] == 0x01);
    //Reg -> Abs
    /*
    0000: 43 00 00 10 ...
    */
    reset(&proc);
    proc.r[0] = 0xFF;
    proc.r[1] = 0x01;
    memory[0x00] = 0x43;
    memory[0x03] = 0x10;
    proc.cycle();
    REQUIRE(memory[0x0010] == 0xFF);
    REQUIRE(memory[0x0011] == 0x01);
    //Imm -> Ind
    /*
    0000: 44 FF 01 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    memory[0x00] = 0x44;
    memory[0x01] = 0xFF;
    memory[0x02] = 0x01;
    memory[0x04] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(memory[0x0020] == 0xFF);
    REQUIRE(memory[0x0021] == 0x01);
    //Abs -> Ind
    /*
    0000: 45 00 12 00 10 ...
    0010: 00 20 FF 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x45;
    memory[0x02] = 0x12;
    memory[0x04] = 0x10;
    memory[0x11] = 0x20;
    memory[0x12] = 0xFF;
    memory[0x13] = 0x01;
    proc.cycle();
    REQUIRE(memory[0x0020] == 0xFF);
    REQUIRE(memory[0x0021] == 0x01);
    //Ind -> Ind
    /*
    0000: 46 00 10 00 12 ...
    0010: 00 20 00 22 ...
    0020: FF 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x46;
    memory[0x02] = 0x10;
    memory[0x04] = 0x12;
    memory[0x11] = 0x20;
    memory[0x13] = 0x22;
    memory[0x20] = 0xFF;
    memory[0x21] = 0x01;
    proc.cycle();
    REQUIRE(memory[0x0022] == 0xFF);
    REQUIRE(memory[0x0023] == 0x01);
    //Reg -> Ind
    /*
    0000: 47 00 00 10 ...
    0010: 00 20 ...
    */
    reset(&proc);
    proc.r[0] = 0xFF;
    proc.r[1] = 0x01;
    memory[0x00] = 0x47;
    memory[0x03] = 0x10;
    memory[0x11] = 0x20;
    proc.cycle();
    REQUIRE(memory[0x0020] == 0xFF);
    REQUIRE(memory[0x0021] == 0x01);
    //Imm -> Reg
    /*
    0000: 48 FF 01 00 ...
    */
    reset(&proc);
    memory[0x00] = 0x48;
    memory[0x01] = 0xFF;
    memory[0x02] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.r[1] == 0x01);
    //Abs -> Reg
    /*
    0000: 49 00 10 00 ...
    0010: FF 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x49;
    memory[0x02] = 0x10;
    memory[0x10] = 0xFF;
    memory[0x11] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.r[1] == 0x01);
    //Ind -> Reg
    /*
    0000: 4A 00 10 00 ...
    0010: 00 20 ...
    0020: FF 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x4A;
    memory[0x02] = 0x10;
    memory[0x11] = 0x20;
    memory[0x20] = 0xFF;
    memory[0x21] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.r[1] == 0x01);
    //Reg -> Reg
    /*
    0000: 4B 00 02 ...
    */
    reset(&proc);
    proc.r[0] = 0xFF;
    proc.r[1] = 0x01;
    memory[0x00] = 0x4B;
    memory[0x02] = 0x02;
    proc.cycle();
    REQUIRE(proc.r[2] == 0xFF);
    REQUIRE(proc.r[3] == 0x01);
}

/* 0x50-0x53 */
TEST_CASE("and", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite);
    //Imm
    /*
    0000: 50 FF ...
    */
    reset(&proc);
    proc.r[0] = 0x0F;
    memory[0x00] = 0x50;
    memory[0x01] = 0xFF;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x0F);
    REQUIRE(proc.st[0] == 0);
    //Abs
    /*
    0000: 51 00 03 FF ...
    */
    reset(&proc);
    proc.r[0] = 0x0F;
    memory[0x00] = 0x51;
    memory[0x02] = 0x03;
    memory[0x03] = 0xFF;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x0F);
    REQUIRE(proc.st[0] == 0);
    //Ind
    /*
    0000: 52 00 03 00 05 FF ...
    */
    reset(&proc);
    proc.r[0] = 0xF0;
    memory[0x00] = 0x52;
    memory[0x02] = 0x03;
    memory[0x04] = 0x05;
    memory[0x05] = 0xFF;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xF0);
    REQUIRE(proc.st[0] == 0);
    //Reg
    /*
    0000: 53 01 ...
    */
    reset(&proc);
    proc.r[0] = 0xF0;
    proc.r[1] = 0xFF;
    memory[0x00] = 0x53;
    memory[0x01] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xF0);
    REQUIRE(proc.st[0] == 0);
    //Zero Flag
    /*
    0000: 53 01 ...
    */
    reset(&proc);
    proc.r[0] = 0xF0;
    proc.r[1] = 0x0F;
    memory[0x00] = 0x53;
    memory[0x01] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x00);
    REQUIRE(proc.st[0] == 1);
}

/* 0x54-0x57 */
TEST_CASE("or", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite); 
    //Imm
    /*
    0000: 54 F0 ...
    */
    reset(&proc);
    proc.r[0] = 0x0F;
    memory[0x00] = 0x54;
    memory[0x01] = 0xF0;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.st[0] == 0);
    //Abs
    /*
    0000: 55 00 03 F0 ...
    */
    reset(&proc);
    proc.r[0] = 0x0F;
    memory[0x00] = 0x55;
    memory[0x02] = 0x03;
    memory[0x03] = 0xF0;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.st[0] == 0);
    //Ind
    /*
    0000: 56 00 03 00 05 0F ...
    */
    reset(&proc);
    proc.r[0] = 0xF0;
    memory[0x00] = 0x56;
    memory[0x02] = 0x03;
    memory[0x04] = 0x05;
    memory[0x05] = 0x0F;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.st[0] == 0);
    //Reg
    /*
    0000: 57 01 ...
    */
    reset(&proc);
    proc.r[0] = 0xF0;
    proc.r[1] = 0xFF;
    memory[0x00] = 0x57;
    memory[0x01] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.st[0] == 0);
    //Zero Flag
    /*
    0000: 57 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x57;
    memory[0x01] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x00);
    REQUIRE(proc.st[0] == 1);
}

/* 0x58-0x5B */
TEST_CASE("xor", "[opcodes]"){
    cosproc proc = cosproc(MemoryRead, MemoryWrite);
    //Imm
    /*
    0000: 58 F0 ...
    */
    reset(&proc);
    proc.r[0] = 0x0F;
    memory[0x00] = 0x58;
    memory[0x01] = 0xF0;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.st[0] == 0);
    //Abs
    /*
    0000: 59 00 03 F0 ...
    */
    reset(&proc);
    proc.r[0] = 0xFF;
    memory[0x00] = 0x59;
    memory[0x02] = 0x03;
    memory[0x03] = 0xF0;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x0F);
    REQUIRE(proc.st[0] == 0);
    //Ind
    /*
    0000: 5A 00 03 00 05 0F ...
    */
    reset(&proc);
    proc.r[0] = 0xF0;
    memory[0x00] = 0x5A;
    memory[0x02] = 0x03;
    memory[0x04] = 0x05;
    memory[0x05] = 0x0F;
    proc.cycle();
    REQUIRE(proc.r[0] == 0xFF);
    REQUIRE(proc.st[0] == 0);
    //Reg
    /*
    0000: 5B 01 ...
    */
    reset(&proc);
    proc.r[0] = 0xF0;
    proc.r[1] = 0xFF;
    memory[0x00] = 0x5B;
    memory[0x01] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x0F);
    REQUIRE(proc.st[0] == 0);
    //Zero Flag
    /*
    0000: 5B 01 ...
    */
    reset(&proc);
    memory[0x00] = 0x5B;
    memory[0x01] = 0x01;
    proc.cycle();
    REQUIRE(proc.r[0] == 0x00);
    REQUIRE(proc.st[0] == 1);

}

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
