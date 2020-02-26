/**
 * @file cosproc.hpp
 * 
 * @author Clay Buxton  (clbx, buxtonc@etown.edu)
 * @author Kevin Carman (carmank, carmank@etown.edu)
 * 
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
        /* 0x00-0x08 */
        void UND(uint16_t src);  //Undefined
        void NOP(uint16_t src);  //No Operation
        void HCF(uint16_t src);  //Halt and Catch Fire
        void PUSH(uint16_t src); //Push to stack
        void POP(uint16_t src);  //Pop from stack
        void SWP(uint16_t src);  //Swap registers
        void CALL(uint16_t src); //Call subroutine
        void RET(uint16_t src);  //Return from subroutine

        /* 0x10-0x1F */
        void ADD(uint16_t src);   //Add
        void ADDR(uint16_t src);  //Add from Register
        void ADDX(uint16_t src);  //Add from 16-bit value
        void ADDXR(uint16_t src); //Add from 16-bit register

        void SUB(uint16_t src);   //Subtract
        void SUBR(uint16_t src);  //Subtract from Register
        void SUBX(uint16_t src);  //Subtract from 16-bit value
        void SUBXR(uint16_t src); //Subtract from 16-bit register

        /* 0x20-0x2F */
        void MUL(uint16_t src);   //Multiply
        void MULR(uint16_t src);  //Multiply from register
        void MULX(uint16_t src);  //Multuply from 16 bit value
        void MULXR(uint16_t src); //Multiply from 16bit register

        void DIV(uint16_t src);   //Divide
        void DIVR(uint16_t src);  //Divide from register
        void DIVX(uint16_t src);  //Divide from 16-bit value
        void DIVXR(uint16_t src); //Divide from 16-bit register

        /* 0x30-0x3F */
        void MOVAI(uint16_t src); //Move to Absolute from Immediate
        void MOVA(uint16_t src);  //Move to Absolute
        void MOVAR(uint16_t src); //Move to Absolute from Register
        void MOVII(uint16_t src); //Move to Indirect from Immediate
        void MOVI(uint16_t src);  //Move to Indirect
        void MOVIR(uint16_t src); //Move to Indirect from Register
        void MOVRI(uint16_t src); //Move to Register from Immediate
        void MOVR(uint16_t src);  //Move to Register
        void MOVRR(uint16_t src); //Move to Register from Register

        void SHL(uint16_t src);   //Shift the Accumulator left
        void SHLR(uint16_t src);  //Shift the Accumulator left from Register

        /* 0x40-0x4F */
        void MOVXAI(uint16_t src); //Move to Absolute from Immediate
        void MOVXA(uint16_t src);  //Move to Absolute
        void MOVXAR(uint16_t src); //Move to Absolute from Register
        void MOVXII(uint16_t src); //Move to Indirect from Immediate
        void MOVXI(uint16_t src);  //Move to Indirect
        void MOVXIR(uint16_t src); //Move to Indirect from Register
        void MOVXRI(uint16_t src); //Move to Register from Immediate
        void MOVXR(uint16_t src);  //Move to Register
        void MOVXRR(uint16_t src); //Move to Register from Register

        void SHLX(uint16_t src);  //Shift the 16-bit Accumulator left
        void SHLXR(uint16_t src); //Shift the 16-bit Accumulator left from Register

        /* 0x50-0x5F */
        void AND(uint16_t src);  //AND with Accumulator
        void ANDR(uint16_t src); //AND with Accumulator from register

        void OR(uint16_t src);  //OR with Accumulator
        void ORR(uint16_t src); //OR with Accumulator from register

        void XOR(uint16_t src);  //XOR with Accumulator
        void XORR(uint16_t src); //XOR with Accumulator from register

        void SHR(uint16_t src);   //Shift the Accumulator right
        void SHRR(uint16_t src);  //Shift the Accumulator right from register

        /* 0x60-0x67 */
        void CMP(uint16_t src);   //Compare with Accumulator
        void CMPR(uint16_t src);  //Compare with Accumulator from register
        void CMPX(uint16_t src);  //Compare with 16-bit Accumulator
        void CMPXR(uint16_t src); //Compare with 16-bit Accumulator from register

        /* 0x6C-0x6F */
        void SHRX(uint16_t src);  //Shift the 16-bit Accumulator right
        void SHRXR(uint16_t src); //Shift the 16-bit Accumulator right from register

        /* 0x70-0x9F Jumps */
        void JMP(uint16_t src);  //Jump to a location in memory
        void JMPR(uint16_t src); //Jump to a location in memory from register
        void JZS(uint16_t src);  //Jump if zero flag is set
        void JZSR(uint16_t src); //Jump if zero flag is set from register
        void JNZ(uint16_t src);  //Jump if zero flag is not set
        void JNZR(uint16_t src); //Jump if zero flag is not set from register
        void JCS(uint16_t src);  //Jump if carry flag is set
        void JCSR(uint16_t src); //Jump if carry flag is set from register
        void JNC(uint16_t src);  //Jump if carry flag is not set
        void JNCR(uint16_t src); //Jump if carry flag is not set register
        void JOS(uint16_t src);  //Jump if overflow is set
        void JOSR(uint16_t src); //Jump if overflow is set register
        void JNO(uint16_t src);  //Jump if overflow is not set
        void JNOR(uint16_t src); //Jump if overflow is not set register
        void JNS(uint16_t src);  //Jump if negative is set
        void JNSR(uint16_t src); //Jump if negative is set register
        void JNN(uint16_t src);  //Jump if negative is not set
        void JNNR(uint16_t src); //Jump if negative is not register
        void JLS(uint16_t src);  //Jump if less is set
        void JLSR(uint16_t src); //Jump if less is set register
        void JNL(uint16_t src);  //Jump if less is not set
        void JNLR(uint16_t src); //Jump if less is not set register
        void JES(uint16_t src);  //Jump if error is set
        void JESR(uint16_t src); //Jump if error is set register

        /* 0xA0-0xAD Status Flags */
        void CSF(uint16_t src); //Clear all flags
        void CZF(uint16_t src); //Clear zero flag 
        void SZF(uint16_t src); //Set zero flag
        void CNF(uint16_t src); //Clear negative flag
        void SNF(uint16_t src); //Set negative flag
        void CCF(uint16_t src); //Clear overflow flag
        void SCF(uint16_t src); //Set overflow flag
        void COF(uint16_t src); //Clear carry flag
        void SOF(uint16_t src); //Set carry flag
        void CLF(uint16_t src); //Clear less flag
        void SLF(uint16_t src); //Set less flag
        void CIF(uint16_t src); //Set interrupt flag
        void SIF(uint16_t src); //Clear interrupt flag
        void CEF(uint16_t src); //Clear error flag

        /* 0xB0-0xBB */
        void INC(uint16_t src);   //Increment the data
        void INCR(uint16_t src);  //Increment the register
        void INCX(uint16_t src);  //Increment the 16-bit data
        void INCXR(uint16_t src); //Increment the 16-bit register

        void DEC(uint16_t src);   //Decrement the data
        void DECR(uint16_t src);  //Decrement the register
        void DECX(uint16_t src);  //Decrement the 16-bit data
        void DECXR(uint16_t src); //Decrement the 16-bit register

    public:
        //Public for Debugger Only (Package this up eventually)
        uint16_t pc;  //Program Counter
        uint16_t sp;  //Stack Pointer
        uint8_t r[8]; // General Registers
        bool st[8];   //Status Register
        
        struct Instruction{
            Addressing addressing;
            Opcode opcode;
            const char* mnemonic;
            int bytes;
        };

        Instruction InstructionSet[257];
        
        struct Debug{
            int pc;
            Instruction instruction;
        };

        //Public for System Usage
        cosproc(BusRead r, BusWrite w);
        void reset();
        Debug cycle();
        void LPI();   //Non Maskable Interrupt
        void HPI();   //Send High 
        void execute(Instruction i);
};
