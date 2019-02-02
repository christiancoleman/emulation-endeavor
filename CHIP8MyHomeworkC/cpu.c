/*
	NNN: address
	NN: 8-bit constant
	N: 4-bit constant
	X and Y: 4-bit register identifier
	PC: Program Counter
	I: 16-bit register
*/

#pragma once

#include "memory.c"
#include "gpu.c"

unsigned short getOpcode(int);
void startEmulation();

unsigned short getOpcode(int index){
	unsigned short result = 0x0000;
	result = memory[index] << 8; // 0xA2 into 0xA200
	result += memory[index + 1];
	return result;
}

void startEmulation(){

	printf("\n");
	printf("######################################################\n");
	printf("################ STARTING EMULATION ##################\n");
	printf("######################################################\n\n");

	while(PC != 0x1000) { // TODO: while(stillReading or stillEmulating)
		unsigned short opcode = getOpcode(PC);

		if(opcode == 0x00E0) {
			printf("Found 0x00E0 at: %x\n", PC);
			clearScreen();
		} else if(opcode == 0x00EE) { // return from subroutine
			printf("Found 0x00EE at: %x\n", PC);
		}

		/*case 0x0NNN:
			printf("Found 0x0NNN"); // 	Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
			break;*/

		/*case 0x1NNN:
			printf("Found 0x1NNN at: %x\n", PC);
			break;*/

		/*case 0x2NNN:
			printf("Found 0x2NNN at: %x\n", PC);
			break;*/

		/*case 0x3XNN:
			printf("Found 0x3XNN at: %x\n", PC);
			break;*/

		/*case 0x4XNN:
			printf("Found 0x4XNN at: %x\n", PC);
			break;*/

		/*case 0x5XY0:
			printf("Found 0x5XY0 at: %x\n", PC);
			break;*/

		/*case 0x6XNN:
			printf("Found 0x6XNN at: %x\n", PC);
			break;*/

		/*case 0x7XNN:
			printf("Found 0x7XNN at: %x\n", PC);
			break;*/

		/*case 0x8XY0:
			printf("Found 0x8XY0 at: %x\n", PC);
			break;*/

		/*case 0x8XY1:
			printf("Found 0x8XY1 at: %x\n", PC);
			break;*/

		/*case 0x8XY2:
			printf("Found 0x8XY2 at: %x\n", PC);
			break;*/

		/*case 0x8XY3:
			printf("Found 0x8XY3 at: %x\n", PC);
			break;*/

		/*case 0x8XY4:
			printf("Found 0x8XY4 at: %x\n", PC);
			break;*/

		/*case 0x8XY5:
			printf("Found 0x8XY5 at: %x\n", PC);
			break;*/

		/*case 0x8XY6:
			printf("Found 0x8XY6 at: %x\n", PC);
			break;*/

		/*case 0x8XY7:
			printf("Found 0x8XY7 at: %x\n", PC);
			break;*/

		/*case 0x8XYE:
			printf("Found 0x8XYE at: %x\n", PC);
			break;*/

		/*case 0x9XY0:
			printf("Found 0x9XY0 at: %x\n", PC);
			break;*/

		/*case 0xANNN:
			printf("Found 0xANNN at: %x\n", PC);
			break;*/

		/*case 0xBNNN:
			printf("Found 0xBNNN at: %x\n", PC);
			break;*/

		/*case 0xCXNN:
			printf("Found 0xCXNN at: %x\n", PC);
			break;*/

		/*case 0xDXYN:
			printf("Found 0xDXYN at: %x\n", PC);
			break;*/

		/*case 0xEX9E:
			printf("Found 0xEX9E at: %x\n", PC);
			break;*/

		/*case 0xEXA1:
			printf("Found 0xEXA1 at: %x\n", PC);
			break;*/

		/*case 0xFX07:
			printf("Found 0xFX07 at: %x\n", PC);
			break;*/

		/*case 0xFX0A:
			printf("Found 0xFX0A at: %x\n", PC);
			break;*/

		/*case 0xFX15:
			printf("Found 0xFX15 at: %x\n", PC);
			break;*/

		/*case 0xFX18:
			printf("Found 0xFX18 at: %x\n", PC);
			break;*/

		/*case 0xFX1E:
			printf("Found 0xFX1E at: %x\n", PC);
			break;*/

		/*case 0xFX29:
			printf("Found 0xFX29 at: %x\n", PC);
			break;*/

		/*case 0xFX33:
			printf("Found 0xFX33 at: %x\n", PC);
			break;*/

		/*case 0xFX55:
			printf("Found 0xFX55 at: %x\n", PC);
			break;*/

		/*case 0xFX65:
			printf("Found 0xFX65 at: %x\n", PC);
			break;*/

		//printf("Instruction not found: %x at %x\n", opcode, PC);

		PC += 0x2; // opcodes are 2 bytes long, so we read them in that increment
	}

}