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

// Registers are 16 bits or 2 bytes long

//unsigned int I = 0xFFFFFFFF; // ints are 4 bytes
unsigned short I = 0x0000; // shorts are 2 bytes
unsigned short PC = 0x0000; // programs start at 0x200
unsigned char V0 = 0x00; // char is 1 byte
unsigned char V1 = 0x00;
unsigned char V2 = 0x00;
unsigned char V3 = 0x00;
unsigned char V4 = 0x00;
unsigned char V5 = 0x00;
unsigned char V6 = 0x00;
unsigned char V7 = 0x00;
unsigned char V8 = 0x00;
unsigned char V9 = 0x00;
unsigned char VA = 0x00;
unsigned char VB = 0x00;
unsigned char VC = 0x00;
unsigned char VD = 0x00;
unsigned char VE = 0x00;
unsigned char VF = 0x00; // doubles as a flag (should be avoided)

unsigned short stack[16];
unsigned short sp; // stack pointer

void initMemory(){
	memory = calloc( ( 4096 + 1 ), sizeof(unsigned char) );
	PC = 0x200;
}