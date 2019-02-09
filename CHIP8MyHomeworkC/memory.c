#pragma once

/*
	0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	0x200-0xFFF - Program ROM and work RAM
*/

// Max value for an unsigned char is 0xFF or 255 or 11111111b

unsigned char *memory;
unsigned short *stack;

unsigned int SP; // stack pointer

unsigned short I; 	// index register

unsigned char key; 	// which key is pressed

unsigned char DT; 	// delay timer
unsigned char ST; 	// sound timer

unsigned short PC; 	// programs start at 0x200
unsigned char V0;
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
unsigned char VF; 	// doubles as a flag (should be avoided)

void initMemory();
unsigned char* getRegister(unsigned char);
void DEBUG_printMemory();
void DEBUG_printState(unsigned short);
void DEBUG_printStack();

void initMemory(){
	printf("\n");
	printf("######################################################\n");
	printf("############## INITIALIZING MEMORY ###################\n");
	printf("######################################################\n\n");

	memory = calloc( ( 4096 + 1 ), sizeof(unsigned char) );
	stack = calloc( ( 16 + 1 ), sizeof(unsigned short) );

	SP = 0;

	I = 0x0000;

	key = 0x00;

	DT = 0x3C; 	// might want to set these to 60 or 0x3C
	ST = 0x3C;

	PC = 0x0200; // This is the entry point for all CHIP-8 roms
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

unsigned char* getRegister(unsigned char character){
	if(character == 0x00){
		return &V0;
	}
	else if(character == 0x01){
		return &V1;
	}
	else if(character == 0x02){
		return &V2;
	}
	else if(character == 0x03){
		return &V3;
	}
	else if(character == 0x04){
		return &V4;
	}
	else if(character == 0x05){
		return &V5;
	}
	else if(character == 0x06){
		return &V6;
	}
	else if(character == 0x07){
		return &V7;
	}
	else if(character == 0x08){
		return &V8;
	}
	else if(character == 0x09){
		return &V9;
	}
	else if(character == 0x0A){
		return &VA;
	}
	else if(character == 0x0B){
		return &VB;
	}
	else if(character == 0x0C){
		return &VC;
	}
	else if(character == 0x0D){
		return &VD;
	}
	else if(character == 0x0E){
		return &VE;
	}
	else if(character == 0x0F){
		return &VF;
	}
}

void DEBUG_printMemory(){
	for(int i = 0; i < 1024; i++){
		// first 8
		printf("0x%x: 0x%x \t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x \t", i, memory[i]);
		i++;
		// next 8
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\t", i, memory[i]);
		i++;
		printf("0x%x: 0x%x\n", i, memory[i]);
	}
}

void DEBUG_printState(unsigned short opcode){
	printf("\n");
	printf("Opcode: \t%x\n", opcode);
	printf("\n");
	printf("Stack: \t\t%x\n", stack);
	printf("\n");
	printf("SP: \t\t%x\n", SP);
	printf("\n");
	printf("PC: \t\t%x\n", PC);
	printf("\n");
	printf("I: \t\t%x\n", I);
	printf("\n");
	printf("key: \t\t%x\n", key);
	printf("\n");
	printf("DT: \t\t%x\n", DT);
	printf("ST: \t\t%x\n", ST);
	printf("\n");
	printf("V0: \t\t%x\n", V0);
	printf("V1: \t\t%x\n", V1);
	printf("V2: \t\t%x\n", V2);
	printf("V3: \t\t%x\n", V3);
	printf("V4: \t\t%x\n", V4);
	printf("V5: \t\t%x\n", V5);
	printf("V6: \t\t%x\n", V6);
	printf("V7: \t\t%x\n", V7);
	printf("V8: \t\t%x\n", V8);
	printf("V9: \t\t%x\n", V9);
	printf("VA: \t\t%x\n", VA);
	printf("VB: \t\t%x\n", VB);
	printf("VC: \t\t%x\n", VC);
	printf("VD: \t\t%x\n", VD);
	printf("VE: \t\t%x\n", VE);
	printf("VF: \t\t%x\n", VF);

	printf("\nPress enter to continue stepping through the code...\n");
	int d = getchar();
}

void DEBUG_printStack(){
	printf("\n");
	printf("~~~Stack~~~\n");
	for(int i = 0; i < 16; i++){
		printf("stack[%i]: \t%x\n", i, stack[i]);
	}
}