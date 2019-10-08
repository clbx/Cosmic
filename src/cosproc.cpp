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
	instruction.addressing = &cosproc::immediate;
	instruction.opcode = &cosproc::NOP;
	InstructionSet[0x00] = instruction;

	reset();

	return;
}

void cosproc::reset()
{
	memset(r,0,sizeof(r));
	pc = 0;
    sp = 0;
    st = 0;

	return;
}


void cosproc::run(uint32_t n)
{
	uint8_t opcode;
	Instruction currentInstruction;

	while(n-- > 0)
	{
		// fetch
        printf("fetch\n");
		opcode = Read(pc);
        printf("decode\n");
		// decode
		currentInstruction = InstructionSet[opcode];
        printf("execute\n");
		// execute
		execute(currentInstruction);
	}
}

void cosproc::execute(Instruction i)
{
	uint16_t src = (this->*i.addressing)();
	(this->*i.opcode)(src);
}

/** -= ADDRESSING MODES =- **/

uint16_t cosproc::immediate()
{
    printf("Immediate Addressing\n");
	return pc;
}

/** -= OPCODES =- **/

/**
 * 0x00 NOP
 * Immeditate
 * 1 byte
 */
void cosproc::NOP(uint16_t src)
{
	printf("NOP\n");
    return;
}
