#pragma warning(disable : 4996)
#include <stdlib.h>

#include <iostream>

#include "CPU.hpp"
#include "constants.hpp"

struct Program {
    Byte* data;
    u64 length;
};

char* loadFile(char* filename) {
    FILE* text = fopen(filename, "r");
    if (text == NULL) {
        std::cout << "File could not be opened\n";
        exit(-1);
    }
}

Program compile(char* filename) {}