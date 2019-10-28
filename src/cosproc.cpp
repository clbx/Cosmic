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

	//Fill InstructionSet with Undefined Opcodes so it don't crash no mo'
	for(int i = 0;i < 0xFF; i++){
		InstructionSet[i] = (Instruction){&cosproc::IMP,&cosproc::UND};
	}

	//Populate instruction set
	InstructionSet[0x00] = (Instruction){&cosproc::IMP,&cosproc::NOP};
	InstructionSet[0x01] = (Instruction){&cosproc::IMP,&cosproc::HCF};
	InstructionSet[0x02] = (Instruction){&cosproc::IMP,&cosproc::PUSH};
	InstructionSet[0x03] = (Instruction){&cosproc::IMP,&cosproc::POP};
	InstructionSet[0x04] = (Instruction){&cosproc::IMP,&cosproc::SWP};
	
	InstructionSet[0x10] = (Instruction){&cosproc::IMM,&cosproc::ADD};
	InstructionSet[0x11] = (Instruction){&cosproc::ABS,&cosproc::ADD};
	InstructionSet[0x12] = (Instruction){&cosproc::IND,&cosproc::ADD};
	InstructionSet[0x13] = (Instruction){&cosproc::REG,&cosproc::ADDR};
	InstructionSet[0x14] = (Instruction){&cosproc::IMM,&cosproc::ADDX};
	InstructionSet[0x15] = (Instruction){&cosproc::ABS,&cosproc::ADDX};
	InstructionSet[0x16] = (Instruction){&cosproc::IND,&cosproc::ADDX};
	InstructionSet[0x17] = (Instruction){&cosproc::REG,&cosproc::ADDXR};









	reset();

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

uint16_t cosproc::ABS(){
	printf("Absolute ");
	//TOOD: Make me pretty
	uint16_t val =  (Read(pc) << 8 | Read(pc + 1)); //Return 16bit address of where to look at data
	pc += 2;
	return val;
}

uint16_t cosproc::IND(){
	printf("Indirect");
	//TOOD: Make me pretty too
	uint16_t val = Read(Read(pc) << 8 | Read(pc + 1));
	pc += 2; 
	return val;

}

uint16_t cosproc::REG(){
	printf("Register");
	return Read(++pc);
}

/** -= OPCODES =- **/
/* Undefined Opcode */
void cosproc::UND(uint16_t src){
	printf("Unknown Opcode Called!\n");
	pc++;
}

/* 0x00 NOP */
void cosproc::NOP(uint16_t src){
	printf("NOP\n");
	pc++;
    return;
}

/* 0x01 HCF */
void cosproc::HCF(uint16_t src){
	printf("HCF\n");
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


//TOOD: Less code reuse
/* 0x10-0x12 ADD */
void cosproc::ADD(uint16_t src){
	printf("ADD\n");
	uint8_t data = Read(src);

	unsigned int temp = r[0] + data;

	//TODO: Find a better way to do this. 
	//Set Zero
	st[0] = temp == 0;
	//Set Negative
	st[1] = temp > 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

	pc++;
}

/* 0x13 ADD */
void cosproc::ADDR(uint16_t src){
	printf("ADDR\n");
	uint8_t data = r[src];

	unsigned int temp = r[0] + data;

	//Set Zero
	st[0] = temp == 0;
	//Set Negative
	st[1] = temp > 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

	pc++;

}

/* 0x14-0x16 ADDX */
void cosproc::ADDX(uint16_t src){
	uint8_t dataHigh = Read(src);
	uint8_t dataLow = Read(src+1);


	uint16_t data = ((dataHigh << 8) | dataLow);


	unsigned int temp =  ((r[0] << 8) | r[1] ) + data;

	//Set Zero
	st[0] = temp == 0;
	//Set Negative
	st[1] = temp > 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;
	//Set Value
	r[0] = temp & 0xFF00 >> 8;
	r[1] = temp & 0x00FF;

	pc+=3;

}

/* 0x17 ADDXR  */
void cosproc::ADDXR(uint16_t src){

	//TODO: FIX ME IM GARBAGE
	int reg = 0; 
	switch(src){
		case 0:
		case 1:
			reg = 0; break;
		case 2:
		case 3:
			reg = 1; break;
		case 4:
		case 5:
			reg = 2; break;
		case 6:
		case 7:
			reg = 3; break;

		uint8_t dataHigh = r[reg*2];
		uint8_t dataLow = r[reg*2+1];
		uint16_t data = ((dataHigh << 8) | dataLow);

		unsigned int temp =  ((r[0] << 8) | r[1] ) + data;

		//Set Zero
		st[0] = temp == 0;
		//Set Negative
		st[1] = temp > 0x8000;
		//Set Carry
		st[2] = temp > 0xFFFF;
		//Set Overflow
		st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;
		//Set Value
		r[0] = temp & 0xFF00 >> 8;
		r[1] = temp & 0x00FF;

		pc+=2;
	}

}

