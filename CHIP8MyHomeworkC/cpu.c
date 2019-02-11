/*
	NNN: address
	NN: 8-bit constant
	N: 4-bit constant
	X and Y: 4-bit register identifier
	PC: Program Counter
	I: 16-bit register
*/

#pragma once

#include <time.h>
#include "memory.c"
#include "gpu.c"

unsigned short getOpcode(int);
void doCycle(bool);
unsigned short removeBase(unsigned short, unsigned short);
void pushAddressToStack();
void popAddressFromStack();
void call(unsigned short);
void gotoAddr(unsigned short);
void nextInstruction();
void skipNextInstruction();
unsigned char getHighNibble(unsigned char);
unsigned char getLowNibble(unsigned char);
unsigned char getFirstByte(unsigned short);
unsigned char getLastByte(unsigned short);
void instructionNotFound(unsigned short);
void instructionNotImplemented(unsigned short);

unsigned short getOpcode(int index){
	unsigned short result = 0x0000;	// Example: 0xA2B4 in memory
	result = memory[index] << 8; 	// 0xA2 into 0xA200
	result += memory[index + 1];	// 0xA200 + 0xB4
	return result;					// return 0xA2B4
}

void doCycle(bool IS_RUNNING_TESTS){
	unsigned short opcode = getOpcode(PC);

	if(IS_RUNNING_TESTS){
		printf("#################################################\n");
		DEBUG_printMemory();
		printf("\nPC: \t\t%x\n", PC);
	}

	// Special zero case #1
	// Clears the screen.
	// type: Display
	// template: 00E0
	// disp_clear()
	if(opcode == 0x00E0) {
		clearScreen();
		shouldDraw = true;
		nextInstruction();
	}

	// Special zero case #2
	// return from subroutine
	// type: Flow
	// template: 00EE
	// return
	else if(opcode == 0x00EE) {
		popAddressFromStack();
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// We are NOT incrementing PC so this code won't work
		// unless all function calls are implemented correctly
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	// Generic zero case catch
	// Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
	// type: Call
	// template: 0NNN
	// call NNN
	else if( (0x0000 <= opcode) && (opcode <= 0x0FFF) ){
		// for consistency, but this does nothing
		unsigned short opcodeClean = removeBase(opcode, 0x0000);
		call(opcodeClean);
	}

	// All 0x1*** cases are the same
	// Jumps to address NNN.
	// type: Flow
	// template: 1NNN
	// goto NNN
	else if( (0x1000 <= opcode) && (opcode <= 0x1FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x1000);
		gotoAddr(opcodeClean);
	}

	// All 0x2*** cases are the same
	// Calls subroutine at NNN.
	// type: Flow
	// template: 2NNN
	// *(0xNNN)()
	else if( (0x2000 <= opcode) && (opcode <= 0x2FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x2000);
		call(opcodeClean);
	}

	// All 0x3*** cases are the same
	// 	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to
	// 		skip a code block)
	// type: Condition
	// template: 3XNN
	// if(Vx!=NN)
	else if( (0x3000 <= opcode) && (opcode <= 0x3FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x3000);

		// get NN
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		if(*VXdynamicRegister == lastByte){
			skipNextInstruction();
		} else {
			nextInstruction();
		}
	}

	// All 0x4*** cases are the same
	// Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a
	// 		jump to skip a code block)
	// type: Condition
	// template: 4XNN
	// if(Vx==NN)
	else if( (0x4000 <= opcode) && (opcode <= 0x4FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x4000);

		// get NN
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		if(*VXdynamicRegister != lastByte){
			skipNextInstruction();
		} else {
			nextInstruction();
		}
	}

	// All 0x5*** cases are the same
	// Skips the next instruction if VX equals VY. (Usually the next instruction is a jump
	// 		to skip a code block)
	// type: Condition
	// template: 5XY0
	// if(Vx!=Vy)
	else if( (0x5000 <= opcode) && (opcode <= 0x5FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x5000);

		// get last byte (usually NN but that's not needed for this operation but is needed
		// 		for lowNibble)
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		// get Vy
		unsigned char highNibbleOnSecondByte = getHighNibble(lastByte);
		unsigned char *VYdynamicRegister = getRegister(highNibbleOnSecondByte);

		if(*VXdynamicRegister == *VYdynamicRegister){
			skipNextInstruction();
		} else {
			nextInstruction();
		}
	}

	// All 0x6*** cases are the same
	// Sets VX to NN
	// type: Constant
	// template: 6XNN
	// Vx = NN
	else if( (0x6000 <= opcode) && (opcode <= 0x6FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x6000);

		// get NN
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		*VXdynamicRegister = lastByte;

		nextInstruction();
	}

	// All 0x7*** cases are the same
	// Adds NN to VX. (Carry flag is not changed)
	// type: Constant
	// template: 7XNN
	// Vx += NN
	else if( (0x7000 <= opcode) && (opcode <= 0x7FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x7000);

		// get NN
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		*VXdynamicRegister = *VXdynamicRegister + lastByte;

		nextInstruction();
	}

	// 8 ////////////////////////////////////////////////

	else if( (0x8000 <= opcode) && (opcode <= 0x8FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x8000);

		unsigned char lastByte = getLastByte(opcodeClean);
		unsigned char lowNibbleOnLastByte = getLowNibble(lastByte);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		// get Vy
		unsigned char highNibbleOnSecondByte = getHighNibble(lastByte);
		unsigned char *VYdynamicRegister = getRegister(highNibbleOnSecondByte);

		// used later sometimes
		unsigned short result = 0x00;

		switch(lowNibbleOnLastByte){

			// Sets VX to the value of VY.
			// type: Assign
			// template: 8XY0
			// Vx=Vy
			case 0x0:
				*VXdynamicRegister = *VYdynamicRegister;
				break;

			// Sets VX to VX or VY. (Bitwise OR operation)
			// type: BitOp
			// template: 8XY1
			// Vx=Vx|Vy
			case 0x1:
				*VXdynamicRegister = *VXdynamicRegister | *VYdynamicRegister;
				break;

			// Sets VX to VX and VY. (Bitwise AND operation)
			// type: BitOp
			// template: 8XY2
			// Vx=Vx&Vy
			case 0x2:
				*VXdynamicRegister = *VXdynamicRegister & *VYdynamicRegister;
				break;

			// Sets VX to VX xor VY.
			// type: BitOp
			// template: 8XY3
			// Vx=Vx^Vy
			case 0x3:
				*VXdynamicRegister = *VXdynamicRegister ^ *VYdynamicRegister;
				break;

			// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
			// type: Math
			// template: 8XY4
			// Vx += Vy
			case 0x4: ;
				result = *VXdynamicRegister + *VYdynamicRegister;
				if(result > 0xFF){
					VF = 0x1;
				} else {
					VF = 0x0;
				}
				*VXdynamicRegister += *VYdynamicRegister;
				break;

			// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when
			// 		there isn't.
			// type: Math
			// template: 8XY5
			// Vx -= Vy
			case 0x5:
				result = *VXdynamicRegister - *VYdynamicRegister;
				if(result > 0xFF){
					VF = 0x0;
				} else {
					VF = 0x1;
				}
				*VXdynamicRegister -= *VYdynamicRegister;
				break;

			// Stores the least significant bit of VX in VF and then shifts VX to the right by 1
			// type: BitOp
			// template: 8XY6
			// Vx>>=1
			case 0x6:
				result = *VXdynamicRegister & 0x1;
				VF = result;
				*VXdynamicRegister >>= 1;
				break;

			// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there
			// 		isn't.
			// type: Math
			// template: 8XY7
			// Vx=Vy-Vx
			case 0x7:
				result = *VYdynamicRegister - *VXdynamicRegister;
				if(result > 0xFF){
					VF = 0x0;
				} else {
					VF = 0x1;
				}
				*VXdynamicRegister = *VYdynamicRegister - *VXdynamicRegister;
				break;

			// Stores the most significant bit of VX in VF and then shifts VX to the left by 1
			// type: BitOp
			// template: 8XYE
			// Vx<<=1
			case 0xE:
				result = *VXdynamicRegister >> 7;
				VF = result;
				*VXdynamicRegister <<= 1;
				break;

			default:
				instructionNotFound(opcode);
				return;
		}

		nextInstruction();
	}

	// All 0x9*** cases are the same
	// Skips the next instruction if VX doesn't equal VY. (Usually the next instruction
	// 		is a jump to skip a code block)
	// type: Condition
	// template: 9XY0
	// if(Vx==Vy)
	else if( (0x9000 <= opcode) && (opcode <= 0x9FFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0x9000);

		// get last byte (usually NN but that's not needed for this operation but is needed for lowNibble)
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		// get Vy
		unsigned char highNibbleOnSecondByte = getHighNibble(lastByte);
		unsigned char *VYdynamicRegister = getRegister(highNibbleOnSecondByte);

		if(*VXdynamicRegister != *VYdynamicRegister){
			skipNextInstruction();
		} else {
			nextInstruction();
		}
	}

	// All 0x0A*** cases are the same
	// Sets I to the address NNN.
	// type: Memory
	// template: ANNN
	// I = NNN
	else if( (0xA000 <= opcode) && (opcode <= 0xAFFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0xA000);
		I = opcodeClean;
		nextInstruction();
	}

	// All 0xB*** cases are the same
	// Jumps to the address NNN plus V0.
	// type: Flow
	// template: BNNN
	// PC=V0+NNN
	else if( (0xB000 <= opcode) && (opcode <= 0xBFFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0xB000);
		gotoAddr(V0 + opcodeClean);
	}

	// All 0xC*** cases are the same
	// Sets VX to the result of a bitwise and operation on a random number (Typically:
	// 		0 to 255) and NN.
	// type: Random
	// template: CXNN
	// Vx=rand()&NN
	else if( (0xC000 <= opcode) && (opcode <= 0xCFFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0xC000);

		// get NN
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		if(IS_RUNNING_TESTS){
			*VXdynamicRegister = 0x7 & lastByte; // hardcoding for tests
		} else {
			srand(time(0));
			unsigned char randomChar = rand() % 255;
			*VXdynamicRegister = randomChar & lastByte;
		}

		nextInstruction();
	}

	// All 0xD*** cases are the same
	// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height
	// 		of N pixels. Each row of 8 pixels is read as bit-coded starting from memory
	// 		location I; I value doesn’t change after the execution of this instruction.
	// 		As described above, VF is set to 1 if any screen pixels are flipped from
	// 		set to unset when the sprite is drawn, and to 0 if that doesn’t happen
	// type: Display
	// template: DXYN
	// draw(Vx,Vy,N)
	else if( (0xD000 <= opcode) && (opcode <= 0xDFFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0xD000);

		// get last byte (usually NN but that's not needed for this operation but is needed for lowNibble)
		unsigned char lastByte = getLastByte(opcodeClean);

		// get N or height
		unsigned char lowNibbleOnSecondByte = getLowNibble(lastByte);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		// get Vy
		unsigned char highNibbleOnSecondByte = getHighNibble(lastByte);
		unsigned char *VYdynamicRegister = getRegister(highNibbleOnSecondByte);

		// store backup of old frame
		/*for(int i = 0; i < SCREEN_WIDTH; i++){
			for(int j = 0; j < SCREEN_HEIGHT; j++){
				oldFrame[i][j] = newFrame[i][j];
			}
		}*/

		// for prettier reading code
		unsigned char height = lowNibbleOnSecondByte;
		unsigned char VX = *VXdynamicRegister;
		unsigned char VY = *VYdynamicRegister;

		unsigned char pixelToDraw = 0x0;

		// add new sprites to new frame
		for(int i = 0; i < height; i++){

			// each pixel is on a new line and is 8 bits wide
			pixelToDraw = memory[I + i];

			// each sprite is 8 pixels wide
			for(int j = 0; j < 8; j++){

				// 0x80 is 10000000b
				// Compares 10000000, 01000000, 00100000, etc. to new pixel
				// if pixel has a bit in that spot it will trigger;
				// actually filling frame with new data
				// if new frame already had data there then we set VF to 1
				if((pixelToDraw & (0x80 >> j)) != 0) {
					if(newFrame[VX+j][VY+i] == 1) {
						VF = 0x1;
					}
					newFrame[VX+j][VY+i] ^= 1;
				}
			}
		}

		shouldDraw = true;

		nextInstruction();
	}

	// E ////////////////////////////////////////////////

	else if( (0xE000 <= opcode) && (opcode <= 0xEFFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0xE000);

		// we match the instruction on the last byte
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		switch(lastByte){

			// Skips the next instruction if the key stored in VX is pressed. (Usually
			// 		the next instruction is a jump to skip a code block)
			// type: KeyOp
			// template: 0xEX9E
			// if(key()==Vx)
			case 0x9E:
				if(keys[*VXdynamicRegister] == 0x1){
					skipNextInstruction();
				} else {
					nextInstruction();
				}
				break;

			// 	Skips the next instruction if the key stored in VX isn't pressed.
			// 		(Usually the next instruction is a jump to skip a code block)
			// type: KeyOp
			// template: EXA1
			// if(key()!=Vx)
			case 0xA1:
				if(keys[*VXdynamicRegister] != 0x1){
					skipNextInstruction();
				} else{
					nextInstruction();
				}
				break;

			default:
				instructionNotFound(opcode);
				return;
		}
	}

	// F ////////////////////////////////////////////////

	else if( (0xF000 <= opcode) && (opcode <= 0xFFFF) ){
		unsigned short opcodeClean = removeBase(opcode, 0xF000);

		// we match the instruction on the last byte
		unsigned char lastByte = getLastByte(opcodeClean);

		// get Vx
		unsigned char lowNibbleOnFirstByte = getLowNibble(getFirstByte(opcodeClean));
		unsigned char *VXdynamicRegister = getRegister(lowNibbleOnFirstByte);

		switch(lastByte){

			// Sets VX to the value of the delay timer.
			// type: Timer
			// template: FX07
			// Vx = get_delay()
			case 0x07:
				*VXdynamicRegister = DT;
				break;

			// A key press is awaited, and then stored in VX. (Blocking Operation. All
			// 		instruction halted until next key event)
			// type: KeyOp
			// template: FX0A
			// Vx = get_key()
			case 0x0A:
				for(int i = 0; i < 16; i++){
					if(keys[i] == 0x1){
						*VXdynamicRegister = i;
					}
				}
				break;

			// Sets the delay timer to VX.
			// type: Timer
			// template: FX15
			// delay_timer(Vx)
			case 0x15:
				DT = *VXdynamicRegister;
				break;

			// Sets the sound timer to VX.
			// type: Sound
			// template: FX18
			// sound_timer(Vx)
			case 0x18:
				ST = *VXdynamicRegister;
				break;

			// Adds VX to I
			// type: Memory
			// template: FX1E
			// I +=Vx
			case 0x1E:
				I += *VXdynamicRegister;
				// IS THIS NEEDED??????????????????????????????????????????????????????????????????????????????
				/*if(I > 0xFFF){
					VF = 1;
				} else {
					VF = 0;
				}*/
				break;

			// Sets I to the location of the sprite for the character in VX. Characters 0-F
			// 		(in hexadecimal) are represented by a 4x5 font.
			// type: Memory
			// template: FX29
			// I=sprite_addr[Vx]
			case 0x29:
				I = *VXdynamicRegister * 0x5;
				break;

			// Stores the binary-coded decimal representation of VX, with the most significant
			// 		of three digits at the address in I, the middle digit at I plus 1, and
			// 		the least significant digit at I plus 2. (In other words, take the decimal
			// 		representation of VX, place the hundreds digit in memory at location in I,
			// 		 the tens digit at location I+1, and the ones digit at location I+2.)
			// type: BCD
			// template: FX33
			// set_BCD(Vx);
			// *(I+0)=BCD(3);
			// *(I+1)=BCD(2);
			// *(I+2)=BCD(1);
			case 0x33:
					memory[I] = *VXdynamicRegister / 100;
					memory[I + 1] = (*VXdynamicRegister / 10) % 10;
					memory[I + 2] = *VXdynamicRegister % 10;
				break;

			// Stores V0 to VX (including VX) in memory starting at address I. The offset from
			// 		I is increased by 1 for each value written, but I itself is left unmodified.
			// type: Memory
			// template: FX55
			// reg_dump(Vx,&I)
			case 0x55:
				for(int i = 0; i <= lowNibbleOnFirstByte; i++){
					unsigned char *VtempDynamicRegister = getRegister(i);
					memory[I + i] = *VtempDynamicRegister;
				}
				break;

			// Fills V0 to VX (including VX) with values from memory starting at address I.
			// 		The offset from I is increased by 1 for each value written, but I itself
			// 		is left unmodified.
			// type: Memory
			// template: FX65
			// reg_load(Vx,&I)
			case 0x65:
				for(int i = 0; i <= lowNibbleOnFirstByte; i++){
					unsigned char *VtempDynamicRegister = getRegister(i);
					*VtempDynamicRegister = memory[I + i];
				}
				// I've seen this done differently a few places
				// https://github.com/Spittie/chip8-sdl/blob/master/src/main.c
				// ^ just adds 1
				// whereas
				// http://www.codeslinger.co.uk/pages/projects/chip8/examples.html
				// ^ does + 1 and + X (from VX)

				// https://en.wikipedia.org/wiki/CHIP-8#cite_note-onlgame-4
				// ^ suggests doing nothing though
				break;

			default:
				instructionNotFound(opcode);
				return;
		}

		nextInstruction();
	}

	else {
		instructionNotFound(opcode);
		return;
	}

	if(IS_RUNNING_TESTS){
		DEBUG_printStack();
		DEBUG_printState(opcode);
	}

	// Update timers
	// Update timers
	if(DT > 0) {
		--DT;
	} else {
		DT = 0x3C; // not sure if this is what we should actually do
	}

	if(ST > 0){
		if(ST == 1){
			//printf("BEEP!");
		}
		--ST;
	} else {
		ST = 0x3C; // not sure if this is what we should actually do
	}

}

unsigned short removeBase(unsigned short opcode, unsigned short base){
	return opcode - base;
}

void pushAddressToStack(){
	// store the address of the instruction after the call
	stack[SP] = PC + 0x02;

	SP += 1;
}

void popAddressFromStack(){
	// pull return address from the stack
	PC = stack[SP - 1];

	// stacks usually don't set the popped value to null, but it'll help me with visualization
	stack[SP - 1] = 0x0;

	SP -= 1;
}

void call(unsigned short addr){
	pushAddressToStack();
	PC = addr;
}

void gotoAddr(unsigned short addr){
	PC = addr;
}

void nextInstruction(){
	PC += 0x02;
}

void skipNextInstruction(){
	PC += 0x04;
}

unsigned char getHighNibble(unsigned char c){
	c = c >> 4;
	c = c & 0x0F;
	return c;
}

unsigned char getLowNibble(unsigned char c){
	c = c & 0x0F;
	return c;
}

unsigned char getFirstByte(unsigned short s){
	s = s >> 8;
	unsigned char c = s;
	return c;
}

unsigned char getLastByte(unsigned short s){
	// overflow should leave only what we want
	unsigned char c = s;
	return c;
}

void instructionNotFound(unsigned short opcode){
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("Instruction NOT FOUND at: %x with value of: %x\n", PC, opcode);
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}

void instructionNotImplemented(unsigned short opcode){
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("Instruction NOT IMPLEMENTED at: %x with value of: %x\n", PC, opcode);
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}