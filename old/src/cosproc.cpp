/**
 * @file cosproc.cpp
 * 
 * @author Clay Buxton  (clbx, buxtonc@etown.edu)
 * @author Kevin Carman (carmank, carmank@etown.edu)
 * 
 */

#include "cosproc.hpp"

cosproc::cosproc(BusRead r, BusWrite w)
{
	Write = (BusWrite)w;
	Read = (BusRead)r;

	//Fill InstructionSet with Undefined Opcodes so it don't crash no mo'
	for(int i = 0;i <= 0xFF; i++){
		InstructionSet[i] = (Instruction){&cosproc::IMP,&cosproc::UND,"Undefined",1};
	}

	//Populate instruction set
	InstructionSet[0x00] = (Instruction){&cosproc::IMP,&cosproc::NOP,"NOP",1};
	InstructionSet[0x01] = (Instruction){&cosproc::IMP,&cosproc::HCF,"HCF",0};
	InstructionSet[0x02] = (Instruction){&cosproc::IMP,&cosproc::PUSH,"PUSH",1};
	InstructionSet[0x03] = (Instruction){&cosproc::IMP,&cosproc::POP,"POP",1};
	InstructionSet[0x04] = (Instruction){&cosproc::IMP,&cosproc::SWP,"SWP",3};

	//The program counter should not increment after a call instruction to avoid overstepping the program counter.
	InstructionSet[0x05] = (Instruction){&cosproc::IMM,&cosproc::CALL,"CALL #oper",0};
	InstructionSet[0x06] = (Instruction){&cosproc::ABS,&cosproc::CALL,"CALL oper",0};
	InstructionSet[0x07] = (Instruction){&cosproc::IND,&cosproc::CALL,"CALL @oper",0};
	//Return move the program counter by 3 bytes to step over the call instruction.
	InstructionSet[0x08] = (Instruction){&cosproc::IMP,&cosproc::RET,"RET",3};
	
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

	InstructionSet[0x20] = (Instruction){&cosproc::IMM,&cosproc::MUL,"MUL #oper",2};
	InstructionSet[0x21] = (Instruction){&cosproc::ABS,&cosproc::MUL,"MUL oper",3};
	InstructionSet[0x22] = (Instruction){&cosproc::IND,&cosproc::MUL,"MUL @oper",3};
	InstructionSet[0x23] = (Instruction){&cosproc::REG,&cosproc::MULR,"MUL RX",2};
	InstructionSet[0x24] = (Instruction){&cosproc::IMM,&cosproc::MULX,"MULX #oper",3};
	InstructionSet[0x25] = (Instruction){&cosproc::ABS,&cosproc::MULX,"MULX oper",3};
	InstructionSet[0x26] = (Instruction){&cosproc::IND,&cosproc::MULX,"MULX @oper",3};
	InstructionSet[0x27] = (Instruction){&cosproc::REG,&cosproc::MULXR,"MULX RX",2};

	InstructionSet[0x28] = (Instruction){&cosproc::IMM,&cosproc::DIV,"DIV #oper",2};
	InstructionSet[0x29] = (Instruction){&cosproc::ABS,&cosproc::DIV,"DIV oper",3};
	InstructionSet[0x2A] = (Instruction){&cosproc::IND,&cosproc::DIV,"DIV @oper",3};
	InstructionSet[0x2B] = (Instruction){&cosproc::REG,&cosproc::DIVR,"DIV RX",2};
	InstructionSet[0x2C] = (Instruction){&cosproc::IMM,&cosproc::DIVX,"DIVX #oper",3};
	InstructionSet[0x2D] = (Instruction){&cosproc::ABS,&cosproc::DIVX,"DIVX oper",3};
	InstructionSet[0x2E] = (Instruction){&cosproc::IND,&cosproc::DIVX,"DIVX @oper",3};
	InstructionSet[0x2F] = (Instruction){&cosproc::REG,&cosproc::DIVXR,"DIVX RX",2};

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
	InstructionSet[0x3B] = (Instruction){&cosproc::REG,&cosproc::MOVRR,"MOV RX, RX",3};

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

	InstructionSet[0x6C] = (Instruction){&cosproc::IMM,&cosproc::SHRX,"SHRX #oper",3};
	InstructionSet[0x6D] = (Instruction){&cosproc::ABS,&cosproc::SHRX,"SHRX oper",3};
	InstructionSet[0x6E] = (Instruction){&cosproc::IND,&cosproc::SHRX,"SHRX @oper",3};
	InstructionSet[0x6F] = (Instruction){&cosproc::REG,&cosproc::SHRXR,"SHRX RX",2};

	//The program counter should not increment after a jump instruction to avoid overstepping the program counter.
	InstructionSet[0x70] = (Instruction){&cosproc::IMM,&cosproc::JMP,"JMP #oper",0};
	InstructionSet[0x71] = (Instruction){&cosproc::ABS,&cosproc::JMP,"JMP oper",0};
	InstructionSet[0x72] = (Instruction){&cosproc::IND,&cosproc::JMP,"JMP @oper",0};
	InstructionSet[0x73] = (Instruction){&cosproc::REG,&cosproc::JMPR,"JMP RX",0};

	InstructionSet[0x74] = (Instruction){&cosproc::IMM,&cosproc::JZS,"JZS #oper",0};
	InstructionSet[0x75] = (Instruction){&cosproc::ABS,&cosproc::JZS,"JZS oper",0};
	InstructionSet[0x76] = (Instruction){&cosproc::IND,&cosproc::JZS,"JZS @oper",0};
	InstructionSet[0x77] = (Instruction){&cosproc::REG,&cosproc::JZSR,"JZS RX",0};

	InstructionSet[0x78] = (Instruction){&cosproc::IMM,&cosproc::JNZ,"JNZ #oper",0};
	InstructionSet[0x79] = (Instruction){&cosproc::ABS,&cosproc::JNZ,"JNZ oper",0};
	InstructionSet[0x7A] = (Instruction){&cosproc::IND,&cosproc::JNZ,"JNZ @oper",0};
	InstructionSet[0x7B] = (Instruction){&cosproc::REG,&cosproc::JNZR,"JMP RX",0};

	InstructionSet[0x7C] = (Instruction){&cosproc::IMM,&cosproc::JCS,"JCS #oper",0};
	InstructionSet[0x7D] = (Instruction){&cosproc::ABS,&cosproc::JCS,"JCS oper",0};
	InstructionSet[0x7E] = (Instruction){&cosproc::IND,&cosproc::JCS,"JCS @oper",0};
	InstructionSet[0x7F] = (Instruction){&cosproc::REG,&cosproc::JCSR,"JCS RX",0};

	InstructionSet[0x80] = (Instruction){&cosproc::IMM,&cosproc::JNC,"JNC #oper",0};
	InstructionSet[0x81] = (Instruction){&cosproc::ABS,&cosproc::JNC,"JNC oper",0};
	InstructionSet[0x82] = (Instruction){&cosproc::IND,&cosproc::JNC,"JNC @oper",0};
	InstructionSet[0x83] = (Instruction){&cosproc::REG,&cosproc::JNCR,"JNC RX",0};

	InstructionSet[0x84] = (Instruction){&cosproc::IMM,&cosproc::JOS,"JOS #oper",0};
	InstructionSet[0x85] = (Instruction){&cosproc::ABS,&cosproc::JOS,"JOS oper",0};
	InstructionSet[0x86] = (Instruction){&cosproc::IND,&cosproc::JOS,"JOS @oper",0};
	InstructionSet[0x87] = (Instruction){&cosproc::REG,&cosproc::JOSR,"JOS RX",0};

	InstructionSet[0x88] = (Instruction){&cosproc::IMM,&cosproc::JNO,"JNO #oper",0};
	InstructionSet[0x89] = (Instruction){&cosproc::ABS,&cosproc::JNO,"JNO #oper",0};
	InstructionSet[0x8A] = (Instruction){&cosproc::IND,&cosproc::JNO,"JNO #oper",0};
	InstructionSet[0x8B] = (Instruction){&cosproc::REG,&cosproc::JNOR,"JNO #oper",0};

	InstructionSet[0x8C] = (Instruction){&cosproc::IMM,&cosproc::JNS,"JNS #oper",0};
	InstructionSet[0x8D] = (Instruction){&cosproc::ABS,&cosproc::JNS,"JNS oper",0};
	InstructionSet[0x8E] = (Instruction){&cosproc::IND,&cosproc::JNS,"JNS @oper",0};
	InstructionSet[0x8F] = (Instruction){&cosproc::REG,&cosproc::JNSR,"JNS RX",0};

	InstructionSet[0x90] = (Instruction){&cosproc::IMM,&cosproc::JNN,"JNN #oper",0};
	InstructionSet[0x91] = (Instruction){&cosproc::ABS,&cosproc::JNN,"JNN oper",0};
	InstructionSet[0x92] = (Instruction){&cosproc::IND,&cosproc::JNN,"JNN @oper",0};
	InstructionSet[0x93] = (Instruction){&cosproc::REG,&cosproc::JNNR,"JNN RX",0};

	InstructionSet[0x94] = (Instruction){&cosproc::IMM,&cosproc::JLS,"JLS #oper",0};
	InstructionSet[0x95] = (Instruction){&cosproc::ABS,&cosproc::JLS,"JLS oper",0};
	InstructionSet[0x96] = (Instruction){&cosproc::IND,&cosproc::JLS,"JLS @oper",0};
	InstructionSet[0x97] = (Instruction){&cosproc::REG,&cosproc::JLSR,"JLS RX",0};

	InstructionSet[0x98] = (Instruction){&cosproc::IMM,&cosproc::JNL,"JNL #oper",0};
	InstructionSet[0x99] = (Instruction){&cosproc::ABS,&cosproc::JNL,"JNL oper",0};
	InstructionSet[0x9A] = (Instruction){&cosproc::IND,&cosproc::JNL,"JNL @oper",0};
	InstructionSet[0x9B] = (Instruction){&cosproc::REG,&cosproc::JNLR,"JNL RX",0};

	InstructionSet[0x9C] = (Instruction){&cosproc::IMM,&cosproc::JES,"JES #oper",0};
	InstructionSet[0x9D] = (Instruction){&cosproc::ABS,&cosproc::JES,"JES oper",0};
	InstructionSet[0x9E] = (Instruction){&cosproc::IND,&cosproc::JES,"JES @oper",0};
	InstructionSet[0x9F] = (Instruction){&cosproc::REG,&cosproc::JESR,"JES RX",0};

	InstructionSet[0xA0] = (Instruction){&cosproc::IMP,&cosproc::CSF,"CSF",1};
	InstructionSet[0xA1] = (Instruction){&cosproc::IMP,&cosproc::CZF,"CZF",1};
	InstructionSet[0xA2] = (Instruction){&cosproc::IMP,&cosproc::SZF,"SZF",1};
	InstructionSet[0xA3] = (Instruction){&cosproc::IMP,&cosproc::CNF,"CNF",1};
	InstructionSet[0xA4] = (Instruction){&cosproc::IMP,&cosproc::SNF,"SNF",1};
	InstructionSet[0xA5] = (Instruction){&cosproc::IMP,&cosproc::CCF,"CCF",1};
	InstructionSet[0xA6] = (Instruction){&cosproc::IMP,&cosproc::SCF,"SCF",1};
	InstructionSet[0xA7] = (Instruction){&cosproc::IMP,&cosproc::COF,"COF",1};
	InstructionSet[0xA8] = (Instruction){&cosproc::IMP,&cosproc::SOF,"SOF",1};
	InstructionSet[0xA9] = (Instruction){&cosproc::IMP,&cosproc::CLF,"CLF",1};
	InstructionSet[0xAA] = (Instruction){&cosproc::IMP,&cosproc::SLF,"SLF",1};
	InstructionSet[0xAB] = (Instruction){&cosproc::IMP,&cosproc::CIF,"CIF",1};
	InstructionSet[0xAC] = (Instruction){&cosproc::IMP,&cosproc::SIF,"SIF",1};
	InstructionSet[0xAD] = (Instruction){&cosproc::IMP,&cosproc::CEF,"CEF",1};

	InstructionSet[0xB0] = (Instruction){&cosproc::ABS,&cosproc::INC,"INC oper",3};
	InstructionSet[0xB1] = (Instruction){&cosproc::IND,&cosproc::INC,"INC @oper",3};
	InstructionSet[0xB2] = (Instruction){&cosproc::REG,&cosproc::INCR,"INC RX",2};
	InstructionSet[0xB3] = (Instruction){&cosproc::ABS,&cosproc::INCX,"INCX oper",3};
	InstructionSet[0xB4] = (Instruction){&cosproc::IND,&cosproc::INCX,"INCX @oper",3};
	InstructionSet[0xB5] = (Instruction){&cosproc::REG,&cosproc::INCXR,"INCX RX",2};

	InstructionSet[0xB6] = (Instruction){&cosproc::ABS,&cosproc::DEC,"DEC oper",3};
	InstructionSet[0xB7] = (Instruction){&cosproc::IND,&cosproc::DEC,"DEC @oper",3};
	InstructionSet[0xB8] = (Instruction){&cosproc::REG,&cosproc::DECR,"DEC RX",2};
	InstructionSet[0xB9] = (Instruction){&cosproc::ABS,&cosproc::DECX,"DECX oper",3};
	InstructionSet[0xBA] = (Instruction){&cosproc::IND,&cosproc::DECX,"DECX @oper",3};
	InstructionSet[0xBB] = (Instruction){&cosproc::REG,&cosproc::DECXR,"DECX RX",2};

	reset();
}

