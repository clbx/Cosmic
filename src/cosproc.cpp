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
		InstructionSet[i] = (Instruction){&cosproc::IMP,&cosproc::UND,"Undefined",1};
	}

	//Populate instruction set
	InstructionSet[0x00] = (Instruction){&cosproc::IMP,&cosproc::NOP,"NOP",1};
	InstructionSet[0x01] = (Instruction){&cosproc::IMP,&cosproc::HCF,"HCF",0};
	InstructionSet[0x02] = (Instruction){&cosproc::IMP,&cosproc::PUSH,"PUSH",1};
	InstructionSet[0x03] = (Instruction){&cosproc::IMP,&cosproc::POP,"POP",1};
	InstructionSet[0x04] = (Instruction){&cosproc::IMP,&cosproc::SWP,"SWP",3}; //On the chopping block
	
	InstructionSet[0x10] = (Instruction){&cosproc::IMM,&cosproc::ADD,"ADD #oper",2};
	InstructionSet[0x11] = (Instruction){&cosproc::ABS,&cosproc::ADD,"ADD oper",3};
	InstructionSet[0x12] = (Instruction){&cosproc::IND,&cosproc::ADD,"ADD @oper",3};
	InstructionSet[0x13] = (Instruction){&cosproc::REG,&cosproc::ADDR,"ADD RX",2};
	InstructionSet[0x14] = (Instruction){&cosproc::IMM,&cosproc::ADDX,"ADDX #oper",3};
	InstructionSet[0x15] = (Instruction){&cosproc::ABS,&cosproc::ADDX,"ADDX oper",3};
	InstructionSet[0x16] = (Instruction){&cosproc::IND,&cosproc::ADDX,"ADDX @oper",3};
	InstructionSet[0x17] = (Instruction){&cosproc::REG,&cosproc::ADDXR,"ADDX RX",2};

	InstructionSet[0x18] = (Instruction){&cosproc::IMM,&cosproc::SUB,"SUB #oper",2};
	InstructionSet[0x19] = (Instruction){&cosproc::ABS,&cosproc::SUB,"SUB oper",3};
	InstructionSet[0x1A] = (Instruction){&cosproc::IND,&cosproc::SUB,"SUB @oper",3};
	// InstructionSet[0x1B] = (Instruction){&cosproc::REG,&cosproc::SUBR,"SUB RX",2};
	// InstructionSet[0x1C] = (Instruction){&cosproc::IMM,&cosproc::SUBX,"SUBX #oper",3};
	// InstructionSet[0x1D] = (Instruction){&cosproc::ABS,&cosproc::SUBX,"SUBX oper",3};
	// InstructionSet[0x1E] = (Instruction){&cosproc::IND,&cosproc::SUBX,"SUBX @oper",3};
	// InstructionSet[0x1F] = (Instruction){&cosproc::REG,&cosproc::SUBXR,"SUBX RX",2};


	InstructionSet[0x30] = (Instruction){&cosproc::IMM,&cosproc::MOVAI,"MOV #oper, oper",4};
	InstructionSet[0x31] = (Instruction){&cosproc::ABS,&cosproc::MOVA,"MOV oper, oper",5};
	InstructionSet[0x32] = (Instruction){&cosproc::IND,&cosproc::MOVA,"MOV @oper, oper",5};
	InstructionSet[0x33] = (Instruction){&cosproc::REG,&cosproc::MOVAR,"MOV RX, oper",4};

	/*
	InstructionSet[0x34] = (Instruction){&cosproc::IMM,&cosproc::MOVII,"MOV #oper, @oper",4};
	InstructionSet[0x35] = (Instruction){&cosproc::ABS,&cosproc::MOVI,"MOV oper, @oper",5};
	InstructionSet[0x36] = (Instruction){&cosproc::IND,&cosproc::MOVI,"MOV @oper, @oper",5};
	InstructionSet[0x37] = (Instruction){&cosproc::REG,&cosproc::MOVIR,"MOV RX, @oper",4};
	*/


	reset();

}

