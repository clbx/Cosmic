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

	instruction.addressing = &cosproc::IMM;
	instruction.opcode = &cosproc::ADD;
	InstructionSet[0x10] = instruction;





	reset();

	return;
}

void cosproc::reset()
{
	memset(r,0,sizeof(r));
	pc = 0;
    sp = 255;
    memset(st,0,sizeof(st));

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

uint16_t cosproc::IMM(){
	printf("Immediate ");
	return ++pc;
}

/** -= OPCODES =- **/

/* 0x00 NOP */
void cosproc::NOP(uint16_t src){
	printf("NOP\n");
	pc++;
    return;
}

/* 0x01 HCF */
void cosproc::HCF(uint16_t src){
	printf("HCF\n");
	return;
}

/* 0x02 PUSH */
void cosproc::PUSH(uint16_t src){
	Write(sp,r[0]);
	sp--;
	pc++;
}

/* 0x03 POP */
void cosproc::POP(uint16_t src){
	r[0] = Read(sp);
	sp ++;
	pc++;
}

/* 0x04 SWP*/
void cosproc::SWP(uint16_t src){
	//fill
}



/* 0x10-0x12 ADD */
void cosproc::ADD(uint16_t src){
	printf("ADD\n");
	uint8_t data = Read(src);
	unsigned int temp = r[0] + data;
	if(temp == 0){
		st[0] = true;
	}
	
	if(temp > 0x80){
		st[1] = true;
	}

	//set overflow

	if(temp > 0xFF){
		st[2] = true;
	}

	r[0] = temp & 0xFF;

	pc++;
}

/* 0x13 ADD */
void cosproc::ADDR(uint16_t src){

}

/* 0x14-0x16 ADDX */
void cosproc::ADDX(uint16_t src){

}

/* 0x17 ADDX  */
void cosproc::ADDXR(uint16_t src){

}

