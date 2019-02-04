//char c = 'z';
//char d = 122;
//char e = 0x7A;
//char f = '\x7a';
// ^ all of these are equal

#pragma once

/*
	0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	0x200-0xFFF - Program ROM and work RAM
*/

// Max value for an unsigned char is 0xFF or 255 or 11111111b

unsigned char *memory;
unsigned short *stack;
unsigned short *SP; // stack pointer

// Registers are 16 bits or 2 bytes long

//unsigned int I = 0xFFFFFFFF; // ints are 4 bytes
unsigned short I; // shorts are 2 bytes
unsigned short PC; // programs start at 0x200
unsigned char V0; // char is 1 byte
unsigned char V1;
unsigned char V2;
unsigned char V3;
unsigned char V4;
unsigned char V5;
unsigned char V6;
unsigned char V7;
unsigned char V8;
unsigned char V9;
unsigned char VA;
unsigned char VB;
unsigned char VC;
unsigned char VD;
unsigned char VE;
unsigned char VF; // doubles as a flag (should be avoided)

void initMemory();
void DEBUG_printMemory();

void initMemory(){
	printf("\n");
	printf("######################################################\n");
	printf("############## INITIALIZING MEMORY ###################\n");
	printf("######################################################\n\n");

	memory = calloc( ( 4096 + 1 ), sizeof(unsigned char) );
	stack = calloc( ( 16 + 1 ), sizeof(unsigned char) );
	SP = 0x0000;

	PC = 0x0200; // This is the entry point for all CHIP-8 roms
	I = 0x0000;
	V0 = 0x00;
	V1 = 0x00;
	V2 = 0x00;
	V3 = 0x00;
	V4 = 0x00;
	V5 = 0x00;
	V6 = 0x00;
	V7 = 0x00;
	V8 = 0x00;
	V9 = 0x00;
	VA = 0x00;
	VB = 0x00;
	VC = 0x00;
	VD = 0x00;
	VE = 0x00;
	VF = 0x00;

	printf("Reset all memory and registers...\n");
}

void DEBUG_printMemory(){
	for(int i = 0; i < 4096; i++){
		printf("0x%x: 0x%x \t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\n", i, memory[i]);
	}
}