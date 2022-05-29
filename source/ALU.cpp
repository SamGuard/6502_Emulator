#include "../headers/CPU.hpp"



//-----------Arithmetic-------------
void CPU::ADC(Byte addrMode, Word addr, u32& cycles, Memory& mem) {
	Byte val = (int8_t)load(addrMode, addr, cycles, mem);
	int8_t sA = A;
	A = val + sA + C;
	Z = A == 0;
	C = 0;

	N = (A & 0b10000000) > 0;

	if ((val & 0b10000000) != (sA & 0b10000000)) {
		return;
	}

	if ((val & 0b10000000) != (A & 0b10000000)) {
		C = 1;
		V = true;
	}
}

void CPU::SUB(Byte addrMode, Word addr, u32& cycles, Memory& mem) {
	int8_t val = load(addrMode, addr, cycles, mem);
	int8_t sA = A;
	A = sA - val - (1 - C);
	Z = A == 0;
	C = 0;


	N = (A & 0b10000000) > 0;

	if ((val & 0b10000000) == (sA & 0b10000000)) {
		return;
	}

	if ((sA & 0b10000000) == (A & 0b10000000)) {
		C = 1;
		V = true;
	}
}


//----------Logic------------
void CPU::AND(Byte addrMode, Word addr, u32& cycles, Memory& mem) {
	int8_t val = load(addrMode, addr, cycles, mem);

	A = val & A;

	Z = A == 0;
	N = (A & 0b10000000) > 0;

}

void CPU::ASL(Byte addrMode, Word addr, u32& cycles, Memory& mem) {
	Byte val = load(addrMode, addr, cycles, mem);

	C = (val & 0b10000000) > 0;

	A = val << 1;

	Z = A == 0;
	N = (A & 0b10000000) > 0;
}

void CPU::CMP(Byte addrMode, Word addr, u32& cycles, Memory& mem) {
	Byte m = load(addrMode, addr, cycles, mem);

	C = A >= m;
	Z = A == m;
	N = ((A - m) & 0b10000000) > 0;
}
