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

unsigned short getOpcode(int index){
	return 0xFF;
}

void startEmulation(){

	printf("\n");
	printf("######################################################\n");
	printf("################ STARTING EMULATION ##N###############\n");
	printf("######################################################\n\n");

	PC = 0x200;

	while(PC != 0x1000){ // TODO: while(stillReading or stillEmulating)
		unsigned short opcode = getOpcode(1);
		switch(opcode) {

			/*case 0x0NNN:
				printf("Found 0x0NNN");
				break;*/

			case 0x00E0: // clear_screen();
				printf("Found 0x00E0");
				break;

			case 0x00EE: // return from subroutine
				printf("Found 0x00EE");
				break;

			/*case 0x1NNN:
				printf("Found 0x1NNN");
				break;*/

			/*case 0x2NNN:
				printf("Found 0x2NNN");
				break;*/

			/*case 0x3XNN:
				printf("Found 0x3XNN");
				break;*/

			/*case 0x4XNN:
				printf("Found 0x4XNN");
				break;*/

			/*case 0x5XY0:
				printf("Found 0x5XY0");
				break;*/

			/*case 0x6XNN:
				printf("Found 0x6XNN");
				break;*/

			/*case 0x7XNN:
				printf("Found 0x7XNN");
				break;*/

			/*case 0x8XY0:
				printf("Found 0x8XY0");
				break;*/

			/*case 0x8XY1:
				printf("Found 0x8XY1");
				break;*/

			/*case 0x8XY2:
				printf("Found 0x8XY2");
				break;*/

			/*case 0x8XY3:
				printf("Found 0x8XY3");
				break;*/

			/*case 0x8XY4:
				printf("Found 0x8XY4");
				break;*/

			/*case 0x8XY5:
				printf("Found 0x8XY5");
				break;*/

			/*case 0x8XY6:
				printf("Found 0x8XY6");
				break;*/

			/*case 0x8XY7:
				printf("Found 0x8XY7");
				break;*/

			/*case 0x8XYE:
				printf("Found 0x8XYE");
				break;*/

			/*case 0x9XY0:
				printf("Found 0x9XY0");
				break;*/

			/*case 0xANNN:
				printf("Found 0xANNN");
				break;*/

			/*case 0xBNNN:
				printf("Found 0xBNNN");
				break;*/

			/*case 0xCXNN:
				printf("Found 0xCXNN");
				break;*/

			/*case 0xDXYN:
				printf("Found 0xDXYN");
				break;*/

			/*case 0xEX9E:
				printf("Found 0xEX9E");
				break;*/

			/*case 0xEXA1:
				printf("Found 0xEXA1");
				break;*/

			/*case 0xFX07:
				printf("Found 0xFX07");
				break;*/

			/*case 0xFX0A:
				printf("Found 0xFX0A");
				break;*/

			/*case 0xFX15:
				printf("Found 0xFX15");
				break;*/

			/*case 0xFX18:
				printf("Found 0xFX18");
				break;*/

			/*case 0xFX1E:
				printf("Found 0xFX1E");
				break;*/

			/*case 0xFX29:
				printf("Found 0xFX29");
				break;*/

			/*case 0xFX33:
				printf("Found 0xFX33");
				break;*/

			/*case 0xFX55:
				printf("Found 0xFX55");
				break;*/

			/*case 0xFX65:
				printf("Found 0xFX65");
				break;*/

			/* you can have any number of case statements */
			default : /* Optional */
				//printf("Instruction not found: %x", opcode);
				printf("");
		}
		PC += 0x1;
		printf("0x%x: 0x%x\t", PC, memory[PC]);
		PC += 0x1;
		printf("0x%x: 0x%x\t", PC, memory[PC]);
		PC += 0x1;
		printf("0x%x: 0x%x\t", PC, memory[PC]);
		PC += 0x1;
		printf("0x%x: 0x%x\n", PC, memory[PC]);
	}

}