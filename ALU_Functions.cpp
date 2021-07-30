#pragma once
#include "constants.cpp"

static class ALU
{
public:
	static void ADC(Byte val, Byte& A, Byte& C, Byte& Z, Byte& N, Byte& V) {
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

private:

};

