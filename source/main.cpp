#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <bitset>
#include <iostream>

#include "../headers/CPU.hpp"
#include "../headers/constants.hpp"

const Byte OUTPUT = 254;

void writeProgram(Byte* program, u32 programStart, u32 length, Memory& mem) {
    for (unsigned int i = 0; i < length; i++) {
        mem.writeByte(programStart + i, program[i]);
    }
}
void writeProgram(char* program, u32 programStart, u32 length, Memory& mem) {
    for (unsigned int i = 0; i < length; i++) {
        mem.writeByte(programStart + i, (Byte)program[i]);
    }
}

void basicProgram(Memory& mem) {
    Byte program[] = {INS_NOP, INS_LDA_ZP, 0x0080, INS_RTS};
    mem.writeByte(0x0080, 63);
    writeProgram(program, 600, sizeof(program) / sizeof(*program), mem);
}

void additionProgram(CPU cpu, Memory& mem) {
    const Byte VLA = 0;
    const Byte VLB = 2;

    Byte program[] = {0x0,        INS_CLC,    INS_LDA_ZP, VLA + 0,
                      INS_ADC_ZP, VLB + 0,    INS_STA_ZP, OUTPUT,
                      INS_LDA_ZP, VLA + 1,    INS_ADC_ZP, VLB + 1,
                      INS_STA_ZP, OUTPUT + 1, INS_RTS};

    u32 cycles = 0;  // Hack to get it working
    cpu.writeWord(VLA, 200, cycles, mem);
    cpu.writeWord(VLB, 300, cycles, mem);

    writeProgram(program, 600, sizeof(program) / sizeof(*program), mem);
}

void substitutionProgram(CPU cpu, Memory& mem, Word programStart) {
    const Byte VLA = 0;
    const Byte VLB = 2;

    Byte program[] = {0x0,        INS_SEC,    INS_LDA_ZP, VLA + 0,
                      INS_SUB_ZP, VLB + 0,    INS_STA_ZP, OUTPUT,
                      INS_LDA_ZP, VLA + 1,    INS_SUB_ZP, VLB + 1,
                      INS_STA_ZP, OUTPUT + 1, INS_RTS};

    u32 cycles = 0;  // Hack to get it working
    cpu.writeWord(VLA, 200, cycles, mem);
    cpu.writeWord(VLB, 300, cycles, mem);

    writeProgram(program, programStart, sizeof(program) / sizeof(*program),
                 mem);
}

void atariFirmware(CPU cpu, Memory& mem, Word programStart) {
    FILE* file;
    char buff[2048];
    Byte program[MAX_MEM];
		unsigned int ptr = 0;

    file = fopen("./firmware/7800.ROM", "r");
    if (file == NULL) {
        std::cout << "Could not open file" << std::endl;
        return;
    }

    while (fgets(buff, 2048, file) != NULL) {
			for(int i = 0; i < 2048; i++) {
				program[ptr+i] = (Byte)buff[i];
			}
			ptr += 2048;
    }
	
		writeProgram(program, programStart, ptr, mem);
}

void readTextOut(Memory& mem) {
    //Byte cursor = mem.readByte(TEXT_OUT_START);
    Word i = TEXT_OUT_START + 1;
    while (i <= TEXT_OUT_END) {
        std::cout << mem.readByte(i);
        i++;
    }
}

int main() {
    CPU cpu;
    Memory mem;
    cpu.init(mem);

    //substitutionProgram(cpu, mem, PROGRAM_START);
		atariFirmware(cpu, mem, PROGRAM_START);

    cpu.execute(0, false, mem);
    readTextOut(mem);

    int16_t result = mem.readByte(OUTPUT) | (mem.readByte(OUTPUT + 1) << 8);
    std::bitset<16> resultBits(result);

    std::cout << resultBits << std::endl;
    std::cout << result << std::endl;

    return 0;
}