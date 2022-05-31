#include "../headers/CPU.hpp"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <bitset>
#include <iostream>

void CPU::printStatus() {
    std::cout << std::endl << "PC: " << (uint32_t)PC << std::endl;
    std::cout << "A: " << (uint32_t)A << " X: " << (uint32_t)X
              << " Y:" << (uint32_t)Y << std::endl;
    Byte status = C << 6 | Z << 5 | I << 4 | D << 3 | B << 2 | V << 1 | N;
    std::cout << "CZIDBVN" << std::endl << std::bitset<7>(status) << std::endl;
}

CPU::CPU() {
    A = X = Y = 0;                  // Registers
    C = Z = I = D = B = V = N = 0;  // Flags

    PC = 0xFFFC;
    SP = 0x00FF;
}

CPU::~CPU() {}

void CPU::init(Memory& mem) {
    A = X = Y = 0;                  // Registers
    C = Z = I = D = B = V = N = 0;  // Flags

    PC = 0xFFFC;
    SP = 0x00FF;

    mem.init();

    // Bootstrap
    mem.writeByte(0xFFFC, INS_JSR);
    mem.writeByte(0xFFFD, getLowByte(PROGRAM_START));
    mem.writeByte(0xFFFE, getHighByte(PROGRAM_START));
    mem.writeByte(0xFFFF, INS_NOP);
    //-------

    // Text output
    const char* inputMessage = "Welcome to samos\n";

    unsigned int i = 0;
    while (inputMessage[i] != '\0' && TEXT_OUT_START + i + 1 <= TEXT_OUT_END) {
        mem.writeByte(TEXT_OUT_START + i + 1, inputMessage[i]);
        i++;
    }
    mem.writeByte(TEXT_OUT_START, i);
}

Byte CPU::getLowByte(Word val) { return (0b0000000011111111 & val); }

Byte CPU::getHighByte(Word val) { return (0b1111111100000000 & val) >> 8; }

// Converting twos compliment word from a byte to a word
Byte CPU::sByteToWord(Byte x) {
    return ((0b11111111 << 8) * ((0b10000000 & x) > 0)) | x;
}

void CPU::branch(bool condition, u32& cycles, Memory& mem) {
    PC++;
    if (condition) {
        PC += sByteToWord(readByte(PC, cycles, mem));
    }
}