void cosproc::reset()
{
	memset(r,0,sizeof(r));
	pc = 0;
    sp = 0xC399;
    memset(st,0,sizeof(st));

	return;
}


cosproc::Debug cosproc::cycle()
{
	uint8_t opcode = Read(pc); //Fetch
	Instruction currentInstruction = InstructionSet[opcode]; //Decode
	execute(currentInstruction); //Execute
	Debug debugPackage;
	debugPackage.pc = pc;
	debugPackage.instruction = currentInstruction;
	pc += currentInstruction.bytes; //Writeback
	return debugPackage;
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
	r[0] = Read(++sp);
}

/* 0x04 SWP */
void cosproc::SWP(uint16_t src){
	uint8_t temp = r[Read(pc+1)];
	r[Read(pc+1)] = r[Read(pc+2)];
	r[Read(pc+2)] = temp;
}

/* 0x05-0x07 CALL */
void cosproc::CALL(uint16_t src){
	Write(sp,(pc & 0xFF));
	Write(--sp,(pc >> 8) & 0xFF);
	sp--;
	pc = (Read(src) << 8) | Read(src+1);
}

/* 0x08 RET */
void cosproc::RET(uint16_t src){
	pc = (Read(sp+1) << 8) | Read(sp+2);
	sp +=2;
}

