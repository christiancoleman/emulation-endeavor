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

unsigned short getOpcode(int index){
	unsigned short result = 0x0000;	// Example: 0xA2B4 in memory
	result = memory[index] << 8; 	// 0xA2 into 0xA200
	result += memory[index + 1];	// 0xA200 + 0xB4
	return result;					// return 0xA2B4
}

void startEmulation(){

	printf("\n");
	printf("######################################################\n");
	printf("################ STARTING EMULATION ##################\n");
	printf("######################################################\n\n");

	while(PC != 0x1000) { // TODO: while(stillReading or stillEmulating)


		printf("#################################################\n");

		unsigned short opcode = getOpcode(PC);

		/*printf("~~~opcode~~~ %x\n", opcode);
		printf("~~~firstbyte~~~ %x\n", getFirstByte(opcode));
		printf("~~~lastbyte~~~ %x\n", getLastByte(opcode));
		printf("~~~1nib1byte~~~ %x\n", getHighNibble(getFirstByte(opcode)));
		printf("~~~2nib1byte~~~ %x\n", getLowNibble(getFirstByte(opcode)));
		printf("~~~2nib1byte~~~ %x\n", getHighNibble(getLastByte(opcode)));
		printf("~~~2nib2byte~~~ %x\n", getLowNibble(getLastByte(opcode)));*/

		// Special zero case #1
		// Clears the screen.
		// type: Display
		// template: 00E0
		// disp_clear()
		if(opcode == 0x00E0) {
			clearScreen();
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
			call(opcode);
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
		// 	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// template: 3XNN
		// if(Vx!=NN)
		else if( (0x3000 <= opcode) && (opcode <= 0x3FFF) ){
			unsigned short opcodeClean = removeBase(opcode, 0x3000);

			// get NN
			unsigned char lastByte = getLastByte(opcodeClean);

			// get Vx
			unsigned short lowNibbleOnFirstByteSHORT = opcodeClean - lastByte;
			lowNibbleOnFirstByteSHORT = lowNibbleOnFirstByteSHORT >> 8;
			unsigned char lowNibbleOnFirstByteCHAR = lowNibbleOnFirstByteSHORT;
			unsigned char *dynamicRegister = getRegister(lowNibbleOnFirstByteCHAR);

			if(*dynamicRegister == lastByte){
				skipNextInstruction();
			} else {
				nextInstruction();
			}
		}

		// All 0x4*** cases are the same
		// Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// template: 4XNN
		// if(Vx==NN)
		else if( (0x4000 <= opcode) && (opcode <= 0x4FFF) ){
			unsigned short opcodeClean = removeBase(opcode, 0x4000);

			// get NN
			unsigned char lastByte = getLastByte(opcodeClean);

			// get Vx
			unsigned short lowNibbleOnFirstByteSHORT = opcodeClean - lastByte;
			lowNibbleOnFirstByteSHORT = lowNibbleOnFirstByteSHORT >> 8;
			unsigned char lowNibbleOnFirstByteCHAR = lowNibbleOnFirstByteSHORT;
			unsigned char *dynamicRegister = getRegister(lowNibbleOnFirstByteCHAR);

			if(*dynamicRegister != lastByte){
				skipNextInstruction();
			} else {
				nextInstruction();
			}
		}

		// All 0x5*** cases are the same
		// Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// template: 5XY0
		// if(Vx!=Vy)
		else if( (0x5000 <= opcode) && (opcode <= 0x5FFF) ){
			unsigned short opcodeClean = removeBase(opcode, 0x5000);

			// get last byte (usually NN but that's not needed for this operation but is needed for lowNibble)
			unsigned char lastByte = getLastByte(opcodeClean);

			// get Vx
			unsigned short lowNibbleOnFirstByteSHORT = opcodeClean - lastByte;
			lowNibbleOnFirstByteSHORT = lowNibbleOnFirstByteSHORT >> 8;
			unsigned char lowNibbleOnFirstByteCHAR = lowNibbleOnFirstByteSHORT;
			unsigned char *XdynamicRegister = getRegister(lowNibbleOnFirstByteCHAR);

			// get Vy
			unsigned char highNibbleOnSecondByteCHAR = getHighNibble(lastByte);
			unsigned char *YdynamicRegister = getRegister(highNibbleOnSecondByteCHAR);

			if(*XdynamicRegister == *YdynamicRegister){
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
			unsigned short lowNibbleOnFirstByteSHORT = opcodeClean - lastByte;
			lowNibbleOnFirstByteSHORT = lowNibbleOnFirstByteSHORT >> 8;
			unsigned char lowNibbleOnFirstByteCHAR = lowNibbleOnFirstByteSHORT;
			unsigned char *dynamicRegister = getRegister(lowNibbleOnFirstByteCHAR);

			*dynamicRegister = lastByte;

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
			unsigned short lowNibbleOnFirstByteSHORT = opcodeClean - lastByte;
			lowNibbleOnFirstByteSHORT = lowNibbleOnFirstByteSHORT >> 8;
			unsigned char lowNibbleOnFirstByteCHAR = lowNibbleOnFirstByteSHORT;
			unsigned char *dynamicRegister = getRegister(lowNibbleOnFirstByteCHAR);

			*dynamicRegister = *dynamicRegister + lastByte;

			nextInstruction();
		}

		// 8 ////////////////////////////////////////////////

		// TODO: COME BACK TO THIS
		/*else if( (0x8000 <= opcode) && (opcode <= 0x8FFF) ){
			unsigned short opcodeClean = removeBase(opcode, 0x7000);
		}*/

		// Sets VX to the value of VY.
		// type: Assign
		// template: 8XY0
		// Vx=Vy
		/*case 0x8XY0:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Sets VX to VX or VY. (Bitwise OR operation)
		// type: BitOp
		// template: 8XY1
		// Vx=Vx|Vy
		/*case 0x8XY1:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Sets VX to VX and VY. (Bitwise AND operation)
		// type: BitOp
		// template: 8XY2
		// Vx=Vx&Vy
		/*case 0x8XY2:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Sets VX to VX xor VY.
		// type: BitOp
		// template: 8XY3
		// Vx=Vx^Vy
		/*case 0x8XY3:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
		// type: Math
		// template: 8XY4
		// Vx += Vy
		/*case 0x8XY4:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
		// type: Math
		// template: 8XY5
		// Vx -= Vy
		/*case 0x8XY5:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Stores the least significant bit of VX in VF and then shifts VX to the right by 1
		// type: BitOp
		// template: 8XY6
		// Vx>>=1
		/*case 0x8XY6:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
		// type: Math
		// template: 8XY7
		// Vx=Vy-Vx
		/*case 0x8XY7:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Stores the most significant bit of VX in VF and then shifts VX to the left by 1
		// type: BitOp
		// template: 8XYE
		// Vx<<=1
		/*case 0x8XYE:
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// All 0x9*** cases are the same
		// Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// template: 9XY0
		// if(Vx==Vy)
		else if( (0x9000 <= opcode) && (opcode <= 0x9FFF) ){
			unsigned short opcodeClean = removeBase(opcode, 0x9000);

			// get last byte (usually NN but that's not needed for this operation but is needed for lowNibble)
			unsigned char lastByte = getLastByte(opcodeClean);

			// get Vx
			unsigned short lowNibbleOnFirstByteSHORT = opcodeClean - lastByte;
			lowNibbleOnFirstByteSHORT = lowNibbleOnFirstByteSHORT >> 8;
			unsigned char lowNibbleOnFirstByteCHAR = lowNibbleOnFirstByteSHORT;
			unsigned char *XdynamicRegister = getRegister(lowNibbleOnFirstByteCHAR);

			// get Vy
			unsigned char highNibbleOnSecondByteCHAR = getHighNibble(lastByte);
			unsigned char *YdynamicRegister = getRegister(highNibbleOnSecondByteCHAR);

			if(*XdynamicRegister != *YdynamicRegister){
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
		// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		// type: Random
		// template: CXNN
		// Vx=rand()&NN
		else if( (0xC000 <= opcode) && (opcode <= 0xCFFF) ){
			unsigned short opcodeClean = removeBase(opcode, 0xC000);

			// get NN
			unsigned char lastByte = getLastByte(opcodeClean);

			// get Vx
			unsigned short lowNibbleOnFirstByteSHORT = opcodeClean - lastByte;
			lowNibbleOnFirstByteSHORT = lowNibbleOnFirstByteSHORT >> 8;
			unsigned char lowNibbleOnFirstByteCHAR = lowNibbleOnFirstByteSHORT;
			unsigned char *XdynamicRegister = getRegister(lowNibbleOnFirstByteCHAR);

			*XdynamicRegister = rand() & lastByte;

			nextInstruction();
		}

		// All 0xD*** cases are the same
		// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
		// type: Display
		// template: DXYN
		// draw(Vx,Vy,N)
		else if( (0xD000 <= opcode) && (opcode <= 0xDFFF) ){
			unsigned short opcodeClean = removeBase(opcode, 0xD000);
			draw();
			nextInstruction();
		}

		// E ////////////////////////////////////////////////

		// Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
		// type: KeyOp
		// template: 0xEX9E
		// if(key()==Vx)
		/*case 0xEX9E:
			unsigned short opcodeClean = removeBase(opcode, 0xE000);
			break;*/

		// 	Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
		// type: KeyOp
		// template: EXA1
		// if(key()!=Vx)
		/*case 0xEXA1:
			unsigned short opcodeClean = removeBase(opcode, 0xE000);
			break;*/

		// F ////////////////////////////////////////////////

		// Sets VX to the value of the delay timer.
		// type: Timer
		// template: FX07
		// Vx = get_delay()
		/*case 0xFX07:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
		// type: KeyOp
		// template: FX0A
		// Vx = get_key()
		/*case 0xFX0A:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Sets the delay timer to VX.
		// type: Timer
		// template: FX15
		// delay_timer(Vx)
		/*case 0xFX15:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Sets the sound timer to VX.
		// type: Sound
		// template: FX18
		// sound_timer(Vx)
		/*case 0xFX18:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Adds VX to I
		// type: Memory
		// template: FX1E
		// I +=Vx
		/*case 0xFX1E:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
		// type: Memory
		// template: FX29
		// I=sprite_addr[Vx]
		/*case 0xFX29:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
		// type: BCD
		// template: FX33
		// set_BCD(Vx);
		// *(I+0)=BCD(3);
		// *(I+1)=BCD(2);
		// *(I+2)=BCD(1);
		/*case 0xFX33:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
		// type: Memory
		// template: FX55
		// reg_dump(Vx,&I)
		/*case 0xFX55:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
		// type: Memory
		// template: FX65
		// reg_load(Vx,&I)
		/*case 0xFX65:
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		else {
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("Instruction not found at: %x with value of: %x\n", PC, opcode);
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			PC += 0x2;
		}

		DEBUG_printStack();
		DEBUG_printState(opcode);

	}
}

unsigned short removeBase(unsigned short opcode, unsigned short base){
	return opcode - base;
}

void pushAddressToStack(){
	if(stack != SP){
		SP += 0x01;
	}

	// store the address of the instruction after the call
	*SP = PC + 0x02;
}

void popAddressFromStack(){
	// pull return address from the stack
	PC = *SP;

	// stacks usually don't set the popped value to null, but it'll help me with visualization
	*SP = 0x0;

	// popped return address means
	// stack pointer needs to move to previous value
	if(SP != stack){
		SP -= 0x01;
	}
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