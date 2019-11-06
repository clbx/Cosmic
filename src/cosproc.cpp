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
	InstructionSet[0x04] = (Instruction){&cosproc::IMP,&cosproc::SWP,"SWP",3};
	
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
	InstructionSet[0x1B] = (Instruction){&cosproc::REG,&cosproc::SUBR,"SUB RX",2};
	InstructionSet[0x1C] = (Instruction){&cosproc::IMM,&cosproc::SUBX,"SUBX #oper",3};
	InstructionSet[0x1D] = (Instruction){&cosproc::ABS,&cosproc::SUBX,"SUBX oper",3};
	InstructionSet[0x1E] = (Instruction){&cosproc::IND,&cosproc::SUBX,"SUBX @oper",3};
	InstructionSet[0x1F] = (Instruction){&cosproc::REG,&cosproc::SUBXR,"SUBX RX",2};

	InstructionSet[0x30] = (Instruction){&cosproc::IMM,&cosproc::MOVAI,"MOV #oper, oper",4};
	InstructionSet[0x31] = (Instruction){&cosproc::ABS,&cosproc::MOVA,"MOV oper, oper",5};
	InstructionSet[0x32] = (Instruction){&cosproc::IND,&cosproc::MOVA,"MOV @oper, oper",5};
	InstructionSet[0x33] = (Instruction){&cosproc::REG,&cosproc::MOVAR,"MOV RX, oper",4};
	InstructionSet[0x34] = (Instruction){&cosproc::IMM,&cosproc::MOVII,"MOV #oper, @oper",4};
	InstructionSet[0x35] = (Instruction){&cosproc::ABS,&cosproc::MOVI,"MOV oper, @oper",5};
	InstructionSet[0x36] = (Instruction){&cosproc::IND,&cosproc::MOVI,"MOV @oper, @oper",5};
	InstructionSet[0x37] = (Instruction){&cosproc::REG,&cosproc::MOVIR,"MOV RX, @oper",4};
	InstructionSet[0x38] = (Instruction){&cosproc::IMM,&cosproc::MOVRI,"MOV #oper, RX",3};
	InstructionSet[0x39] = (Instruction){&cosproc::ABS,&cosproc::MOVR,"MOV oper, RX",4};
	InstructionSet[0x3A] = (Instruction){&cosproc::IND,&cosproc::MOVR,"MOV @oper, RX",4};
	InstructionSet[0x3B] = (Instruction){&cosproc::IMM,&cosproc::MOVRR,"MOV RX, RX",3};

	InstructionSet[0x3C] = (Instruction){&cosproc::IMM,&cosproc::SHL,"SHL #oper",2};
	InstructionSet[0x3D] = (Instruction){&cosproc::ABS,&cosproc::SHL,"SHL oper",3};
	InstructionSet[0x3E] = (Instruction){&cosproc::IND,&cosproc::SHL,"SHL @oper",3};
	InstructionSet[0x3F] = (Instruction){&cosproc::REG,&cosproc::SHLR,"SHL RX",2};

	InstructionSet[0x40] = (Instruction){&cosproc::IMM,&cosproc::MOVXAI,"MOVX #oper, oper",5};
	InstructionSet[0x41] = (Instruction){&cosproc::ABS,&cosproc::MOVXA,"MOVX oper, oper",5};
	InstructionSet[0x42] = (Instruction){&cosproc::IND,&cosproc::MOVXA,"MOVX @oper, oper",5};
	InstructionSet[0x43] = (Instruction){&cosproc::REG,&cosproc::MOVXAR,"MOVX RX, oper",4};
	InstructionSet[0x44] = (Instruction){&cosproc::IMM,&cosproc::MOVXII,"MOVX #oper, @oper",5};
	InstructionSet[0x45] = (Instruction){&cosproc::ABS,&cosproc::MOVXI,"MOVX oper, @oper",5};
	InstructionSet[0x46] = (Instruction){&cosproc::IND,&cosproc::MOVXI,"MOVX @oper, @oper",5};
	InstructionSet[0x47] = (Instruction){&cosproc::REG,&cosproc::MOVXIR,"MOVX RX, @oper",4};
	InstructionSet[0x48] = (Instruction){&cosproc::IMM,&cosproc::MOVXRI,"MOVX #oper, RX",4};
	InstructionSet[0x49] = (Instruction){&cosproc::ABS,&cosproc::MOVXR,"MOVX oper, RX",4};
	InstructionSet[0x4A] = (Instruction){&cosproc::IND,&cosproc::MOVXR,"MOVX @oper, RX",4};
	InstructionSet[0x4B] = (Instruction){&cosproc::REG,&cosproc::MOVXRR,"MOVX RX, RX",3};

	InstructionSet[0x4C] = (Instruction){&cosproc::IMM,&cosproc::SHLX,"SHLX #oper",3};
	InstructionSet[0x4D] = (Instruction){&cosproc::ABS,&cosproc::SHLX,"SHLX oper",3};
	InstructionSet[0x4E] = (Instruction){&cosproc::IND,&cosproc::SHLX,"SHLX @oper",3};
	InstructionSet[0x4F] = (Instruction){&cosproc::REG,&cosproc::SHLXR,"SHLX RX",2};

	InstructionSet[0x50] = (Instruction){&cosproc::IMM,&cosproc::AND,"AND #oper",2};
	InstructionSet[0x51] = (Instruction){&cosproc::ABS,&cosproc::AND,"AND oper",3};
	InstructionSet[0x52] = (Instruction){&cosproc::IND,&cosproc::AND,"AND @oper",3};
	InstructionSet[0x53] = (Instruction){&cosproc::REG,&cosproc::ANDR,"AND RX",2};

	InstructionSet[0x54] = (Instruction){&cosproc::IMM,&cosproc::OR,"OR #oper",2};
	InstructionSet[0x55] = (Instruction){&cosproc::ABS,&cosproc::OR,"OR oper",3};
	InstructionSet[0x56] = (Instruction){&cosproc::IND,&cosproc::OR,"OR @oper",3};
	InstructionSet[0x57] = (Instruction){&cosproc::REG,&cosproc::ORR,"OR RX",2};

	InstructionSet[0x58] = (Instruction){&cosproc::IMM,&cosproc::XOR,"XOR #oper",2};
	InstructionSet[0x59] = (Instruction){&cosproc::ABS,&cosproc::XOR,"XOR oper",3};
	InstructionSet[0x5A] = (Instruction){&cosproc::IND,&cosproc::XOR,"XOR @oper",3};
	InstructionSet[0x5B] = (Instruction){&cosproc::REG,&cosproc::XORR,"XOR RX",2};

	InstructionSet[0x5C] = (Instruction){&cosproc::IMM,&cosproc::SHR,"SHR #oper",2};
	InstructionSet[0x5D] = (Instruction){&cosproc::ABS,&cosproc::SHR,"SHR oper",3};
	InstructionSet[0x5E] = (Instruction){&cosproc::IND,&cosproc::SHR,"SHR @oper",3};
	InstructionSet[0x5F] = (Instruction){&cosproc::REG,&cosproc::SHRR,"SHR RX",2};

	InstructionSet[0x60] = (Instruction){&cosproc::IMM,&cosproc::CMP,"CMP #oper",2};
	InstructionSet[0x61] = (Instruction){&cosproc::ABS,&cosproc::CMP,"CMP oper",3};
	InstructionSet[0x62] = (Instruction){&cosproc::IND,&cosproc::CMP,"CMP @oper",3};
	InstructionSet[0x63] = (Instruction){&cosproc::REG,&cosproc::CMPR,"CMP RX",2};
	InstructionSet[0x64] = (Instruction){&cosproc::IMM,&cosproc::CMPX,"CMPX #oper",3};
	InstructionSet[0x65] = (Instruction){&cosproc::ABS,&cosproc::CMPX,"CMPX oper",3};
	InstructionSet[0x66] = (Instruction){&cosproc::IND,&cosproc::CMPX,"CMPX @oper",3};
	InstructionSet[0x67] = (Instruction){&cosproc::REG,&cosproc::CMPXR,"CMPX RX",2};

	InstructionSet[0x68] = (Instruction){&cosproc::IMP,&cosproc::INC,"INC",1};
	InstructionSet[0x69] = (Instruction){&cosproc::IMP,&cosproc::INCX,"INCX",1};
	InstructionSet[0x6A] = (Instruction){&cosproc::IMP,&cosproc::DEC,"DEC",1};
	InstructionSet[0x6B] = (Instruction){&cosproc::IMP,&cosproc::DECX,"DECX",1};

	InstructionSet[0x6C] = (Instruction){&cosproc::IMM,&cosproc::SHRX,"SHRX #oper",3};
	InstructionSet[0x6D] = (Instruction){&cosproc::ABS,&cosproc::SHRX,"SHRX oper",3};
	InstructionSet[0x6E] = (Instruction){&cosproc::IND,&cosproc::SHRX,"SHRX @oper",3};
	InstructionSet[0x6F] = (Instruction){&cosproc::REG,&cosproc::SHRXR,"SHRX RX",2};

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
	uint8_t temp = r[Read(pc+1)];
	r[Read(pc+1)] = r[Read(pc+2)];
	r[Read(pc+2)] = temp;
}

