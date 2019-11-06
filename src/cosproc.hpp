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
#include <string>
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
        typedef uint16_t (cosproc::*Addressing)();
        typedef void (cosproc::*Opcode)(uint16_t);
        
        


        

        // -= ADDRESSING MODES =-
        uint16_t IMP(); //Implied Addressing
        uint16_t IMM(); //Immediate Addressing
        uint16_t ABS(); //Absolute Addressing
        uint16_t IND(); //Indirect Addressing
        uint16_t REG(); //Register Addressing

        // -= OPCODES =-  R = Register mode, X = 16-bit Mode
        void UND(uint16_t src);  //Undefined
        void NOP(uint16_t src);  //No Operation
        void HCF(uint16_t src);  //Halt and Catch Fire
        void PUSH(uint16_t src); //Push to stack
        void POP(uint16_t src);  //Pop from stack
        void SWP(uint16_t src);  //Swap registers

        void ADD(uint16_t src);   //Add
        void ADDR(uint16_t src);  //Add from Register
        void ADDX(uint16_t src);  //Add from 16 bit value
        void ADDXR(uint16_t src); //Add from 16 bit register

        void SUB(uint16_t src);   //Subtract
        void SUBR(uint16_t src);  //Subtract from Register
        void SUBX(uint16_t src);  //Subtract from 16 bit value
        void SUBXR(uint16_t src); //Subtract from 16bit register

        void MUL(uint16_t src);   //Multiply
        void MULR(uint16_t src);  //Multiply from register
        void MULX(uint16_t src);  //Multuply from 16 bit value
        void MULXR(uint16_t src); //Multiply from 16bit register

        void DIV(uint16_t src);   //Divide
        void DIVR(uint16_t src);  //Divide from register
        void DIVX(uint16_t src);  //Divide from 16 bit value
        void DIVXR(uint16_t src); //Divide from 16bit register

        void MOVAI(uint16_t src); //Move to Absolute from Immediate
        void MOVA(uint16_t src);  //Move to Absolute
        void MOVAR(uint16_t src); //Move to Absolute from Register
        void MOVII(uint16_t src); //Move to Indirect from Immediate
        void MOVI(uint16_t src);  //Move to Indirect
        void MOVIR(uint16_t src); //Move to Indirect from Register
        void MOVRI(uint16_t src); //Move to Register from Immediate
        void MOVR(uint16_t src);  //Move to Register
        void MOVRR(uint16_t src); //Move to Register from Register

        void MOVXAI(uint16_t src); //Move to Absolute from Immediate
        void MOVXA(uint16_t src);  //Move to Absolute
        void MOVXAR(uint16_t src); //Move to Absolute from Register
        void MOVXII(uint16_t src); //Move to Indirect from Immediate
        void MOVXI(uint16_t src);  //Move to Indirect
        void MOVXIR(uint16_t src); //Move to Indirect from Register
        void MOVXRI(uint16_t src); //Move to Register from Immediate
        void MOVXR(uint16_t src);  //Move to Register
        void MOVXRR(uint16_t src); //Move to Register from Register

    public:
        //Public for Debugger Only (Package this up eventually)
        uint16_t pc; //Program Counter
        uint16_t sp; //Stack Pointer
        uint8_t r[8]; // General Registers
        bool st[8]; //Status Register

        
        typedef struct {
            Addressing addressing;
            Opcode opcode;
            const char* mnemonic;
            int bytes;
        } Instruction;

        Instruction InstructionSet[256];
        

        //Public for System Usage
        cosproc(BusRead r, BusWrite w);
        void reset();
        void cycle();
        void NMI();   //Non Maskable Interrupt
        void INTR();  //Interrupt request
        void execute(Instruction i);


};