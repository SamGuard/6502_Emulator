#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using Byte = u8;
using Word = u16;



constexpr u8 MAX_INT = 255;
const u16 WORD_LENGTH = 16;

const Word PROGRAM_START = 0x300;

const Word TEXT_OUT_START = 0x200;
const Word TEXT_OUT_END = 0x2FF;

static constexpr Byte
	ADR_AC = 0,
	ADR_IM = 1,
	ADR_ZP = 2,
	ADR_ZX = 3,
	ADR_ZY = 4,
	ADR_AB = 5,
	ADR_AX = 6,
	ADR_AY = 7,
	ADR_ID = 8,
	ADR_IX = 9,
	ADR_IY = 10
	;

static constexpr Byte
	REG_A = 0,
	REG_X = 1,
	REG_Y = 2
	;


static constexpr Byte
	//Loading
	INS_LDA_IM = 0xA9,
	INS_LDA_ZP = 0xA5,
	INS_LDA_ZX = 0xB5,
	INS_LDA_AB = 0xAD,
	INS_LDA_AX = 0xBD,
	INS_LDA_AY = 0xB9,
	INS_LDA_IX = 0xA1,
	INS_LDA_IY = 0xB1,

	INS_LDX_IM = 0xA2,
	INS_LDX_ZP = 0xA6,
	INS_LDX_ZY = 0xB6,
	INS_LDX_AB = 0xAE,
	INS_LDX_AY = 0xBE,

	INS_LDY_IM = 0xA0,
	INS_LDY_ZP = 0xA4,
	INS_LDY_ZX = 0xB4,
	INS_LDY_AB = 0xAC,
	INS_LDY_AX = 0xBC,

	//Storing
	INS_STA_ZP = 0x85,
	INS_STA_ZX = 0x95,
	INS_STA_AB = 0x8D,
	INS_STA_AX = 0x9D,
	INS_STA_AY = 0x99,
	INS_STA_IX = 0x81,
	INS_STA_IY = 0x91,

	INS_STX_ZP = 0x86,
	INS_STX_ZY = 0x96,
	INS_STX_AB = 0x8E,

	INS_STY_ZP = 0x84,
	INS_STY_ZX = 0x94,
	INS_STY_AB = 0x8C,

	//Maf
	INS_ADC_IM = 0x69,
	INS_ADC_ZP = 0x65,
	INS_ADC_ZX = 0x75,
	INS_ADC_AB = 0x6D,
	INS_ADC_AX = 0x7D,
	INS_ADC_AY = 0x79,
	INS_ADC_IX = 0x61,
	INS_ADC_IY = 0x71,

	INS_SUB_IM = 0xE9,
	INS_SUB_ZP = 0xE5,
	INS_SUB_ZX = 0xF5,
	INS_SUB_AB = 0xED,
	INS_SUB_AX = 0xFD,
	INS_SUB_AY = 0xF9,
	INS_SUB_IX = 0xE1,
	INS_SUB_IY = 0xF1,

	//Logic
	INS_AND_IM = 0x29,
	INS_AND_ZP = 0x25,
	INS_AND_ZX = 0x35,
	INS_AND_AB = 0x2D,
	INS_AND_AX = 0x3D,
	INS_AND_AY = 0x39,
	INS_AND_IX = 0x21,
	INS_AND_IY = 0x31,

	INS_ASL_AC = 0x0A,
	INS_ASL_ZP = 0x06,
	INS_ASL_ZX = 0x16,
	INS_ASL_AB = 0x0E,
	INS_ASL_AX = 0x1E,

	INS_CMP_IM = 0xC9,
	INS_CMP_ZP = 0xC5,
	INS_CMP_ZX = 0xD5,
	INS_CMP_AB = 0xCD,
	INS_CMP_AX = 0xDD,
	INS_CMP_AY = 0xD9,
	INS_CMP_IX = 0xC1,
	INS_CMP_IY = 0xD1,

	//Branching
	INS_BCC = 0x90,//Carry clear
	INS_BCS = 0xB0,//Carry set
	INS_BEQ = 0xF0,//Equal
	INS_BMI = 0x30,//Negative
	INS_BNE = 0xD0,//Not equal
	INS_BPL = 0x10,//Positive
	INS_BVC = 0x50,//Overflow clear
	INS_BVS = 0x70,//Overflow set

	//Bit test
	INS_BIT_ZP = 0x24,
	INS_BIT_AB = 0x2C,

	//System
	INS_NOP = 0xEA,//No operation

	INS_JSR = 0x20,
	INS_RTS = 0x60,

	INS_CLC = 0x18,
	INS_SEC = 0xF8
	;


#endif