/* 0x10-0x12 ADD */
void cosproc::ADD(uint16_t src){
	uint8_t data = Read(src);

	unsigned int temp = r[0] + data;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF; //TODO: Check for accuracy across functions
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0; //TODO: Check for accuracy across functions
	
	//Set Value
	r[0] = temp & 0xFF;

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x13 ADD */
void cosproc::ADDR(uint16_t src){
	uint8_t data = r[src];

	unsigned int temp = r[0] + data;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x14-0x16 ADDX */
void cosproc::ADDX(uint16_t src){
	uint8_t dataHigh = Read(src);
	uint8_t dataLow = Read(src+1);
	uint16_t data = ((dataHigh << 8) | dataLow);

	unsigned int temp =  ((r[0] << 8) | r[1] ) + data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x17 ADDXR  */
void cosproc::ADDXR(uint16_t src){
	uint16_t data;

	if(src % 2 == 0){
		data = ((r[src] << 8) | r[src+1]);
	}else{
		data = ((r[src-1] << 8) | r[src]);
	}

	unsigned int temp =  ((r[0] << 8) | r[1] ) + data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x18-0x1A SUB */
void cosproc::SUB(uint16_t src){
	uint8_t data = Read(src);
	unsigned int temp = r[0] - data;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x1B SUBR */
void cosproc::SUBR(uint16_t src){
	uint8_t data = r[src];
	unsigned int temp = r[0] - data;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x1C-0x1E SUBX */
void cosproc::SUBX(uint16_t src){
	uint8_t dataHigh = Read(src++);
	uint8_t dataLow = Read(src);

	uint16_t data = ((dataHigh << 8) | dataLow);

	unsigned int temp =  ((r[0] << 8) | r[1] ) - data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x1F SUBXR  */
void cosproc::SUBXR(uint16_t src){
	uint16_t data;

	if(src % 2 == 0){
		data = ((r[src] << 8) | r[src+1]);
	}else{
		data = ((r[src-1] << 8) | r[src]);
	}

	unsigned int temp = ((r[0] << 8) | r[1] ) - data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x30 MOV to Absolute from Immediate */
void cosproc::MOVAI(uint16_t src){
	uint16_t dst = ((Read(pc+2) << 8) | Read(pc+3)); //Get the 16bit destination
	Write(dst,Read(src)); //Write value of memory at destination
}

/* 0x31-0x32 MOV to Absolute from Absolute/Indirect */
void cosproc::MOVA(uint16_t src){
	uint16_t dst = ((Read(pc+3) << 8) | Read(pc+4)); //Get the 16bit destination
	Write(dst,Read(src)); //Write value of memory at destination
}

/* 0x33 MOV to Absolute from Reigster */
void cosproc::MOVAR(uint16_t src){
	uint16_t dst = ((Read(pc+2) << 8) | Read(pc+3)); //Get the 16bit destination
	Write(dst,r[src-1]);  //Write the value of the register to the location
}

/* 0x34 MOV to Indirect from Immediate */
void cosproc::MOVII(uint16_t src){
	uint16_t pre_dst = ((Read(pc+2) << 8) | Read(pc+3)); //Get the 16bit pre-destination
	uint16_t dst = ((Read(pre_dst) << 8) | Read(pre_dst+1)); //Get the 16bit destination
	Write(dst, Read(src));
}

/* 0x35-0x36 MOV to Indirect from Absolute/Indirect */
void cosproc::MOVI(uint16_t src){
	uint16_t pre_dst = ((Read(pc+3) << 8) | Read(pc+4)); //Get the 16bit pre-destination
	uint16_t dst = ((Read(pre_dst) << 8) | Read(pre_dst+1)); //Get the 16bit destination
	Write(dst, Read(src));
}

/* 0x37 MOV to Indirect from Register */
void cosproc::MOVIR(uint16_t src){
	uint16_t pre_dst = ((Read(pc+2) << 8) | Read(pc+3)); //Get the 16bit pre-destination
	uint16_t dst = ((Read(pre_dst) << 8) | Read(pre_dst+1)); //Get the 16bit destination
	Write(dst,r[src]);
}

/* 0x38 MOV to Register from Immediate */
void cosproc::MOVRI(uint16_t src){
	r[Read(src+1)] = Read(src);
}

/* 0x39-0x3A MOV to Register from Absolute/Indirect */
void cosproc:: MOVR(uint16_t src){
	r[Read(pc+3)] = Read(src);
}

/* 0x3B MOV to Register from Register */
void cosproc::MOVRR(uint16_t src){
	r[Read(src+1)] = r[Read(src)];
}

/* 0x3C-0X3E SHL Shift the Accumulator left from Imm/Abs/Ind */
void cosproc::SHL(uint16_t src){

}

/* 0x3F SHL Shift the Accumulator left from register */
void cosproc::SHLR(uint16_t src){

}

/* 0x40 MOVX to Absolute from Immediate */
void cosproc::MOVXAI(uint16_t src){
	uint16_t dst = ((Read(src+2) << 8 | Read(src+3))); //Get the 16bit destination
	Write(dst, Read(src));
	Write(dst+1, Read(src+1));
}

/* 0x41-0x42 MOVX to Absolute from Absolute/Indirect */
void cosproc::MOVXA(uint16_t src){
	uint16_t dst = ((Read(pc+3) << 8 | Read(pc+4))); //Get the 16bit destination
	Write(dst, Read(src));
	Write(dst+1, Read(src+1));
}

/* 0x43 MOVX to Absolute from Register */
void cosproc::MOVXAR(uint16_t src){
	uint16_t dst = ((Read(pc+2) << 8 | Read(pc+3))); //Get the 16bit destination
	Write(dst, r[src]);
	Write(dst+1, r[src+1]);
}

/* 0x44 MOVX to Indirect from Immediate */
void cosproc::MOVXII(uint16_t src){
	uint16_t pre_dst = ((Read(pc+3) << 8) | Read(pc+4)); //Get the 16bit pre-destination
	uint16_t dst = ((Read(pre_dst) << 8) | Read(pre_dst+1)); //Get the 16bit destination
	Write(dst, Read(src));
	Write(dst+1, Read(src+1));
}

/* 0x45-0x46 MOVX to Indirect from Absolute/Indirect */
void cosproc::MOVXI(uint16_t src){
	uint16_t pre_dst = ((Read(pc+3) << 8) | Read(pc+4)); //Get the 16bit pre-destination
	uint16_t dst = ((Read(pre_dst) << 8) | Read(pre_dst+1)); //Get the 16bit destination
	Write(dst, Read(src));
	Write(dst+1, Read(src+1));
}

/* 0x47 MOVX to Indirect from Register */
void cosproc::MOVXIR(uint16_t src){
	uint16_t pre_dst = ((Read(pc+2) << 8) | Read(pc+3)); //Get the 16bit pre-destination
	uint16_t dst = ((Read(pre_dst) << 8) | Read(pre_dst+1)); //Get the 16bit destination
	Write(dst, r[src]);
	Write(dst+1, r[src+1]);
}

/* 0x48 MOVX to Register from Immediate */
void cosproc::MOVXRI(uint16_t src){
	int reg = Read(pc+3);
	r[reg] = Read(src);
	r[reg+1] = Read(src+1);
}

/* 0x49-0x4A MOVX to Register from Absolute/Indirect */
void cosproc::MOVXR(uint16_t src){
	int reg = Read(pc+3);
	r[reg] = Read(src);
	r[reg+1] = Read(src+1);
}

/* 0x4B MOVX to Register from Register */
void cosproc::MOVXRR(uint16_t src){
	int reg = Read(pc+2);
	r[reg] = r[src];
	r[reg+1] = r[src+1];
}

/* 0x4C-0x4E SHLX Shift the 16-bit Accumulator left from Imm/Abs/Ind */
void cosproc::SHLX(uint16_t src){
	
}

/* 0x4F SHLX Shift the 16-bit Accumulator left from register */
void cosproc::SHLXR(uint16_t src){
	
}

/* 0x50-0x52 AND with Accumulator */
void cosproc::AND(uint16_t src){
	r[0] &= Read(src);

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x53 AND with Accumulator from register */
void cosproc::ANDR(uint16_t src){
	r[0] &= r[src];

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x54-0x56 OR with Accumulator */
void cosproc::OR(uint16_t src){
	r[0] |= Read(src);

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x57 OR with Accumulator from register */
void cosproc::ORR(uint16_t src){
	r[0] |= Read(src);

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x58-0x5A XOR with Accumulator */
void cosproc::XOR(uint16_t src){
	r[0] ^= Read(src);

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x5B XOR with Accumulator from register */
void cosproc::XORR(uint16_t src){
	r[0] ^= Read(src);

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x5C-0x5E SHR Shift the Accumulator right from Imm/Abs/Ind */
void cosproc::SHR(uint16_t src){

}

/* 0x5F SHR Shift the Accumulator right from register */
void cosproc::SHRR(uint16_t src){

}

/* 0x60-0x62 CMP Compare with Accumulator */
void cosproc::CMP(uint16_t src){
	
}

/* 0x63 CMPR Compare with Accumulator from register*/
void cosproc::CMPR(uint16_t src){
	
}

/* 0x64-0x66 CMPX Compare with 16-bit Accumulator */
void cosproc::CMPX(uint16_t src){
	
}

/* 0x67 CMPXR Compare with 16-bit Accumulator from register*/
void cosproc::CMPXR(uint16_t src){
	
}

/* 0x68 INC Increment the Accumulator */
void cosproc::INC(uint16_t src){
	unsigned int temp = r[0] + 1;
	
	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(0x01^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x69 INC Increment the 16-bit Accumulator */
void cosproc::INCX(uint16_t src){
	unsigned int temp = ((r[0] << 8) | r[1] ) + 1;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(0x0001^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x6A DEC Decrement the Accumulator */
void cosproc::DEC(uint16_t src){
	unsigned int temp = r[0] - 1;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(0x01^temp)&0x80) != 0;
	
	//Set Value
	r[0] = temp & 0xFF;

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x6B DECX Decrement the 16-bit Accumulator */
void cosproc::DECX(uint16_t src){
	unsigned int temp = ((r[0] << 8) | r[1] ) - 1;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((r[0]^temp)&(0x0001^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x6C-0x6E SHRX Shift the 16-bit Accumulator right from Imm/Abs/Ind */
void cosproc::SHRX(uint16_t src){
	
}

/* 0x6F SHRX Shift the 16-bit Accumulator right from register */
void cosproc::SHRXR(uint16_t src){
	
}