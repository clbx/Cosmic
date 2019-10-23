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
	Instruction instruction;


	//Populate instruction set
	instruction.addressing = &cosproc::IMP;
	instruction.opcode = &cosproc::NOP;
	InstructionSet[0x00] = instruction;
	instruction.addressing = &cosproc::IMP;
	instruction.opcode = &cosproc::HCF;
	InstructionSet[0x01] = instruction;
	instruction.addressing = &cosproc::IMP;
	instruction.opcode = &cosproc::PUSH;
	InstructionSet[0x02] = instruction;
	instruction.addressing = &cosproc::IMP;
	instruction.opcode = &cosproc::POP;
	InstructionSet[0x03] = instruction;

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
	Instruction currentInstruction = InstructionSet[opcode]; //Decode
	execute(currentInstruction); //Execute
}

void cosproc::execute(Instruction i)
{
	uint16_t src = (this->*i.addressing)();
	(this->*i.opcode)(src);
}

/** -= ADDRESSING MODES =- **/

uint16_t cosproc::IMP()
{
    printf("Implied ");
	return 0;
}

/** -= OPCODES =- **/

/**
 * 0x00 NOP
 * Immeditate
 * 1 byte
 * 
 * No Operation
 */
void cosproc::NOP(uint16_t src){
	printf("NOP\n");
	pc++;
    return;
}

/**
 * 0x01 HCF
 * Immediate
 * 1 Byte
 * 
 * Halt and Catch Fire
 * Stops execution
 */
void cosproc::HCF(uint16_t src){
	printf("HCF\n");
	return;
}

/**
 * 0x02 PUSH
 * Immediate
 * 1 Byte
 * 
 * Pushes the Accumulator to the Stack
 */
void cosproc::PUSH(uint16_t src){
	Write(sp,r[0]);
	sp--;
	pc++;
}

/** 0x03 POP
 * Immediate
 * 1 Byte
 * 
 * Pops the stack to the Accumulator
 */
void cosproc::POP(uint16_t src){
	r[0] = Read(sp);
	sp ++;
	pc++;
}

/** 0x04 SWP
 * Register
 * 2 Bytes  OP Regsiter
 * 
 * Swaps
 */
void cosproc::SWP(uint16_t src){
	//fill
}
