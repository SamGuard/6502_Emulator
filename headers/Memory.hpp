#pragma once
#include <assert.h>

#include "constants.hpp"

/*

16 bit address bus (So max address is 2^16-1)

The first 256 bytes (0 - 00FF) is the zero page, the next 256 bytes (0100-01FF)
is reserved for system stack

0x200 - 0x300 is the text output, byte 1 is the cursor position



*/

class Memory {
   private:
    Byte data[MAX_MEM];

   public:
    void init() {
        for (u32 i = 0; i < MAX_MEM; i++) {
            data[i] = 0;
        }
    }

    Byte readByte(Word address) {
        assert(address < MAX_MEM);
        return data[address];
    }

    void writeByte(Word address, Byte val) {
        assert(address < MAX_MEM);
        data[address] = val;
    }
};