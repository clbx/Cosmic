/**
 * @file cosproc.cpp
 * 
 * 
 * 
 * 
 */

#include "cosproc.hpp"

cosproc::cosproc(BusRead r, BusWrite w)
{
	Write = (BusWrite)w;
	Read = (BusRead)r;

	//Populate instruction set
	InstructionSet[0x00] = &cosproc::NOP;
	InstructionSet[0x01] = &cosproc::HCF;
	InstructionSet[0x02] = &cosproc::PUSH;
	InstructionSet[0x03] = &cosproc::POP;

	reset();

	return;
}

void cosproc::reset()
{
	memset(r,0,sizeof(r));
	pc = 0;
    sp = 255;
    st = 0;

	return;
}


void cosproc::cycle()
{
	uint8_t opcode = Read(pc); //Fetch
	Opcode currentInstruction = InstructionSet[opcode]; //Decode
	(this->*currentInstruction)(); //Execute
}

/** -= OPCODES =- **/

/**
 * 0x00 NOP
 * 1 byte
 * 
 * No Operation
 */
void cosproc::NOP(){
	pc++;
}

/**
 * 0x01 HCF
 * 1 Byte
 * 
 * Halt and Catch Fire
 * Stops execution
 */
void cosproc::HCF(){
	//fill
}

/**
 * 0x02 PUSH
 * 1 byte
 * 
 * Pushes the Accumulator to the Stack
 */
void cosproc::PUSH(){
	//fill
}

/** 0x03 POP
 * 
 * Pops the stack to the Accumulator
 */
void cosproc::POP(){
	//fill
}

/** SWP
 * 2 Bytes: OP Regsiter
 * 
 * Swaps
 */
void cosproc::SWP(){
	//fill
}