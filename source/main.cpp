#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <bitset>
#include "../headers/constants.hpp"
#include "../headers/CPU.hpp"

const Byte OUTPUT = 254;



void writeProgram(Byte* program, u32 programStart, u32 length, Memory& mem) {
	for (unsigned int i = 0; i < length; i++) {
		mem.writeByte(programStart + i, program[i]);
	}
}

void basicProgram(Memory& mem) {
	Byte program[] = {
			INS_NOP,
			INS_LDA_ZP,
			0x0080,
			INS_RTS
	};
	mem.writeByte(0x0080, 63);
	writeProgram(program, 600, sizeof(program)/sizeof(*program), mem);
}


void additionProgram(CPU cpu, Memory& mem) {
	const Byte VLA = 0;
	const Byte VLB = 2;


	Byte program[] = {
		0x0,
		INS_CLC,
		INS_LDA_ZP,
		VLA+0,
		INS_ADC_ZP,
		VLB+0,
		INS_STA_ZP,
		OUTPUT,
		INS_LDA_ZP,
		VLA + 1,
		INS_ADC_ZP,
		VLB + 1,
		INS_STA_ZP,
		OUTPUT + 1,
		INS_RTS
	};

	u32 cycles = 0;//Hack to get it working
	cpu.writeWord(VLA, 200, cycles, mem);
	cpu.writeWord(VLB, 300, cycles, mem);



	writeProgram(program, 600, sizeof(program) / sizeof(*program), mem);
}


void substitutionProgram(CPU cpu, Memory& mem, Word programStart) {
	const Byte VLA = 0;
	const Byte VLB = 2;


	Byte program[] = {
		0x0,
		INS_SEC,
		INS_LDA_ZP,
		VLA + 0,
		INS_SUB_ZP,
		VLB + 0,
		INS_STA_ZP,
		OUTPUT,
		INS_LDA_ZP,
		VLA + 1,
		INS_SUB_ZP,
		VLB + 1,
		INS_STA_ZP,
		OUTPUT + 1,
		INS_RTS
	};

	u32 cycles = 0;//Hack to get it working
	cpu.writeWord(VLA, 200, cycles, mem);
	cpu.writeWord(VLB, 300, cycles, mem);



	writeProgram(program, programStart, sizeof(program) / sizeof(*program), mem);
}


void readTextOut(Memory& mem) {
	Byte cursor = mem.readByte(TEXT_OUT_START);
	Word i = TEXT_OUT_START + 1;
	while (i <= TEXT_OUT_END) {
		if (cursor + 1 == i - TEXT_OUT_START) {
			std::cout << "_";
		}
		else {
			std::cout << mem.readByte(i);
		}
		i++;
	}
}

int main() {
	CPU cpu;
	Memory mem;
	cpu.init(mem);
	
	
	substitutionProgram(cpu, mem, PROGRAM_START);
	

	cpu.execute(100, true, mem);

	int16_t result = mem.readByte(OUTPUT) | (mem.readByte(OUTPUT + 1) << 8);
	std::bitset<16> resultBits(result);

	std::cout << resultBits << std::endl;
	std::cout << result << std::endl;

	readTextOut(mem);

	return 1;
}