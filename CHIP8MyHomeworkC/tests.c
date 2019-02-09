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
bool test6XNN();
bool test7XNN();
bool test8XY0();
bool test8XY1();
bool test8XY2();
bool test8XY3();
bool test8XY4_WithCarry();
bool test8XY4_NOCarry();
bool test8XY5_WithCarry();
bool test8XY5_NOCarry();
bool test8XY6();
bool test8XY7();
bool test8XYE();

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
	&& test6XNN()
	&& test7XNN()
	&& test8XY0()
	&& test8XY1()
	&& test8XY2()
	&& test8XY3()
	&& test8XY4_WithCarry()
	&& test8XY4_NOCarry()
	&& test8XY5_WithCarry()
	&& test8XY5_NOCarry()
	&& test8XY6()
	&& test8XY7()
	&& test8XYE()
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

bool test6XNN(){
	char* OP_NAME = "6XNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x69;
	memory[PC + 1] = 0x77;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x77){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test7XNN(){
	char* OP_NAME = "7XNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x79;
	memory[PC + 1] = 0x77;

	V9 = 0x1;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x78){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY0(){
	char* OP_NAME = "8XY0: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x70;

	V9 = 0x1;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != V7){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY1(){
	char* OP_NAME = "8XY1: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x71;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x37){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY2(){
	char* OP_NAME = "8XY2: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x72;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x3){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
	return true;
}

bool test8XY3(){
	char* OP_NAME = "8XY3: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x73;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x34){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY4_WithCarry(){
	char* OP_NAME = "test8XY4_WithCarry: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x74;

	V9 = 0x7;
	V7 = 0xFF;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x06){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	// check VF
	if(VF != 1){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY4_NOCarry(){
	char* OP_NAME = "test8XY4_NOCarry: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x74;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x3A){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	//check VF
	if(VF != 0){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY5_WithCarry(){
	char* OP_NAME = "8XY5: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x75;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x37){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY5_NOCarry(){
	char* OP_NAME = "8XY5: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x75;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x37){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY6(){
	char* OP_NAME = "8XY6: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x76;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x37){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY7(){
	char* OP_NAME = "8XY7: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x77;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x37){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XYE(){
	char* OP_NAME = "8XYE: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x7E;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle();

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x37){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}