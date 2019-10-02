/**
 * @file proc.cpp
 * 
 * 
 * 
 * 
 */

#include "proc.h"

proc::proc(BusRead r, BusWrite w)
{
	Write = (BusWrite)w;
	Read = (BusRead)r;
	Instruction instruction;

	//Populate instruction set
	instruction.addressing = &proc::immediate;
	instruction.opcode = &proc::NOP;
	InstructionSet[0x00] = instruction;

	reset();

	return;
}

void proc::reset()
{
	memset(r,0,sizeof(r));
	pc = 0;
    sp = 0;
    st = 0;

	return;
}


void proc::run(uint32_t n)
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

void proc::execute(Instruction i)
{
	uint16_t src = (this->*i.addressing)();
	(this->*i.opcode)(src);
}

/** -= ADDRESSING MODES =- **/

uint16_t proc::immediate()
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
void proc::NOP(uint16_t src)
{
	printf("NOP\n");
    return;
}
