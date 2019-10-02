/**
 * @file proc.hpp
 * 
 * @author Clay Buxton (clbx, buxtonc@etown.edu)
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
        typedef void (*BusWrite)(uint16_t, uint8_t);
        typedef uint8_t (*BusRead)(uint16_t);

        //Read and write callbacks for the bus
        BusWrite Write;
        BusRead Read;
        
        //Instruction encoding pointers
        typedef void (proc::*Opcode)(uint16_t);
        typedef uint16_t (proc::*Addressing)();

        //Instruction encoding
        typedef struct Instruction{
            Opcode opcode;
            Addressing addressing;
        } Instruction;

        Instruction InstructionSet[256];

        

        // -= ADDRESSING MODES =-
        uint16_t implied();
        uint16_t immediate();
        uint16_t absolute();
        uint16_t relative();
        uint16_t indirect();

        // -= OPCODES =-
        void NOP(uint16_t src);



    public:
        //Public for Debugger Only (Package this up eventually)
        uint16_t pc; //Program Counter
        uint16_t sp; //Stack Pointer
        uint8_t r[8]; // General Registers
        uint8_t st; //Status Register

        //Public for System Usage
        proc(BusRead r, BusWrite w);
        void reset();
        void run(uint32_t n);
        void execute(Instruction i);

};