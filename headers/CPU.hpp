#pragma once
#include "constants.hpp"
#include "Memory.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <bitset>

class CPU
{

public:
	CPU();

	~CPU();

	Word PC;
	Byte SP;

	Byte A, X, Y; //Accumulator, register X, register Y


	Byte
		C : 1, //Carry
		Z : 1, //Zero
		I : 1, //Interrupt Disable
		D : 1, //Decimal Mode
		B : 1, //Break Command
		V : 1, //Overflow
		N : 1; //Negative

	void init(Memory& mem);

	void printStatus();

	Byte getLowByte(Word);
	Byte getHighByte(Word);

	Byte sByteToWord(Byte x);

	void branch(bool condition, u32& cycles, Memory& mem);

	Byte fetch(u32&, Memory&);
	Word fetchWord(u32&, Memory&);

	Byte readByte(Word address, u32& cycles, Memory& mem);

	Word readWord(Word address, u32& cycles, Memory& mem);

	void writeByte(Word address, Byte val, u32& cycles, Memory& mem);

	void writeWord(Word address, Word val, u32& cycles, Memory& mem);

	Byte load(Byte addrMode, Word dataAddr, u32& cycles, Memory& mem);

	void storeToReg(Byte reg, Byte val);

	void store(Word addrMode, Word dataAddr, Byte data, u32& cycles, Memory& mem);

	void ADC(Byte addrMode, Word addr, u32& cycles, Memory& mem);

	void SUB(Byte addrMode, Word addr, u32& cycles, Memory& mem);

	void AND(Byte addrMode, Word addr, u32& cycles, Memory& mem);

	void ASL(Byte addrMode, Word addr, u32& cycles, Memory& mem);

	void CMP(Byte addrMode, Word addr, u32& cycles, Memory& mem);

	void execute(u32 cycles, bool useCycles, Memory& mem);


private:

};

static void ASSERT_VERBOSE(bool condition, const char* expression, unsigned int line, CPU* cpu) {
	if (!condition) {
		std::cout << std::endl << expression << " failed. On line " << line << std::endl;
		cpu->printStatus();
		assert(false);
	}
};