void CPU::execute(u32 cycles, bool useCycles, Memory& mem) {
    while ((cycles > 0 || !useCycles) && PC != 0xFFFF) {
        Word ins = fetch(cycles, mem);
        std::cout << PC << " " << ins << std::endl;

        switch (ins) {
                //------------#LDA--------------
            case (INS_LDA_IM): {
                PC++;
                storeToReg(REG_A, load(ADR_IM, PC, cycles, mem));
            } break;
            case (INS_LDA_ZP): {
                PC++;
                storeToReg(REG_A, load(ADR_ZP, PC, cycles, mem));
            } break;
            case (INS_LDA_ZX): {
                PC++;
                storeToReg(REG_A, load(ADR_ZX, PC, cycles, mem));
            } break;
            case (INS_LDA_AB): {
                PC++;
                storeToReg(REG_A, load(ADR_AB, PC, cycles, mem));
            } break;
            case (INS_LDA_AX): {
                PC++;
                storeToReg(REG_A, load(ADR_AX, PC, cycles, mem));
            } break;
            case (INS_LDA_AY): {
                PC++;
                storeToReg(REG_A, load(ADR_AY, PC, cycles, mem));
            } break;
            case (INS_LDA_IX): {
                PC++;
                storeToReg(REG_A, load(ADR_IX, PC, cycles, mem));
            } break;
            case (INS_LDA_IY): {
                PC++;
                storeToReg(REG_A, load(ADR_IY, PC, cycles, mem));
            } break;

                //-----------#LDX---------------
            case (INS_LDX_IM): {
                PC++;
                storeToReg(REG_X, load(ADR_IM, PC, cycles, mem));
            } break;
            case (INS_LDX_ZP): {
                PC++;
                storeToReg(REG_X, load(ADR_ZP, PC, cycles, mem));
            } break;
            case (INS_LDX_ZY): {
                PC++;
                storeToReg(REG_X, load(ADR_ZY, PC, cycles, mem));
            } break;
            case (INS_LDX_AB): {
                PC++;
                storeToReg(REG_X, load(ADR_AB, PC, cycles, mem));
            } break;
            case (INS_LDX_AY): {
                PC++;
                storeToReg(REG_X, load(ADR_AY, PC, cycles, mem));
            } break;

                //-----------#LDY---------------
            case (INS_LDY_IM): {
                PC++;
                storeToReg(REG_Y, load(ADR_IM, PC, cycles, mem));
            } break;
            case (INS_LDY_ZP): {
                PC++;
                storeToReg(REG_Y, load(ADR_ZP, PC, cycles, mem));
            } break;
            case (INS_LDY_ZX): {
                PC++;
                storeToReg(REG_Y, load(ADR_ZX, PC, cycles, mem));
            } break;
            case (INS_LDY_AB): {
                PC++;
                storeToReg(REG_Y, load(ADR_AB, PC, cycles, mem));
            } break;
            case (INS_LDY_AX): {
                PC++;
                storeToReg(REG_Y, load(ADR_AX, PC, cycles, mem));
            } break;

                //------------#STA--------------
            case (INS_STA_ZP): {
                PC++;
                store(ADR_ZP, PC, A, cycles, mem);
            } break;
            case (INS_STA_ZX): {
                PC++;
                store(ADR_ZX, PC, A, cycles, mem);
            } break;
            case (INS_STA_AB): {
                PC++;
                store(ADR_AB, PC, A, cycles, mem);
            } break;
            case (INS_STA_AX): {
                PC++;
                store(ADR_AX, PC, A, cycles, mem);
            } break;
            case (INS_STA_AY): {
                PC++;
                store(ADR_AY, PC, A, cycles, mem);
            } break;
            case (INS_STA_IX): {
                PC++;
                store(ADR_IX, PC, A, cycles, mem);
            } break;
            case (INS_STA_IY): {
                PC++;
                store(ADR_IY, PC, A, cycles, mem);
            } break;

                //-----------#STX----------------
            case (INS_STX_ZP): {
                PC++;
                store(ADR_ZP, PC, X, cycles, mem);
            } break;
            case (INS_STX_ZY): {
                PC++;
                store(ADR_ZY, PC, X, cycles, mem);
            } break;
            case (INS_STX_AB): {
                PC++;
                store(ADR_AB, PC, X, cycles, mem);
            } break;

                //-----------#STY----------------
            case (INS_STY_ZP): {
                PC++;
                store(ADR_ZP, PC, Y, cycles, mem);
            } break;
            case (INS_STY_ZX): {
                PC++;
                store(ADR_ZX, PC, Y, cycles, mem);
            } break;
            case (INS_STY_AB): {
                PC++;
                store(ADR_AB, PC, Y, cycles, mem);
            } break;

                //----------#ADC-----------
            case (INS_ADC_IM): {
                PC++;
                ADC(ADR_IM, PC, cycles, mem);
            } break;
            case (INS_ADC_ZP): {
                PC++;
                ADC(ADR_ZP, PC, cycles, mem);
            } break;
            case (INS_ADC_ZX): {
                PC++;
                ADC(ADR_ZX, PC, cycles, mem);
            } break;
            case (INS_ADC_AB): {
                PC++;
                ADC(ADR_AB, PC, cycles, mem);
            } break;
            case (INS_ADC_AX): {
                PC++;
                ADC(ADR_AX, PC, cycles, mem);
            } break;
            case (INS_ADC_AY): {
                PC++;
                ADC(ADR_AY, PC, cycles, mem);
            } break;
            case (INS_ADC_IX): {
                PC++;
                ADC(ADR_IX, PC, cycles, mem);
            } break;
            case (INS_ADC_IY): {
                PC++;
                ADC(ADR_IY, PC, cycles, mem);
            } break;

                //----------#CMP-----------
            case (INS_CMP_IM): {
                PC++;
                CMP(ADR_IM, PC, cycles, mem);
            } break;
            case (INS_CMP_ZP): {
                PC++;
                CMP(ADR_ZP, PC, cycles, mem);
            } break;
            case (INS_CMP_ZX): {
                PC++;
                CMP(ADR_ZX, PC, cycles, mem);
            } break;
            case (INS_CMP_AB): {
                PC++;
                CMP(ADR_AB, PC, cycles, mem);
            } break;
            case (INS_CMP_AX): {
                PC++;
                CMP(ADR_AX, PC, cycles, mem);
            } break;
            case (INS_CMP_AY): {
                PC++;
                CMP(ADR_AY, PC, cycles, mem);
            } break;
            case (INS_CMP_IX): {
                PC++;
                CMP(ADR_IX, PC, cycles, mem);
            } break;
            case (INS_CMP_IY): {
                PC++;
                CMP(ADR_IY, PC, cycles, mem);
            } break;

                //------------#SUB---------------
            case (INS_SUB_IM): {
                PC++;
                SUB(ADR_IM, PC, cycles, mem);
            } break;
            case (INS_SUB_ZP): {
                PC++;
                SUB(ADR_ZP, PC, cycles, mem);
            } break;
            case (INS_SUB_ZX): {
                PC++;
                SUB(ADR_ZX, PC, cycles, mem);
            } break;
            case (INS_SUB_AB): {
                PC++;
                SUB(ADR_AB, PC, cycles, mem);
            } break;
            case (INS_SUB_AX): {
                PC++;
                SUB(ADR_AX, PC, cycles, mem);
            } break;
            case (INS_SUB_AY): {
                PC++;
                SUB(ADR_AY, PC, cycles, mem);
            } break;
            case (INS_SUB_IX): {
                PC++;
                SUB(ADR_IX, PC, cycles, mem);
            } break;
            case (INS_SUB_IY): {
                PC++;
                SUB(ADR_IY, PC, cycles, mem);
            } break;

                //--------------#AND-------------
            case (INS_AND_IM): {
                PC++;
                AND(ADR_IM, PC, cycles, mem);
            } break;
            case (INS_AND_ZP): {
                PC++;
                AND(ADR_ZP, PC, cycles, mem);
            } break;
            case (INS_AND_ZX): {
                PC++;
                AND(ADR_ZX, PC, cycles, mem);
            } break;
            case (INS_AND_AB): {
                PC++;
                AND(ADR_AB, PC, cycles, mem);
            } break;
            case (INS_AND_AX): {
                PC++;
                AND(ADR_AX, PC, cycles, mem);
            } break;
            case (INS_AND_AY): {
                PC++;
                AND(ADR_AY, PC, cycles, mem);
            } break;
            case (INS_AND_IX): {
                PC++;
                AND(ADR_IX, PC, cycles, mem);
            } break;
            case (INS_AND_IY): {
                PC++;
                AND(ADR_IY, PC, cycles, mem);
            } break;

                //-------------#ASL--------------
            case (INS_ASL_AC): {
                PC++;
                ASL(ADR_AC, PC, cycles, mem);
            } break;
            case (INS_ASL_ZP): {
                PC++;
                ASL(ADR_ZP, PC, cycles, mem);
            } break;
            case (INS_ASL_ZX): {
                PC++;
                ASL(ADR_ZX, PC, cycles, mem);
            } break;
            case (INS_ASL_AB): {
                PC++;
                ASL(ADR_AB, PC, cycles, mem);
            } break;
            case (INS_ASL_AX): {
                PC++;
                ASL(ADR_AX, PC, cycles, mem);
            } break;

                //----------#Branching---------

            case (INS_BCC): {
                branch(C == 0, cycles, mem);
            } break;
            case (INS_BCS): {
                branch(C == 1, cycles, mem);
            } break;
            case (INS_BEQ): {
                branch(Z == 1, cycles, mem);
            } break;
            case (INS_BMI): {
                branch(N == 1, cycles, mem);
            } break;
            case (INS_BNE): {
                branch(Z == 0, cycles, mem);
            } break;
            case (INS_BPL): {
                branch(N == 0, cycles, mem);
            } break;
            case (INS_BVC): {
                branch(V == 0, cycles, mem);
            } break;
            case (INS_BVS): {
                branch(V == 1, cycles, mem);
            } break;

                //-----------#JSR---------------
            case (INS_JSR): {
                // Push onto stack
                ASSERT_VERBOSE(SP > 1, "SP > 1", __LINE__, this);

                writeWord(SP + 0x0100 - 1, PC + 2, cycles, mem);
                SP -= 2;
                PC++;
                cycles -= 2;
                PC = fetchWord(cycles, mem);
            } break;

            case (INS_RTS): {
                ASSERT_VERBOSE(SP <= 253, "SP <= 253", __LINE__, this);
                SP += 2;
                PC = readWord(SP + 0x0100 - 1, cycles, mem);
                cycles -= 2;
            } break;

                //----------#Flags---------
            case (INS_CLC): {
                C = 0;
            } break;
            case (INS_SEC): {
                C = 1;
            } break;

                //-------------#SYSTEM-----------
            case (INS_NOP): {
                // This command just skips a line:)
            } break;

            default:
                std::cout << "instruction: " << ins << " not recognised\n";
                std::cout << "PC: " << PC << "\n";
                assert(false);
                break;
        }
        PC++;
    }
}
