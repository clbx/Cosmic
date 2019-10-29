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


	InstructionSet[0x30] = (Instruction){&cosproc::IMM,&cosproc::MOVA,"MOV #oper, oper",4};
	InstructionSet[0x31] = (Instruction){&cosproc::ABS,&cosproc::MOVA,"MOV oper, oper",5};
	InstructionSet[0x32] = (Instruction){&cosproc::IND,&cosproc::MOVA,"MOV @oper, oper",5};
	InstructionSet[0x33] = (Instruction){&cosproc::REG,&cosproc::MOVAR,"MOV RX, oper",4};
	InstructionSet[0x34] = (Instruction){&cosproc::IMM,&cosproc::MOVI,"MOV #oper, @oper",5};
	InstructionSet[0x35] = (Instruction){&cosproc::ABS,&cosproc::MOVI,"MOV oper, @oper",5};
	InstructionSet[0x36] = (Instruction){&cosproc::IND,&cosproc::MOVI,"MOV @oper, @oper",5};
	InstructionSet[0x37] = (Instruction){&cosproc::REG,&cosproc::MOVIR,"MOV RX, @oper",4};
	InstructionSet[0x38] = (Instruction){&cosproc::IMM,&cosproc::MOVR,"MOV #oper, RX",3};
	InstructionSet[0x39] = (Instruction){&cosproc::ABS,&cosproc::MOVR,"MOV oper, RX",4};
	InstructionSet[0x3A] = (Instruction){&cosproc::IND,&cosproc::MOVR,"MOV @oper, RX",4};
	InstructionSet[0x3B] = (Instruction){&cosproc::REG,&cosproc::MOVRR,"MOV RX, RX",3};

	InstructionSet[0x40] = (Instruction){&cosproc::IMM,&cosproc::MOVAX,"MOVX #oper, oper",5};
	InstructionSet[0x41] = (Instruction){&cosproc::ABS,&cosproc::MOVAX,"MOVX oper, oper",5};
	InstructionSet[0x42] = (Instruction){&cosproc::IND,&cosproc::MOVAX,"MOVX @oper, oper",5};
	InstructionSet[0x43] = (Instruction){&cosproc::REG,&cosproc::MOVAXR,"MOVX RX, oper",4};
	InstructionSet[0x44] = (Instruction){&cosproc::IMM,&cosproc::MOVIX,"MOVX #oper, @oper",5};
	InstructionSet[0x45] = (Instruction){&cosproc::ABS,&cosproc::MOVIX,"MOVX oper, @oper",5};
	InstructionSet[0x46] = (Instruction){&cosproc::IND,&cosproc::MOVIX,"MOVX @oper, @oper",5};
	InstructionSet[0x47] = (Instruction){&cosproc::REG,&cosproc::MOVIXR,"MOVX RX, @oper",5};
	InstructionSet[0x48] = (Instruction){&cosproc::IMM,&cosproc::MOVXR,"MOVX #oper, RX",4};
	InstructionSet[0x49] = (Instruction){&cosproc::ABS,&cosproc::MOVXR,"MOVX oper, RX",4};
	InstructionSet[0x4A] = (Instruction){&cosproc::IND,&cosproc::MOVXR,"MOVX @oper, RX",4};
	InstructionSet[0x4B] = (Instruction){&cosproc::REG,&cosproc::MOVXRR,"MOVX RX, RX",3};










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
	uint16_t src = (this->*i.addressing)();
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
	//TOOD: Make me pretty
	uint16_t val =  (Read(pc+1) << 8 | Read(pc+2)); //Return 16bit address of where to look at data
	return val;
}

uint16_t cosproc::IND(){
	//TOOD: Make me pretty too
	uint16_t val = Read(Read(pc+1) << 8 | Read(pc+2));
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
	st[1] = temp > 0x80;
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
	st[1] = temp > 0x80;
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
	st[1] = temp > 0x8000;
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
		st[1] = temp > 0x8000;
		//Set Carry
		st[2] = temp > 0xFFFF;
		//Set Overflow
		st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;
		//Set Value
		r[0] = temp & 0xFF00 >> 8;
		r[1] = temp & 0x00FF;

	}

}


/* 0x30-0x32 MOV Absolute */
void cosproc::MOVA(uint16_t src){

}

/* 0x33 MOV Absolute from Reigster */
void cosproc::MOVAR(uint16_t src){

}

/* 0x34-0x36 MOV Indirect */
void cosproc::MOVI(uint16_t src){

}

/* 0x37 MOV Indrecit from Register */
void cosproc::MOVIR(uint16_t src){

}

/* 0x38-0x3A MOV Register */
void cosproc::MOVR(uint16_t src){

}

/* 0x3B MOV Register from Register */
void cosproc::MOVRR(uint16_t src){

}

/* 0x40-0x42 MOV 16 Bit Absolute */
void cosproc::MOVAX(uint16_t src){

}

/*0x43 MOV 16 Bit Absolute from Register*/
void cosproc::MOVAXR(uint16_t src){

}

/*0x44-0x46 MOV 16 Bit Indirect */
void cosproc::MOVIX(uint16_t src){

}

/*0x47 MOV 16 Bit Indirect from Reigster*/
void cosproc::MOVIXR(uint16_t src){

}

/*0x48-0x4A MOV 16 Bit Register */
void cosproc::MOVXR(uint16_t src){

}

/*0x4B MOV 16 Bit Register from Register */
void cosproc::MOVXRR(uint16_t src){

}


