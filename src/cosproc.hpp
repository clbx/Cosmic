/**
 * @file cosproc.hpp
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
 * 
 */
class cosproc{
    
    private:

        //Bus callbacks
        typedef void (*BusWrite)(uint16_t, uint8_t);
        typedef uint8_t (*BusRead)(uint16_t);

        //Read and write callbacks for the bus
        BusWrite Write;
        BusRead Read;
        
        //Instruction encoding pointers
        typedef void (cosproc::*Opcode)(uint16_t);
        typedef uint16_t (cosproc::*Addressing)();

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
        void dregister();

        // -= OPCODES =-  R = Register mode, X = 16-bit Mode
        void NOP(uint16_t src);
        void HCF(uint16_t src);
        void PUSH(uint16_t src);
        void POP(uint16_t src);
        void SWP(uint16_t src);
        void ADD(uint16_t src);
        void ADDR();
        void ADDX(uint16_t src);
        void SUB(uint16_t src);
        void SUBR();
        void SUBX(uint16_t src);
        void MUL(uint16_t src);
        void MULR();
        void MULX(uint16_t src);
        void DIV(uint16_t src);
        void DIVR();
        void DIVX(uint16_t src);



    public:
        //Public for Debugger Only (Package this up eventually)
        uint16_t pc; //Program Counter
        uint16_t sp; //Stack Pointer
        uint8_t r[8]; // General Registers
        uint8_t st; //Status Register

        //Public for System Usage
        cosproc(BusRead r, BusWrite w);
        void reset();
        void cycle();
        void execute(Instruction i);

};