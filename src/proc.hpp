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
        uint16_t pc; //Program Counter
        uint16_t sp; //Stack Pointer
        uint8_t r[8]; // General Registers
        uint8_t st; //Status Register

        //TODO: Remove internal RAM
        uint8_t mem[128];

        //TODO: Implement Pinout
        bool dataBus[8];
        bool addressBus[16];


        /* Processor directive functions */
        proc();
        void reset();
        bool load(const char*);
        void execute(uint8_t);

        /* Memory functions */
        uint8_t nextByte();
        

    private:
        void ADD();
        void SUB();
        void MUL();
        void DIV();

}