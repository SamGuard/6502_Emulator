#pragma once
#include "CPU.h"


void CPU::printStatus() {
	std::cout << std::endl << "PC: " << (uint32_t)PC << std::endl;
	std::cout << "A: " << (uint32_t)A << " X: " << (uint32_t)X << " Y:" << (uint32_t)Y << std::endl;
	Byte status = C << 6 | Z << 5 | I << 4 | D << 3 | B << 2 | V << 1 | N;
	std::cout << "CZIDBVN" << std::endl << std::bitset<7>(status) << std::endl;
}


CPU::CPU()
{
	A = X = Y = 0; //Registers
	C = Z = I = D = B = V = N = 0; //Flags

	PC = 0xFFFC;
	SP = 0x00FF;
}

CPU::~CPU()
{
}

void CPU::init(Memory& mem) {
	A = X = Y = 0; //Registers
	C = Z = I = D = B = V = N = 0; //Flags

	PC = 0xFFFC;
	SP = 0x00FF;

	mem.init();
}

Byte CPU::getLowByte(Word val) {
	return (0b0000000011111111 & val);
}

Byte CPU::getHighByte(Word val) {
	return (0b1111111100000000 & val) >> 8;
}

Byte CPU::fetch(u32& cycles, Memory& mem) {
	Byte data = mem.readByte(PC);
	cycles--;
	return data;
}

Word CPU::fetchWord(u32& cycles, Memory& mem) {
	Byte lByte = fetch(cycles, mem);
	PC++;
	Byte hByte = fetch(cycles, mem);
	return lByte | (hByte << 8);
}

Byte CPU::readByte(Word address, u32& cycles, Memory& mem) {
	Byte data = mem.readByte(address);
	cycles--;
	return data;
}

Word CPU::readWord(Word address, u32& cycles, Memory& mem) {
	Byte hByte, lByte;
	lByte = readByte(address, cycles, mem);
	hByte = readByte(address + 1, cycles, mem);
	return lByte | (hByte << 8);
}

void CPU::writeByte(Word address, Byte val, u32& cycles, Memory& mem) {
	cycles--;
	mem.writeByte(address, val);
}

void CPU::writeWord(Word address, Word val, u32& cycles, Memory& mem) {
	Byte hByte, lByte;
	hByte = getHighByte(val);
	lByte = getLowByte(val);
	writeByte(address, lByte, cycles, mem);
	writeByte(address + 1, hByte, cycles, mem);
}


//---------------INSTRUCTIONS--------------------------

/* Loading/Storing
	AC = Accumulator, this is where the A value is used and the address is ignored
	IM = Immediate, this is where the address is the actual value, not a reference to a memory location
	ZP = Zero Page, this references the first 256 bytes of memory, useful as you only need 1 byte instead of 2 for a full address
	ZX = Zero Page X, same as ZP but the the value of X is added to the address given
	ZY = Zero Page Y, same as ZX but uses Y and only can be used on LDX and STX
	AB = Absolute, the 2 byte address given is the location in memory to load
	AX = Absolute X, 2 byte address + X is the location in memory to load
	AY = Absolute Y, same as AX but with Y
	ID = Indirect, the 2 byte address given gives the 2 byte address in memory of the 2 byte address to be loaded from memory
	IX = Indexed Indirect, the 1 byte address given is a ZP address, the X register is then added to give the location of the 2 byte address in the ZP memory which is the address in memory
	IY = Indirect Index, reverse of IX, the 1 byte address given is the location of the 2 byte address, then Y is added to that address to give the 2 byte address of the location in memory

*/
Byte CPU::load(Byte addrMode, Word dataAddr, u32& cycles, Memory& mem) {
	//Byte addr = readByte(dataAddr, cycles, mem);
	//Word addr = readWord(dataAddr, cycles, mem);
	Word finalAddr = NULL;
	cycles--;
	switch (addrMode)
	{
	case (ADR_AC): {
		return A;
	}break;
	case ADR_IM: {
		Byte addr = readByte(dataAddr, cycles, mem);
		return addr;
	}break;

	case ADR_ZP: {
		Byte addr = readByte(dataAddr, cycles, mem);
		finalAddr = (Byte)addr;
	} break;

	case(ADR_ZX): {
		Byte addr = readByte(dataAddr, cycles, mem);
		finalAddr = (Byte)addr + X;
		cycles--;
	}break;

	case(ADR_ZY): {
		Byte addr = readByte(dataAddr, cycles, mem);
		finalAddr = (Byte)addr + Y;
		cycles--;
	}break;

	case(ADR_AB): {
		Word addr = readWord(dataAddr, cycles, mem);
		finalAddr = readWord(addr, cycles, mem);
	}break;
	case(ADR_AX): {
		Word addr = readWord(dataAddr, cycles, mem);
		finalAddr = readWord(addr, cycles, mem) + X;
	}break;
	case(ADR_AY): {
		Word addr = readWord(dataAddr, cycles, mem);
		finalAddr = readWord(addr, cycles, mem) + Y;
	}break;

	case(ADR_ID): {
		Word addr = readWord(dataAddr, cycles, mem);
		finalAddr = readWord(readWord(addr, cycles, mem), cycles, mem);

	}break;
	case(ADR_IX): {
		Byte addr = readByte(dataAddr, cycles, mem);
		Byte zeroPageAddr = readByte(addr, cycles, mem) + X;
		finalAddr = readWord(zeroPageAddr, cycles, mem);
	}break;
	case(ADR_IY): {
		Byte addr = readByte(dataAddr, cycles, mem);
		Byte zeroPageAddr = readByte(addr, cycles, mem);
		finalAddr = readWord(zeroPageAddr, cycles, mem) + Y;
	}break;

	default:
		ASSERT_VERBOSE(false, "Invalid addressing mode", __LINE__, this);
		break;
	}

	return readByte(finalAddr, cycles, mem);
}