void cosproc::reset()
{
	memset(r,0,sizeof(r));
	pc = 0;
    sp = 255;
    memset(st,0,sizeof(st));

	//TODO: Replace me with something a little less dangerous
	printf("\033[2J\033[1;1H\n");

	return;
}


void cosproc::cycle()
{
	
	uint8_t opcode = Read(pc); //Fetch
	Instruction currentInstruction = InstructionSet[opcode]; //Decode
	execute(currentInstruction); //Execute
	printf("%s\n",currentInstruction.mnemonic); //Write Debug
	pc += currentInstruction.bytes; //Writeback

}

void cosproc::execute(Instruction i)
{
	printf("Addressing:\n");
	uint16_t src = (this->*i.addressing)();
	printf("Execution:\n");
	(this->*i.opcode)(src);
}



/** -= ADDRESSING MODES =- **/

uint16_t cosproc::IMP()
{
	return 0;
}

uint16_t cosproc::IMM(){
	return pc + 1;
}

uint16_t cosproc::ABS(){
	uint16_t val =  (Read(pc+1) << 8 | Read(pc+2)); //Return 16bit address of where to look at data
	return val;
}

uint16_t cosproc::IND(){
	uint16_t srcHigh = Read(pc+1) << 8 | Read(pc+2);
	uint16_t srcLow = srcHigh+1;
	uint16_t val = (Read(srcHigh) << 8 | Read(srcLow));
	return val;

}

uint16_t cosproc::REG(){
	return Read(pc + 1);
}

/** -= OPCODES =- **/
/* Undefined Opcode */
void cosproc::UND(uint16_t src){
}

/* 0x00 NOP */
void cosproc::NOP(uint16_t src){
}

/* 0x01 HCF */
void cosproc::HCF(uint16_t src){
}

/* 0x02 PUSH */
void cosproc::PUSH(uint16_t src){
	Write(sp,r[0]);
	sp--;
}

/* 0x03 POP */
void cosproc::POP(uint16_t src){
	r[0] = Read(sp);
	sp ++;
}

/* 0x04 SWP*/
void cosproc::SWP(uint16_t src){
	//fill
}


//TOOD: Less code reuse
/* 0x10-0x12 ADD */
void cosproc::ADD(uint16_t src){
	uint8_t data = Read(src);

	unsigned int temp = r[0] + data;

	//TODO: Find a better way to do this. 
	//Set Zero
	st[0] = temp == 0;
	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

}

/* 0x13 ADD */
void cosproc::ADDR(uint16_t src){
	uint8_t data = r[src];

	unsigned int temp = r[0] + data;

	//Set Zero
	st[0] = temp == 0;
	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;


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
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;
	//Set Value
	r[0] = temp & 0xFF00 >> 8;
	r[1] = temp & 0x00FF;


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
		st[1] = temp >= 0x8000;
		//Set Carry
		st[2] = temp > 0xFFFF;
		//Set Overflow
		st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;
		//Set Value
		r[0] = temp & 0xFF00 >> 8;
		r[1] = temp & 0x00FF;

	}

}

/* 0x18-0x1F SUB */
void cosproc::SUB(uint16_t src){
	uint8_t data = Read(src);

	unsigned int temp = r[0] - data;

	//Set Zero
	st[0] = temp == 0;
	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

}

/* 0x30 MOV to Absolute from Immediate */
void cosproc::MOVAI(uint16_t src){
	uint16_t dst = ((Read(pc+2) << 8) | Read(pc+3)); //Get destination
	Write(dst,Read(src)); //Write value of memory at destination
}

/* 0x31-0x32 MOV to Absolute from Absolute/Indirect*/
void cosproc::MOVA(uint16_t src){
	uint16_t dst = ((Read(pc+3) << 8) | Read(pc+4)); //Get destination
	Write(dst,Read(src)); //Write value of memory at destination
}

/* 0x33 MOV to Absolute from Reigster */
void cosproc::MOVAR(uint16_t src){
	uint16_t dst = ((Read(pc+2) << 8) | Read(pc+3)); //Get the destination in a 16bit
	Write(dst,r[src-1]);  //Write the value of the register to the location
}

