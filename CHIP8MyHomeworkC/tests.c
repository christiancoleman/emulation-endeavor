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
bool test8XY6_SetVF();
bool test8XY6_NOSetVF();
bool test8XY7_WithCarry();
bool test8XY7_NOCarry();
bool test8XYE_Store0inVX();
bool test8XYE_Store1inVX();
bool test9XY0_VX_DNE_VY_SKIP();
bool test9XY0_VX_EQUALS_VY_NOSKIP();
bool testANNN();
bool testBNNN();
bool testCNNN();
bool testDNNN();

bool testOpcodes(){
	return
	   test00E0()
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
	&& test8XY6_SetVF()
	&& test8XY6_NOSetVF()
	&& test8XY7_WithCarry()
	&& test8XY7_NOCarry()
	&& test8XYE_Store0inVX()
	&& test8XYE_Store1inVX()
	&& test9XY0_VX_DNE_VY_SKIP()
	&& test9XY0_VX_EQUALS_VY_NOSKIP()
	&& testANNN()
	&& testBNNN()
	&& testCNNN()
	&& testDNNN()
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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	doCycle(true);

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
	char* OP_NAME = "8XY4_WithCarry: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x74;

	V9 = 0x7;
	V7 = 0xFF;

	// RUN TEST!
	doCycle(true);

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
	char* OP_NAME = "8XY4_NOCarry: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x74;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle(true);

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
	char* OP_NAME = "8XY5_WithCarry: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x75;

	V9 = 0x7;
	V7 = 0xFF;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x08){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	//check VF
	if(VF != 1){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY5_NOCarry(){
	char* OP_NAME = "8XY5_NOCarry: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x75;

	V9 = 0x7;
	V7 = 0x6;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x1){
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

bool test8XY6_SetVF(){
	char* OP_NAME = "8XY6_SetVF: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x06;

	V9 = 0x3F;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x1F){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	// check VF
	if(VF != 0x1){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY6_NOSetVF(){
	char* OP_NAME = "8XY6_NOSetVF: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x06;

	V9 = 0x2A;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x15){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	// check VF
	if(VF != 0x0){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY7_WithCarry(){
	char* OP_NAME = "8XY7_WithCarry: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x77;

	V9 = 0x33;
	V7 = 0x7;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0xD4){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	// check VF
	if(VF != 0x1){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XY7_NOCarry(){
	char* OP_NAME = "8XY7_NOCarry: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x77;

	V9 = 0x7;
	V7 = 0x33;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x2C){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	// check VF
	if(VF != 0x0){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XYE_Store0inVX(){
	char* OP_NAME = "8XYE_Store0inVX: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x0E;

	V9 = 0xFE;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0xFC){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	// check VF
	if(VF != 0x1){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test8XYE_Store1inVX(){
	char* OP_NAME = "8XYE_Store1inVX: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x89;
	memory[PC + 1] = 0x0E;

	V9 = 0x7;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0xE){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}

	// check VF
	if(VF != 0x0){
		printf("%s VF - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test9XY0_VX_DNE_VY_SKIP(){
	char* OP_NAME = "9XY0_VX_DNE_VY_SKIP: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x99;
	memory[PC + 1] = 0x00;

	V9 = 0x7;
	V0 = 0x8;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x204) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 == V0){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool test9XY0_VX_EQUALS_VY_NOSKIP(){
	char* OP_NAME = "9XY0_VX_EQUALS_VY_NOSKIP: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x99;
	memory[PC + 1] = 0x00;

	V9 = 0x7;
	V0 = 0x7;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != V0){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool testANNN(){
	char* OP_NAME = "ANNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0xA9;
	memory[PC + 1] = 0x01;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check I
	if(I != 0x901){
		printf("%s I - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool testBNNN(){
	char* OP_NAME = "BNNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0xB9;
	memory[PC + 1] = 0x01;

	V0 = 0x3;

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x904) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

// uses rand() so need a special case in main code
bool testCNNN(){
	char* OP_NAME = "CNNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0xC9;
	memory[PC + 1] = 0x99;

	V9 = 0x23; // arbitrary, we just want to be sure it changes later

	// RUN TEST!
	doCycle(true);

	// check PC
	if(PC != 0x202) {
		printf("%s PC - FAIL\n", OP_NAME);
		return false;
	}

	// check registers
	if(V9 != 0x1){
		printf("%s REGISTERS - FAIL\n", OP_NAME);
		return false;
	}
	return true;
}

bool testDNNN(){
	char* OP_NAME = "DNNN: ";
	initMemory();

	// test conditions
	PC = 0x200;
	memory[PC] = 0x00;
	memory[PC + 1] = 0xE0;

	// RUN TEST!
	doCycle(true);

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