/* 0x10-0x12 ADD */
void cosproc::ADD(uint16_t src){
	uint8_t data = Read(src);

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
	uint16_t regs = r[0] << 8 | r[1];

	unsigned int temp =  ((r[0] << 8) | r[1] ) + data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((regs^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x17 ADDXR */
void cosproc::ADDXR(uint16_t src){
	uint16_t regs = r[0] << 8 | r[1];
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
	st[3] = ((regs^temp)&(data^temp)&0x8000) != 0;

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
	uint16_t regs = r[0] << 8 | r[1];

	unsigned int temp =  ((r[0] << 8) | r[1] ) - data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((regs^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x1F SUBXR  */
void cosproc::SUBXR(uint16_t src){
	uint16_t regs = r[0] << 8 | r[1];
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
	st[3] = ((regs^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x20-0x22 MUL from Imm/Abs/Ind */
void cosproc::MUL(uint16_t src){
	uint8_t data = Read(src);
	unsigned int temp = r[0] * data;

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

/* 0x23 MUL from register */
void cosproc::MULR(uint16_t src){
	uint8_t data = r[src];
	unsigned int temp = r[0] * data;

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

/* 0x24-0x26 MULX from 16-bit Imm/Abs/Ind */
void cosproc::MULX(uint16_t src){
	uint16_t data = ((Read(src) << 8) | Read(src+1));
	uint16_t regs = r[0] << 8 | r[1];
	unsigned int temp =  ((r[0] << 8) | r[1] ) * data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((regs^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x27 MULX from 16-bit register */
void cosproc::MULXR(uint16_t src){
	uint16_t regs = r[0] << 8 | r[1];
	uint16_t data;

	if(src % 2 == 0){
		data = ((r[src] << 8) | r[src+1]);
	}else{
		data = ((r[src-1] << 8) | r[src]);
	}
	unsigned int temp =  ((r[0] << 8) | r[1] ) * data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((regs^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x28-0x2A DIV from Imm/Abs/Ind */
void cosproc::DIV(uint16_t src){
	uint8_t data = Read(src);
	if(!data){
		st[6] = 1;
		return;
	}
	unsigned int temp = r[0] / data;

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

/* 0x2B DIV from register */
void cosproc::DIVR(uint16_t src){
	uint8_t data = r[src];
	if(!data){
		st[6] = 1;
		return;
	}
	unsigned int temp = r[0] / data;

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

/* 0x2C-0x2E DIVX from 16-bit Imm/Abs/Ind */
void cosproc::DIVX(uint16_t src){
	uint16_t data = ((Read(src) << 8) | Read(src+1));
	if(!data){
		st[6] = 1;
		return;
	}
	uint16_t regs = r[0] << 8 | r[1];
	unsigned int temp =  ((r[0] << 8) | r[1] ) / data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((regs^temp)&(data^temp)&0x8000) != 0;

	//Set Value
	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;

	//Set Zero
	st[0] = (r[0] << 8 | r[1]) == 0;
}

/* 0x2F DIVX from 16-bit register */
void cosproc::DIVXR(uint16_t src){
	uint16_t data;
	if(src % 2 == 0){
		data = ((r[src] << 8) | r[src+1]);
	}else{
		data = ((r[src-1] << 8) | r[src]);
	}
	if(!data){
		st[6] = 1;
		return;
	}

	uint16_t regs = r[0] << 8 | r[1];
	unsigned int temp =  ((r[0] << 8) | r[1] ) / data;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((regs^temp)&(data^temp)&0x8000) != 0;

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
	Write(dst,r[src]);  //Write the value of the register to the location
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
	r[Read(pc+2)] = r[src];
}

/* 0x3C-0x3E SHL Shift the Accumulator left from Imm/Abs/Ind */
void cosproc::SHL(uint16_t src){
	uint8_t shift = Read(src);
	if(shift > 8){
		shift = 8;
	}
	uint16_t shifted = r[0] << shift;

	//Set Carry
	st[2] = (shifted & 0xFF00) != 0;

	r[0] = shifted & 0x00FF;
	
	//Set Zero
	st[0] = r[0] == 0;	
}

/* 0x3F SHL Shift the Accumulator left from register */
void cosproc::SHLR(uint16_t src){
	uint8_t shift = r[src];
	if(shift > 8){
		shift = 8;
	}
	uint16_t shifted = r[0] << shift;

	//Set Carry
	st[2] = (shifted & 0xFF00) != 0;

	r[0] = shifted & 0x00FF;
	
	//Set Zero
	st[0] = r[0] == 0;
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
	int reg;
	src % 2 == 0 ? reg = src : reg = src-1;

	Write(dst, r[reg]);
	Write(dst+1, r[reg+1]);
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
	int reg;
	src % 2 ==0 ? reg = src : reg = src-1;

	Write(dst, r[reg]);
	Write(dst+1, r[reg+1]);
}

/* 0x48 MOVX to Register from Immediate */
void cosproc::MOVXRI(uint16_t src){
	int reg = Read(pc+3);
	if(reg % 2 != 0){
		reg -= 1;
	}

	r[reg] = Read(src);
	r[reg+1] = Read(src+1);
}

/* 0x49-0x4A MOVX to Register from Absolute/Indirect */
void cosproc::MOVXR(uint16_t src){
	int reg = Read(pc+3);
	if(reg % 2 != 0){
		reg -= 1;
	}

	r[reg] = Read(src);
	r[reg+1] = Read(src+1);
}

/* 0x4B MOVX to Register from Register */
void cosproc::MOVXRR(uint16_t src){
	int reg = Read(pc+2);
	if(reg % 2 != 0){
		reg -= 1;
	}

	int reg2;
	src % 2 == 0 ? reg2 = src : reg2 = src-1;

	r[reg] = r[reg2];
	r[reg+1] = r[reg2+1];
}

/* 0x4C-0x4E SHLX Shift the 16-bit Accumulator left from Imm/Abs/Ind */
void cosproc::SHLX(uint16_t src){
	uint16_t shift = (Read(src) << 8 | Read(src+1));
	if(shift > 16){
		shift = 16;
	}
	uint32_t shifted = (r[0] << 8 | r[1]) << shift;

	//Set Carry
	st[2] = (shifted & 0xFFFF0000) != 0;

	r[0] = (shifted & 0x0000FF00) >> 8;
	r[1] = shifted & 0x000000FF;
	
	//Set Zero
	st[0] = (r[0] == 0) && (r[1] == 0);	
}

/* 0x4F SHLX Shift the 16-bit Accumulator left from register */
void cosproc::SHLXR(uint16_t src){
	uint16_t shift;

	if(src % 2 == 0){
		shift = ((r[src] << 8) | r[src+1]);
	}else{
		shift = ((r[src-1] << 8) | r[src]);
	}
	if(shift > 16){
		shift = 16;
	}
	uint32_t shifted = (r[0] << 8 | r[1]) << shift;

	//Set Carry
	st[2] = (shifted & 0xFFFF0000) != 0;

	r[0] = (shifted & 0x0000FF00) >> 8;
	r[1] = shifted & 0x000000FF;
	
	//Set Zero
	st[0] = (r[0] == 0) && (r[1] == 0);	
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
	r[0] |= r[src];

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
	r[0] ^= r[src];

	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x5C-0x5E SHR Shift the Accumulator right from Imm/Abs/Ind */
void cosproc::SHR(uint16_t src){
	r[0] = r[0] >> Read(src);
	
	//Set Zero
	st[0] = r[0] == 0;	
}

/* 0x5F SHR Shift the Accumulator right from register */
void cosproc::SHRR(uint16_t src){
	r[0] = r[0] >> r[src];
	
	//Set Zero
	st[0] = r[0] == 0;
}

/* 0x60-0x62 CMP Compare with Accumulator */
void cosproc::CMP(uint16_t src){
	uint8_t temp = (unsigned int)Read(src);

	CLF(0x00); //Clear Flags

	if(temp == r[0]){
		st[0] = true;
	}
	if(temp > r[0]){
		st[1] = true;
	}
	if(temp < r[0]){
		st[2] = true;
	}
	if(int8_t(temp) < int8_t(r[0])){
		st[3] = true;
	}
	if(int8_t(temp) > int8_t(r[0])){
		st[4] = true;
	}
}

/* 0x63 CMPR Compare with Accumulator from register*/
void cosproc::CMPR(uint16_t src){
	unsigned int temp = r[src];

	CLF(0x00); //Clear Flags

	if(temp == r[0]){
		st[0] = true;
	}
	if(temp > r[0]){
		st[1] = true;
	}
	if(temp < r[0]){
		st[2] = true;
	}
	if(int8_t(temp) < int8_t(r[0])){
		st[3] = true;
	}
	if(int8_t(temp) > int8_t(r[0])){
		st[4] = true;
	}
}

/* 0x64-0x66 CMPX Compare with 16-bit Accumulator */
void cosproc::CMPX(uint16_t src){
	uint16_t temp = (Read(src) << 8) | Read(src+1);
	uint16_t uacc = (r[0] << 8) | r[1];
	int16_t sacc = (r[0] << 8) | r[1];

	CLF(0x00); //Clear Flags

	if(temp == uacc){
		st[0] = true;
	}
	if(temp > uacc){
		st[1] = true;
	}
	if(temp < uacc){
		st[2] = true;
	}
	if(int16_t(temp) < sacc){
		st[3] = true;
	}
	if(int16_t(temp) > sacc){
		st[4] = true;
	}
}

/* 0x67 CMPXR Compare with 16-bit Accumulator from register*/
void cosproc::CMPXR(uint16_t src){
	uint16_t temp;

	if(src % 2 == 0){
		temp = ((r[src] << 8) | r[src+1]);
	}else{
		temp = ((r[src-1] << 8) | r[src]);
	}
	uint16_t uacc = (r[0] << 8) | r[1];
	int16_t sacc = (r[0] << 8) | r[1];

	CLF(0x00); //Clear Flags

	if(temp == uacc){
		st[0] = true;
	}
	if(temp > uacc){
		st[1] = true;
	}
	if(temp < uacc){
		st[2] = true;
	}
	if(int16_t(temp) < sacc){
		st[3] = true;
	}
	if(int16_t(temp) > sacc){
		st[4] = true;
	}
}

/* 0x6C-0x6E SHRX Shift the 16-bit Accumulator right from Imm/Abs/Ind */
void cosproc::SHRX(uint16_t src){
	uint16_t shift = (Read(src) << 8) | Read(src+1);
	uint16_t temp = ((r[0] << 8) | r[1]) >> shift;

	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;
	
	//Set Zero
	st[0] = temp == 0;
}

/* 0x6F SHRX Shift the 16-bit Accumulator right from register */
void cosproc::SHRXR(uint16_t src){
	uint16_t shift;

	if(src % 2 == 0){
		shift = ((r[src] << 8) | r[src+1]);
	}else{
		shift = ((r[src-1] << 8) | r[src]);
	}
	uint16_t temp = ((r[0] << 8) | r[1]) >> shift;

	r[0] = (temp & 0xFF00) >> 8;
	r[1] = temp & 0x00FF;
	
	//Set Zero
	st[0] = temp == 0;
}

/* 0x70-0x72 JMP from Imm/Abs/Ind */
void cosproc::JMP(uint16_t src){
	pc = ((Read(src) << 8) | Read(src+1));	
}

// 0x73 JMP from Reg
void cosproc::JMPR(uint16_t src){
	int data;
	if(src % 2 == 0){
		data = ((r[src] << 8) | r[src+1]);
	}else{
		data = ((r[src-1] << 8) | r[src]);
	}
	pc = (data);
}

/* 0x74-0x76 JZS from Imm/Abs/Ind */
void cosproc::JZS(uint16_t src){
	if(st[0]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}
/* 0x77 JZS from Reg */
void cosproc::JZSR(uint16_t src){
	int data;
	if(st[0]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x78-0x7A JNZ from Imm/Abs/Ind */
void cosproc::JNZ(uint16_t src){
	if(!st[0]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x7B JNZ from Reg */
void cosproc::JNZR(uint16_t src){
	int data;
	if(!st[0]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x7C-0x7E JCS from Imm/Abs/Ind */
void cosproc::JCS(uint16_t src){
	if(st[2]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x7F JCS from Reg */
void cosproc::JCSR(uint16_t src){
	int data;
	if(st[2]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x80-0x82 JNC from Imm/Abs/Ind */
void cosproc::JNC(uint16_t src){
	if(!st[2]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x83 JNC fom Reg */
void cosproc::JNCR(uint16_t src){
	int data;
	if(!st[2]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x84-0x86 JOS from Imm/Abs/Ind */
void cosproc::JOS(uint16_t src){
	if(st[3]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x87 JOS from Reg */
void cosproc::JOSR(uint16_t src){
	int data;
	if(st[3]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x88-0x8A JNO from Imm/Abs/Ind */
void cosproc::JNO(uint16_t src){
	if(!st[3]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x8B JNO from Reg */
void cosproc::JNOR(uint16_t src){
	int data;
	if(!st[3]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x8C-0x8E JNS from Imm/Abs/Ind */
void cosproc::JNS(uint16_t src){
	if(st[1]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x8F JNS from Reg */
void cosproc::JNSR(uint16_t src){
	int data;
	if(st[1]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x90-0x92 JNN from Imm/Abs/Ind */
void cosproc::JNN(uint16_t src){
	if(!st[1]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x93 JNN from Reg */
void cosproc::JNNR(uint16_t src){
	int data;
	if(!st[1]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x94-0x96 JLS from Imm/Abs/Ind */
void cosproc::JLS(uint16_t src){
	if(st[4]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x97 JLS from Reg */
void cosproc::JLSR(uint16_t src){
	int data;
	if(st[4]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x98-0x9A JNL from Imm/Abs/Ind */
void cosproc::JNL(uint16_t src){
	if(!st[4]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x9B JNL from Reg */
void cosproc::JNLR(uint16_t src){
	int data;
	if(!st[4]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0x9C-0x9E JES from Imm/Abs/Ind */
void cosproc::JES(uint16_t src){
	if(st[6]){
		pc = ((Read(src) << 8) | Read(src+1));
	}else{
		pc += 3;
	}
}

/* 0x9F JES from Reg */
void cosproc::JESR(uint16_t src){
	int data;
	if(st[6]){
		if(src % 2 == 0){
			data = ((r[src] << 8) | r[src+1]);
		}else{
			data = ((r[src-1] << 8) | r[src]);
		}
		pc = (data);
	}else{
		pc += 2;
	}
}

/* 0xA0 CSF */
void cosproc::CSF(uint16_t src){
	for(int i = 0; i < 8; i++){
		st[i] = 0;
	}
}

/* 0xA1 CZF */
void cosproc::CZF(uint16_t src){
	st[0] = 0;
}

/* 0xA2 SZF */
void cosproc::SZF(uint16_t src){
	st[0] = 1;
}

/* 0xA3 CNF */
void cosproc::CNF(uint16_t src){
	st[1] = 0;
}

/* 0xA4 SNF */
void cosproc::SNF(uint16_t src){
	st[1] = 1;
}

/* 0xA5 CCF */
void cosproc::CCF(uint16_t src){
	st[2] = 0;
}

/* 0xA6 SCF */
void cosproc::SCF(uint16_t src){
	st[2] = 1; 
}

/* 0xA7 COF */
void cosproc::COF(uint16_t src){
	st[3] = 0;
}

/* 0xA8 SOF */
void cosproc::SOF(uint16_t src){
	st[3] = 1;
}

/* 0xA9 CLF */
void cosproc::CLF(uint16_t src){
	st[4] = 0;
}

/* 0xAA SLF */
void cosproc::SLF(uint16_t src){
	st[4] = 1;
}

/* 0xAB CIF */
void cosproc::CIF(uint16_t src){
	st[5] = 0;
}

/* 0xAC SIF */
void cosproc::SIF(uint16_t src){
	st[5] = 1;
}

/* 0xAD CEF */
void cosproc::CEF(uint16_t src){
	st[6] = 0;
}

/* 0xB0-0xB1 INC from Abs/Ind */
void cosproc::INC(uint16_t src){
	uint8_t data = Read(src);

	unsigned int temp = data + 1;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((data^temp)&(0x01^temp)&0x80) != 0;
	
	//Set Value
	Write(src, temp & 0xFF);

	//Set Zero
	st[0] = (temp & 0xFF) == 0;
}

/* 0xB2 INCR from REG */
void cosproc::INCR(uint16_t src){
	uint8_t data = r[src];

	unsigned int temp = data + 1;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((data^temp)&(0x01^temp)&0x80) != 0;
	
	//Set Value
	r[src] = temp & 0xFF;

	//Set Zero
	st[0] = r[src] == 0;
}

/* 0xB3-0xB4 INCX from Abs/Ind */
void cosproc::INCX(uint16_t src){
	uint8_t dataHigh = Read(src);
	uint8_t dataLow = Read(src+1);
	uint16_t data = ((dataHigh << 8) | dataLow);

	unsigned int temp = data + 1;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((data^temp)&(0x0001^temp)&0x8000) != 0;

	//Set Value
	Write(src, (temp & 0xFF00) >> 8);
	Write(src+1, temp & 0x00FF);

	//Set Zero
	st[0] = (temp & 0xFFFF) == 0;
}

/* 0xB5 INCXR from REG */
void cosproc::INCXR(uint16_t src){
	uint16_t data;

	if(src % 2 == 0){
		data = ((r[src] << 8) | r[src+1]);
	}else{
		data = ((r[src-1] << 8) | r[src]);
	}

	unsigned int temp = data + 1;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((data^temp)&(0x0001^temp)&0x8000) != 0;

	//Set Value
	if(src % 2 == 0){
		r[src] = (temp & 0xFF00) >> 8;
		r[src+1] = temp & 0x00FF;

		//Set Zero
		st[0] = (r[src] << 8 | r[src+1]) == 0;
	}else{
		r[src-1] = (temp & 0xFF00) >> 8;
		r[src] = temp & 0x00FF;

		//Set Zero
		st[0] = (r[src-1] << 8 | r[src]) == 0;
	}	
}

/* 0xB6-0xB7 DEC from Abs/Ind */
void cosproc::DEC(uint16_t src){
	uint8_t data = Read(src);

	unsigned int temp = data - 1;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((data^temp)&(0x01^temp)&0x80) != 0;
	
	//Set Value
	Write(src, temp & 0xFF);

	//Set Zero
	st[0] = (temp & 0xFF) == 0;
}

/* 0xB8 DECR from REG */
void cosproc::DECR(uint16_t src){
	uint8_t data = r[src];

	unsigned int temp = data - 1;

	//Set Negative
	st[1] = temp >= 0x80;
	//Set Carry
	st[2] = temp > 0xFF;
	//Set Overflow
	st[3] = ((data^temp)&(0x01^temp)&0x80) != 0;
	
	//Set Value
	r[src] = temp & 0xFF;

	//Set Zero
	st[0] = r[src] == 0;
}

/* 0xB9-0xBA DECX from Abs/Ind */
void cosproc::DECX(uint16_t src){
	uint8_t dataHigh = Read(src);
	uint8_t dataLow = Read(src+1);
	uint16_t data = ((dataHigh << 8) | dataLow);

	unsigned int temp = data - 1;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((data^temp)&(0x0001^temp)&0x8000) != 0;

	//Set Value
	Write(src, (temp & 0xFF00) >> 8);
	Write(src+1, temp & 0x00FF);

	//Set Zero
	st[0] = (temp & 0xFFFF) == 0;
}

/* 0xBB DECXR from REG */
void cosproc::DECXR(uint16_t src){
	uint16_t data;

	if(src % 2 == 0){
		data = ((r[src] << 8) | r[src+1]);
	}else{
		data = ((r[src-1] << 8) | r[src]);
	}

	unsigned int temp = data - 1;

	//Set Negative
	st[1] = temp >= 0x8000;
	//Set Carry
	st[2] = temp > 0xFFFF;
	//Set Overflow
	st[3] = ((data^temp)&(0x0001^temp)&0x8000) != 0;

	//Set Value
	if(src % 2 == 0){
		r[src] = (temp & 0xFF00) >> 8;
		r[src+1] = temp & 0x00FF;

		//Set Zero
		st[0] = (r[src] << 8 | r[src+1]) == 0;
	}else{
		r[src-1] = (temp & 0xFF00) >> 8;
		r[src] = temp & 0x00FF;

		//Set Zero
		st[0] = (r[src-1] << 8 | r[src]) == 0;
	}
}

/* Low Priority Interrupt */
void cosproc::LPI(){
	if(!st[5]){
		Write(sp,((pc*0xFF00)>>8));
		sp--;
		Write(sp,(pc*0x00FF));
		sp--;
		pc = (Read(0xFFF2) << 8) & Read(0xFFF3);
	}
}

/* High Priority Interrupt */
void cosproc::HPI(){
	Write(sp,((pc*0xFF00)>>8));
	sp--;
	Write(sp,(pc*0x00FF));
	sp--;
	pc = (Read(0xFFF2) << 8) & Read(0xFFF3);
}
