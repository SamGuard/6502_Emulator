#include "CPU.h"

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