/**
 * @file proc.hpp
 * 
 * @author Clay Buxton (clbx, buxtonc@etown.edu)
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <time.h>
#include <cstring>
#include <map>

/**
 * The processor for the cosmic system
 * 
 * Holds all of the internals for the cosmic processor, instructions, and I/O.
 */
class proc{

    private:

        //Bus callbacks
        typedef void (*BusWrite)(uint16_t,uint8_t);
        typedef uint8_t (*BusRead)(uint16_t);
        
        //Read and write callbacks for the bus
        BusWrite Write;
        BusRead Read;

        //Instruction encoding pointers
        typedef void (proc::*Opcode)(uint16_t);
        typedef uint16_t(proc::*Addressing)();

        //Instruction encoding
        typedef struct Instruction{
            Opcode opcode;
            Addressing addressing;
        } Instruction;

        //Instruction Set
        Instruction instructionSet[256];

         //Addresing Modes, return the memory address to start looking at
        uint16_t IMPLIED();
        uint16_t IMMEDIATE();
        uint16_t ABSOLUTE();
        uint16_t RELATIVE();
        uint16_t INDIRECT();

        //Opcodes. Things like MOV will be a little fucky.
        void ADD(uint16_t loc); //Gets handed the location in memory to look at.

    public:
        //Public for Debugger Only
        uint16_t pc; //Program Counter
        uint16_t sp; //Stack Pointer
        uint8_t r[8]; // General Registers
        uint8_t st; //Status Register

        //Public for System Usage
        proc(BusWrite w, BusRead r);
        void reset();
        void execute(uint8_t);
        
};