void CPU::storeToReg(Byte reg, Byte val) {
	switch (reg)
	{
	case(REG_A): {
		A = val;
		Z = (A == 0);
		N = (0b10000000 & A) > 0;
	} break;

	case(REG_X): {
		X = val;
		Z = (X == 0);
		N = (0b10000000 & X) > 0;
	} break;

	case(REG_Y): {
		Y = val;
		Z = (Y == 0);
		N = (0b10000000 & Y) > 0;
	}break;

	default:
		ASSERT_VERBOSE(false, "Invalid register", __LINE__, this);
		break;
	}
}


void CPU::store(Word addrMode, Word dataAddr, Byte data, u32& cycles, Memory& mem) {
	cycles--;
	switch (addrMode)
	{
	case ADR_ZP: {
		Byte addr = readByte(dataAddr, cycles, mem);
		ASSERT_VERBOSE((addr & 0b1111111100000000) != 0b1111111100000000, "(addr & 0b1111111100000000) != 0b1111111100000000", __LINE__, this); //Check addr is actually 8 bits
		writeByte(addr, data, cycles, mem);
	} break;

	case(ADR_ZX): {
		Byte addr = readByte(dataAddr, cycles, mem);
		ASSERT_VERBOSE((addr & 0b1111111100000000) != 0b1111111100000000, "(addr & 0b1111111100000000) != 0b1111111100000000", __LINE__, this); //Check addr is actually 8 bits
		writeByte(addr + X, data, cycles, mem);
		cycles--;
	}break;

	case(ADR_ZY): {
		Byte addr = readByte(dataAddr, cycles, mem);
		ASSERT_VERBOSE((addr & 0b1111111100000000) != 0b1111111100000000, "(addr & 0b1111111100000000) != 0b1111111100000000", __LINE__, this); //Check addr is actually 8 bits
		writeByte(addr + Y, data, cycles, mem);
		cycles--;
	}break;

	case(ADR_AB): {
		Word addr = readWord(dataAddr, cycles, mem);
		writeByte(addr, data, cycles, mem);
	}break;
	case(ADR_AX): {
		Word addr = readWord(dataAddr, cycles, mem);
		writeByte(addr + X, data, cycles, mem);
	}break;
	case(ADR_AY): {
		Word addr = readWord(dataAddr, cycles, mem);
		writeByte(addr + Y, data, cycles, mem);
	}break;

	case(ADR_IX): {
		Byte addr = readByte(dataAddr, cycles, mem);
		writeByte(readByte(addr + X, cycles, mem), data, cycles, mem);
	}break;
	case(ADR_IY): {
		Byte addr = readByte(dataAddr, cycles, mem);
		writeByte(readByte(addr, cycles, mem) + Y, data, cycles, mem);
	}break;

	default:
		ASSERT_VERBOSE(false, "Invalid addressing mode", __LINE__, this);
		break;
	}
}


