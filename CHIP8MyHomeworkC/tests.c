#pragma once

#include "memory.c"
#include "cpu.c"
#include "gpu.c"
#include "SDL2/SDL.h"

bool testOpcodes();
bool test00E0();
bool test00EE();
bool test0NNN();
bool test1NNN();
bool test2NNN();
bool test3XNN_Vx_DNE_NN_NOSKIP();
bool test3XNN_Vx_EQUALS_NN_SKIP();
bool test4XNN_Vx_DNE_NN_SKIP();
bool test4XNN_Vx_EQUALS_NN_NOSKIP();
bool test5XY0_VX_EQUALS_VY_SKIP();
bool test5XY0_VX_DNE_VY_NOSKIP();

bool testOpcodes(){
	return test00E0()
	&& test00EE()
	&& test0NNN()
	&& test1NNN()
	&& test2NNN()
	&& test3XNN_Vx_DNE_NN_NOSKIP()
	&& test3XNN_Vx_EQUALS_NN_SKIP()
	&& test4XNN_Vx_DNE_NN_SKIP()
	&& test4XNN_Vx_EQUALS_NN_NOSKIP()
	&& test5XY0_VX_EQUALS_VY_SKIP()
	&& test5XY0_VX_DNE_VY_NOSKIP()
	;
}

bool test00E0(){
	char* OP_NAME = "00E0: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x00;
	memory[PC + 1] = 0xE0;

	// RUN TEST!
	doCycle();

	// TODO!!!!!
	// CLEAR SCREEN CHECK
	// Make sure gfx array is all 0s?

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check stack
	if(stack[0] != 0x0){
		printf("%s Stack - FAIL\n", OP_NAME);
		return false;
	}

	return true;
}

bool test00EE(){
	char* OP_NAME = "00EE: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x00;
	memory[PC + 1] = 0xEE;
	stack[0] = 0x401;
	stack[1] = 0x345;
	stack[2] = 0x224;
	SP = 3;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x224) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check stack
	if(stack[0] != 0x401){
		printf("%s Stack1 - FAIL\n", OP_NAME);
		return false;
	}
	if(stack[1] != 0x345){
		printf("%s Stack2 - FAIL\n", OP_NAME);
		return false;
	}

	// check stack pointer
	if(SP != 2){
		printf("%s SP - FAIL\n", OP_NAME);
		return false;
	}

	return true;
}

bool test0NNN(){
	char* OP_NAME = "0NNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x03;
	memory[PC + 1] = 0x21;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x321) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check stack
	if(stack[0] != 0x202){
		printf("%s Stack - FAIL\n", OP_NAME);
		return false;
	}

	return true;
}


bool test1NNN(){
	char* OP_NAME = "1NNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x12;
	memory[PC + 1] = 0x73;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x273) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	return true;
}

bool test2NNN(){
	char* OP_NAME = "2NNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x29;
	memory[PC + 1] = 0x00;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x900) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check stack
	if(stack[0] != 0x202){
		printf("%s Stack - FAIL\n", OP_NAME);
		return false;
	}

	return true;
}

bool test3XNN_Vx_DNE_NN_NOSKIP(){
	char* OP_NAME = "3XNN_Vx_DNE_NN_NOSKIP: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x39;
	memory[PC + 1] = 0x66;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test3XNN_Vx_EQUALS_NN_SKIP(){
	char* OP_NAME = "3XNN_Vx_EQUALS_NN_SKIP: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x39;
	memory[PC + 1] = 0x66;

	V9 = 0x66;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x204) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test4XNN_Vx_DNE_NN_SKIP(){
	char* OP_NAME = "4XNN_Vx_DNE_NN_SKIP: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x49;
	memory[PC + 1] = 0x66;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x204) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test4XNN_Vx_EQUALS_NN_NOSKIP(){
	char* OP_NAME = "4XNN_Vx_EQUALS_NN_NOSKIP: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x49;
	memory[PC + 1] = 0x66;

	V9 = 0x66;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test5XY0_VX_EQUALS_VY_SKIP(){
	char* OP_NAME = "5XY0_VX_EQUALS_VY_SKIP: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x59;
	memory[PC + 1] = 0x60;

	V6 = 0x66;
	V9 = 0x66;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x204) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test5XY0_VX_DNE_VY_NOSKIP(){
	char* OP_NAME = "5XY0_VX_DNE_VY_NOSKIP: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x59;
	memory[PC + 1] = 0x60;

	V6 = 0x99;
	V9 = 0x66;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}