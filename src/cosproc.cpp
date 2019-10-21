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
	execute(currentInstruction); //Execute
}

void cosproc::execute(Opcode i)
{
	(this->*i)();
}

/** -= OPCODES =- **/

/**
 * 0x00 NOP
 * 1 byte
 * 
 * No Operation
 */
void cosproc::NOP(){
	printf("NOP\n");
	pc++;
    return;
}

/**
 * 0x01 HCF
 * 1 Byte
 * 
 * Halt and Catch Fire
 * Stops execution
 */
void cosproc::HCF(){
	printf("HCF\n");
	return;
}

/**
 * 0x02 PUSH
 * 1 byte
 * 
 * Pushes the Accumulator to the Stack
 */
void cosproc::PUSH(){
	Write(sp,r[0]);
	sp--;
	pc++;
}

/** 0x03 POP
 * 
 * Pops the stack to the Accumulator
 */
void cosproc::POP(){
	r[0] = Read(sp);
	sp ++;
	pc++;
}

/** SWP
 * 2 Bytes: OP Regsiter
 * 
 * Swaps
 */
void cosproc::SWP(){
	//fill
}