void CPU::execute(u32 cycles, bool useCycles, Memory& mem) {
	while ((cycles > 0 || !useCycles) && PC != 0xFFFF) {
		Word ins = fetch(cycles, mem);

		switch (ins)
		{
			//------------#LDA--------------
		case(INS_LDA_IM): {
			PC++;
			storeToReg(REG_A, load(ADR_IM, PC, cycles, mem));
		}break;
		case(INS_LDA_ZP): {
			PC++;
			storeToReg(REG_A, load(ADR_ZP, PC, cycles, mem));
		}break;
		case(INS_LDA_ZX): {
			PC++;
			storeToReg(REG_A, load(ADR_ZX, PC, cycles, mem));
		}break;
		case(INS_LDA_AB): {
			PC++;
			storeToReg(REG_A, load(ADR_AB, PC, cycles, mem));
		}break;
		case(INS_LDA_AX): {
			PC++;
			storeToReg(REG_A, load(ADR_AX, PC, cycles, mem));
		}break;
		case(INS_LDA_AY): {
			PC++;
			storeToReg(REG_A, load(ADR_AY, PC, cycles, mem));
		}break;
		case(INS_LDA_IX): {
			PC++;
			storeToReg(REG_A, load(ADR_IX, PC, cycles, mem));
		}break;
		case(INS_LDA_IY): {
			PC++;
			storeToReg(REG_A, load(ADR_IY, PC, cycles, mem));
		}break;


			//-----------#LDX---------------
		case(INS_LDX_IM): {
			PC++;
			storeToReg(REG_X, load(ADR_IM, PC, cycles, mem));
		}break;
		case(INS_LDX_ZP): {
			PC++;
			storeToReg(REG_X, load(ADR_ZP, PC, cycles, mem));
		}break;
		case(INS_LDX_ZY): {
			PC++;
			storeToReg(REG_X, load(ADR_ZY, PC, cycles, mem));
		}break;
		case(INS_LDX_AB): {
			PC++;
			storeToReg(REG_X, load(ADR_AB, PC, cycles, mem));
		}break;
		case(INS_LDX_AY): {
			PC++;
			storeToReg(REG_X, load(ADR_AY, PC, cycles, mem));
		}break;

			//-----------#LDY---------------
		case(INS_LDY_IM): {
			PC++;
			storeToReg(REG_Y, load(ADR_IM, PC, cycles, mem));
		}break;
		case(INS_LDY_ZP): {
			PC++;
			storeToReg(REG_Y, load(ADR_ZP, PC, cycles, mem));
		}break;
		case(INS_LDY_ZX): {
			PC++;
			storeToReg(REG_Y, load(ADR_ZX, PC, cycles, mem));
		}break;
		case(INS_LDY_AB): {
			PC++;
			storeToReg(REG_Y, load(ADR_AB, PC, cycles, mem));
		}break;
		case(INS_LDY_AX): {
			PC++;
			storeToReg(REG_Y, load(ADR_AX, PC, cycles, mem));
		}break;



			//------------#STA--------------
		case(INS_STA_ZP): {
			PC++;
			store(ADR_ZP, PC, A, cycles, mem);
		}break;
		case(INS_STA_ZX): {
			PC++;
			store(ADR_ZX, PC, A, cycles, mem);
		}break;
		case(INS_STA_AB): {
			PC++;
			store(ADR_AB, PC, A, cycles, mem);
		}break;
		case(INS_STA_AX): {
			PC++;
			store(ADR_AX, PC, A, cycles, mem);
		}break;
		case(INS_STA_AY): {
			PC++;
			store(ADR_AY, PC, A, cycles, mem);
		}break;
		case(INS_STA_IX): {
			PC++;
			store(ADR_IX, PC, A, cycles, mem);
		}break;
		case(INS_STA_IY): {
			PC++;
			store(ADR_IY, PC, A, cycles, mem);
		}break;

			//-----------#STX----------------
		case(INS_STX_ZP): {
			PC++;
			store(ADR_ZP, PC, X, cycles, mem);
		}break;
		case(INS_STX_ZY): {
			PC++;
			store(ADR_ZY, PC, X, cycles, mem);
		}break;
		case(INS_STX_AB): {
			PC++;
			store(ADR_AB, PC, X, cycles, mem);
		}break;

			//-----------#STY----------------
		case(INS_STY_ZP): {
			PC++;
			store(ADR_ZP, PC, Y, cycles, mem);
		}break;
		case(INS_STY_ZX): {
			PC++;
			store(ADR_ZX, PC, Y, cycles, mem);
		}break;
		case(INS_STY_AB): {
			PC++;
			store(ADR_AB, PC, Y, cycles, mem);
		}break;

			//-----------#JSR---------------
		case(INS_JSR): {
			//Push onto stack
			ASSERT_VERBOSE(SP > 1, "SP > 1", __LINE__, this);

			writeWord(SP + 0x0100 - 1, PC + 2, cycles, mem);
			SP -= 2;
			PC++;
			cycles -= 2;
			PC = fetchWord(cycles, mem);
		}break;

		case(INS_RTS): {
			ASSERT_VERBOSE(SP <= 253, "SP <= 253", __LINE__, this);
			SP += 2;
			PC = readWord(SP + 0x0100 - 1, cycles, mem);
			cycles -= 2;
		}break;


			//----------#Flags---------
		case(INS_CLC): {
			C = 0;
		}break;
		case(INS_SEC): {
			C = 1;
		}break;

			//----------#ADC-----------
		case(INS_ADC_IM): {
			PC++;
			ADC(ADR_IM, PC, cycles, mem);
		}break;
		case(INS_ADC_ZP): {
			PC++;
			ADC(ADR_ZP, PC, cycles, mem);
		}break;
		case(INS_ADC_ZX): {
			PC++;
			ADC(ADR_ZX, PC, cycles, mem);
		}break;
		case(INS_ADC_AB): {
			PC++;
			ADC(ADR_AB, PC, cycles, mem);
		}break;
		case(INS_ADC_AX): {
			PC++;
			ADC(ADR_AX, PC, cycles, mem);
		}break;
		case(INS_ADC_AY): {
			PC++;
			ADC(ADR_AY, PC, cycles, mem);
		}break;
		case(INS_ADC_IX): {
			PC++;
			ADC(ADR_IX, PC, cycles, mem);
		}break;
		case(INS_ADC_IY): {
			PC++;
			ADC(ADR_IY, PC, cycles, mem);
		}break;

			//----------#CMP-----------
		case(INS_CMP_IM): {
			PC++;
			CMP(ADR_IM, PC, cycles, mem);
		}break;
		case(INS_CMP_ZP): {
			PC++;
			CMP(ADR_ZP, PC, cycles, mem);
		}break;
		case(INS_CMP_ZX): {
			PC++;
			CMP(ADR_ZX, PC, cycles, mem);
		}break;
		case(INS_CMP_AB): {
			PC++;
			CMP(ADR_AB, PC, cycles, mem);
		}break;
		case(INS_CMP_AX): {
			PC++;
			CMP(ADR_AX, PC, cycles, mem);
		}break;
		case(INS_CMP_AY): {
			PC++;
			CMP(ADR_AY, PC, cycles, mem);
		}break;
		case(INS_CMP_IX): {
			PC++;
			CMP(ADR_IX, PC, cycles, mem);
		}break;
		case(INS_CMP_IY): {
			PC++;
			CMP(ADR_IY, PC, cycles, mem);
		}break;

			//------------#SUB---------------
		case(INS_SUB_IM): {
			PC++;
			SUB(ADR_IM, PC, cycles, mem);
		}break;
		case(INS_SUB_ZP): {
			PC++;
			SUB(ADR_ZP, PC, cycles, mem);
		}break;
		case(INS_SUB_ZX): {
			PC++;
			SUB(ADR_ZX, PC, cycles, mem);
		}break;
		case(INS_SUB_AB): {
			PC++;
			SUB(ADR_AB, PC, cycles, mem);
		}break;
		case(INS_SUB_AX): {
			PC++;
			SUB(ADR_AX, PC, cycles, mem);
		}break;
		case(INS_SUB_AY): {
			PC++;
			SUB(ADR_AY, PC, cycles, mem);
		}break;
		case(INS_SUB_IX): {
			PC++;
			SUB(ADR_IX, PC, cycles, mem);
		}break;
		case(INS_SUB_IY): {
			PC++;
			SUB(ADR_IY, PC, cycles, mem);
		}break;

			//--------------#AND-------------
		case(INS_AND_IM): {
			PC++;
			AND(ADR_IM, PC, cycles, mem);
		}break;
		case(INS_AND_ZP): {
			PC++;
			AND(ADR_ZP, PC, cycles, mem);
		}break;
		case(INS_AND_ZX): {
			PC++;
			AND(ADR_ZX, PC, cycles, mem);
		}break;
		case(INS_AND_AB): {
			PC++;
			AND(ADR_AB, PC, cycles, mem);
		}break;
		case(INS_AND_AX): {
			PC++;
			AND(ADR_AX, PC, cycles, mem);
		}break;
		case(INS_AND_AY): {
			PC++;
			AND(ADR_AY, PC, cycles, mem);
		}break;
		case(INS_AND_IX): {
			PC++;
			AND(ADR_IX, PC, cycles, mem);
		}break;
		case(INS_AND_IY): {
			PC++;
			AND(ADR_IY, PC, cycles, mem);
		}break;


			//-------------#ASL--------------
		case(INS_ASL_AC): {
			PC++;
			ASL(ADR_AC, PC, cycles, mem);
		}break;
		case(INS_ASL_ZP): {
			PC++;
			ASL(ADR_ZP, PC, cycles, mem);
		}break;
		case(INS_ASL_ZX): {
			PC++;
			ASL(ADR_ZX, PC, cycles, mem);
		}break;
		case(INS_ASL_AB): {
			PC++;
			ASL(ADR_AB, PC, cycles, mem);
		}break;
		case(INS_ASL_AX): {
			PC++;
			ASL(ADR_AX, PC, cycles, mem);
		}break;


			//-------------#SYSTEM-----------
		case(INS_NOP): {
			//This command just skips a line:)
		}break;


		default:
			std::cout << "instruction: " << ins << " not recognised\n";
			std::cout << "PC: " << PC << "\n";
			assert(false);
			break;
		}
		PC++;
	}
}



