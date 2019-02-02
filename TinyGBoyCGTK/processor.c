#pragma once

#include <stdbool.h>
#include <unistd.h>

struct processor {
	// numer of elapsed ticks since the start of execution
	int elapsedTicks;

	// the memory address of the next instruction to be fetched
	char PC;

	// the memory address of the top of the stack
	char SP;

	// general purpose registers
	char A,F;
	char B,C;
	char D,E;
	char H,L;

	// set if the result of the operation is zero
	bool ZF;

	// set if the operation was subtraction
	bool OF;

	// set if there was an overflow in the lower half of the result
	bool HCF;

	// set if there was an overflow in the result
	bool CF;
};

void executeCPU(int cycles_to_execute) {
	printf("Emulating CPU now...");
	sleep(cycles_to_execute);
}