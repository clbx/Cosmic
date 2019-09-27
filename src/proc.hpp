/**
 * @file proc.hpp
 * 
 * @author Clay Buxton (clbx, buxtonc@etown.edu)
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <cstring>



/**
 * The processor for the cosmic system
 * 
 * Holds all of the internals for the cosmic processor, instructions, and I/O.
 * Cannot be run on it's own requires external memory.
 */
class proc{
    public:
        //Public for Debugger Only
        uint16_t pc; //Program Counter
        uint16_t sp; //Stack Pointer
        uint8_t r[8]; // General Registers
        uint8_t st; //Status Register

        //Public for System Usage
        proc();
        void busWrite(uint16_t, uint8_t);
        uint16_t busRead();
        void reset();
        void execute(uint8_t);
        
        //Figure out way to handle bus

    private:

        //Addresing Modes
        void IMPLIED();
        void IMMEDIATE();
        void ABSOLUTE();
        void RELATIVE();
        void INDIRECT();

        //Opcodes
        void ADD();
        void SUB();
        void MUL();
        void DIV();

}