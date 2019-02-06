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
void DEBUG_dumpState();
unsigned short removeBase(unsigned short, unsigned short);
void pushAddressToStack();
void popAddressFromStack();
void call(unsigned short);
void gotoAddr(unsigned short);
void nextInstruction();
void skipNextInstruction();
unsigned char getHighNibble(unsigned char);
unsigned char getLowNibble(unsigned char);
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
		unsigned short opcode = getOpcode(PC);
		printf("#################################################\n");
		printf("Location: %x\n", PC);
		printf("Opcode: %x\n", opcode);

		// Special zero case #1
		// Clears the screen.
		// type: Display
		// template: 00E0
		// disp_clear()
		if(opcode == 0x00E0) {
			printf("Found 0x00E0 at: %x\n", PC);
			clearScreen();
			nextInstruction();
		}

		// Special zero case #2
		// return from subroutine
		// type: Flow
		// template: 00EE
		// return
		else if(opcode == 0x00EE) {
			printf("Found 0x00EE at: %x\n", PC);
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
			printf("Found 0x0NNN at: %x with value of: %x\n", PC, opcode);
			pushAddressToStack();
			call(opcode); // go to NNN, since it begins with a zero we don't need any bitwise stuff to get the address
		}

		// All 0x1*** cases are the same
		// Jumps to address NNN.
		// type: Flow
		// template: 1NNN
		// goto NNN
		else if( (0x1000 <= opcode) && (opcode <= 0x1FFF) ){
			printf("Found 0x1NNN at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0x1000);
			gotoAddr(opcodeClean);
		}

		// All 0x2*** cases are the same
		// Calls subroutine at NNN.
		// type: Flow
		// template: 2NNN
		// *(0xNNN)()
		else if( (0x2000 <= opcode) && (opcode <= 0x2FFF) ){
			printf("Found 0x2NNN at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0x2000);
			call(opcodeClean);
		}

		// All 0x3*** cases are the same
		// 	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// template: 3XNN
		// if(Vx!=NN)
		else if( (0x3000 <= opcode) && (opcode <= 0x3FFF) ){
			printf("Found 0x3XNN at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0x3000);
		}

		// All 0x4*** cases are the same
		// Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// template: 4XNN
		// if(Vx==NN)
		else if( (0x4000 <= opcode) && (opcode <= 0x4FFF) ){
			printf("Found 0x4XNN at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0x4000);
		}

		// All 0x5*** cases are the same
		// Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// template: 5XY0
		// if(Vx!=Vy)
		else if( (0x5000 <= opcode) && (opcode <= 0x5FFF) ){
			printf("Found 0x5XY0 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0x5000);
		}

		// All 0x6*** cases are the same
		// Sets VX to NN
		// type: Constant
		// template: 6XNN
		// Vx = NN
		else if( (0x6000 <= opcode) && (opcode <= 0x6FFF) ){
			//VA = 0xFF;
			//printf("VA is: %x\n", VA);

			//printf("Found 0x6XNN at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0x6000);
			//printf("~~~~~~~~~~~~~~~~Clean op is: %x\n", opcodeClean);
			
			unsigned char lastByte = getLastByte(opcodeClean);
			//printf("~~~~~~~~~~~~~~~~Last byte is: %x\n", lastByte);
			
			unsigned short lowNibbleShort = opcodeClean - lastByte;
			//printf("%x\n", lowNibbleShort);
			lowNibbleShort = lowNibbleShort >> 8;
			unsigned char lowNibble = lowNibbleShort;
			//printf("~~~~~~~~~~~~~~~~Low nibble is: %x\n", lowNibble);
			
			unsigned char *dynamicRegister = getRegister(lowNibble);
			*dynamicRegister = lastByte;

			//printf("VA is: %x\n", VA);

			nextInstruction();
		}

		// All 0x7*** cases are the same
		// Adds NN to VX. (Carry flag is not changed)
		// type: Constant
		// template: 7XNN
		// Vx += NN
		else if( (0x7000 <= opcode) && (opcode <= 0x7FFF) ){
			//VA = 0xFF;
			//printf("VA is: %x\n", VA);

			//printf("Found 0x7XNN at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0x7000);
			//printf("~~~~~~~~~~~~~~~~Clean op is: %x\n", opcodeClean);
			
			unsigned char lastByte = getLastByte(opcodeClean);
			//printf("~~~~~~~~~~~~~~~~Last byte is: %x\n", lastByte);
			
			unsigned short lowNibbleShort = opcodeClean - lastByte;
			//printf("%x\n", lowNibbleShort);
			lowNibbleShort = lowNibbleShort >> 8;
			unsigned char lowNibble = lowNibbleShort;
			//printf("~~~~~~~~~~~~~~~~Low nibble is: %x\n", lowNibble);
			
			unsigned char *dynamicRegister = getRegister(lowNibble);
			*dynamicRegister = lastByte;

			//printf("VA is: %x\n", VA);

			nextInstruction();
		}

		// 8 ////////////////////////////////////////////////

		// Sets VX to the value of VY.
		// type: Assign
		// template: 8XY0
		// Vx=Vy
		/*case 0x8XY0:
			printf("Found 0x8XY0 at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Sets VX to VX or VY. (Bitwise OR operation)
		// type: BitOp
		// template: 8XY1
		// Vx=Vx|Vy
		/*case 0x8XY1:
			printf("Found 0x8XY1 at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Sets VX to VX and VY. (Bitwise AND operation)
		// type: BitOp
		// template: 8XY2
		// Vx=Vx&Vy
		/*case 0x8XY2:
			printf("Found 0x8XY2 at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Sets VX to VX xor VY.
		// type: BitOp
		// template: 8XY3
		// Vx=Vx^Vy
		/*case 0x8XY3:
			printf("Found 0x8XY3 at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
		// type: Math
		// template: 8XY4
		// Vx += Vy
		/*case 0x8XY4:
			printf("Found 0x8XY4 at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
		// type: Math
		// template: 8XY5
		// Vx -= Vy
		/*case 0x8XY5:
			printf("Found 0x8XY5 at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Stores the least significant bit of VX in VF and then shifts VX to the right by 1
		// type: BitOp
		// template: 8XY6
		// Vx>>=1
		/*case 0x8XY6:
			printf("Found 0x8XY6 at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
		// type: Math
		// template: 8XY7
		// Vx=Vy-Vx
		/*case 0x8XY7:
			printf("Found 0x8XY7 at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// Stores the most significant bit of VX in VF and then shifts VX to the left by 1
		// type: BitOp
		// template: 8XYE
		// Vx<<=1
		/*case 0x8XYE:
			printf("Found 0x8XYE at: %x\n", PC);
			unsigned short opcodeClean = removeBase(opcode, 0x8000);
			break;*/

		// All 0x9*** cases are the same
		// Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// template: 9XY0
		// if(Vx==Vy)
		else if( (0x9000 <= opcode) && (opcode <= 0x9FFF) ){
			printf("Found 0x9XY0 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0x9000);
			//if()
			//skipNextInstruction();
			//else
		}

		// All 0x0A*** cases are the same
		// Sets I to the address NNN.
		// type: Memory
		// template: ANNN
		// I = NNN
		else if( (0xA000 <= opcode) && (opcode <= 0xAFFF) ){
			//printf("Found 0xANNN at: %x with value of: %x\n", PC, opcode);
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
			printf("Found 0xBNNN at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xB000);
		}

		// All 0xC*** cases are the same
		// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		// type: Random
		// template: CXNN
		// Vx=rand()&NN
		else if( (0xC000 <= opcode) && (opcode <= 0xCFFF) ){
			printf("Found 0xCXNN at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xC000);
		}

		// All 0xD*** cases are the same
		// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
		// type: Display
		// template: DXYN
		// draw(Vx,Vy,N)
		else if( (0xD000 <= opcode) && (opcode <= 0xDFFF) ){
			printf("Found 0xDXYN at: %x with value of: %x\n", PC, opcode);
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
			printf("Found 0xEX9E at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xE000);
			break;*/

		// 	Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
		// type: KeyOp
		// template: EXA1
		// if(key()!=Vx)
		/*case 0xEXA1:
			printf("Found 0xEXA1 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xE000);
			break;*/

		// F ////////////////////////////////////////////////

		// Sets VX to the value of the delay timer.
		// type: Timer
		// template: FX07
		// Vx = get_delay()
		/*case 0xFX07:
			printf("Found 0xFX07 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
		// type: KeyOp
		// template: FX0A
		// Vx = get_key()
		/*case 0xFX0A:
			printf("Found 0xFX0A at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Sets the delay timer to VX.
		// type: Timer
		// template: FX15
		// delay_timer(Vx)
		/*case 0xFX15:
			printf("Found 0xFX15 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Sets the sound timer to VX.
		// type: Sound
		// template: FX18
		// sound_timer(Vx)
		/*case 0xFX18:
			printf("Found 0xFX18 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Adds VX to I
		// type: Memory
		// template: FX1E
		// I +=Vx
		/*case 0xFX1E:
			printf("Found 0xFX1E at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
		// type: Memory
		// template: FX29
		// I=sprite_addr[Vx]
		/*case 0xFX29:
			printf("Found 0xFX29 at: %x with value of: %x\n", PC, opcode);
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
			printf("Found 0xFX33 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
		// type: Memory
		// template: FX55
		// reg_dump(Vx,&I)
		/*case 0xFX55:
			printf("Found 0xFX55 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		// Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
		// type: Memory
		// template: FX65
		// reg_load(Vx,&I)
		/*case 0xFX65:
			printf("Found 0xFX65 at: %x with value of: %x\n", PC, opcode);
			unsigned short opcodeClean = removeBase(opcode, 0xF000);
			break;*/

		else {
			printf("\tInstruction not found at: %x with value of: %x\n", PC, opcode);
			PC += 0x2;
		}

		DEBUG_dumpState();

	}
}

void DEBUG_dumpState(){
	printf("Stack: \t%x\n", stack);
	printf("SP: \t%x\n", SP);
	printf("PC: \t%x\n", PC);
	printf("I: \t%x\n", I);
	printf("V0: \t%x\n", V0);
	printf("V1: \t%x\n", V1);
	printf("V2: \t%x\n", V2);
	printf("V3: \t%x\n", V3);
	printf("V4: \t%x\n", V4);
	printf("V5: \t%x\n", V5);
	printf("V6: \t%x\n", V6);
	printf("V7: \t%x\n", V7);
	printf("V8: \t%x\n", V8);
	printf("V9: \t%x\n", V9);
	printf("VA: \t%x\n", VA);
	printf("VB: \t%x\n", VB);
	printf("VC: \t%x\n", VC);
	printf("VD: \t%x\n", VD);
	printf("VE: \t%x\n", VE);
	printf("VF: \t%x\n", VF);
	
	printf("\nPress any character to continue...\n");
	int d = getchar();

	/*printf("\nPress i to step to the next instruction...\n");
	printf("\nPress m to print memory...\n");
	
	int c = getchar();
	printf("c = %i\n", c);
	if(c == 109){

	}
	else if(c == 231){

	}*/
}

unsigned short removeBase(unsigned short opcode, unsigned short base){
	return opcode - base;
}

void pushAddressToStack(){
	// make room for new value on stack
	SP += 0x2;

	printf("SP is: %x\n", SP);

	// store the address of the instruction after the call
	*SP = PC + 0x2;

	printf("*SP is: %x\n", *SP);
}

void popAddressFromStack(){
	// pull return address from the stack
	PC = *SP;

	// popped return address means
	// stack pointer needs to move to previous value
	SP += 0x2;
}

void call(unsigned short addr){
	pushAddressToStack();
	PC = addr;
}

void gotoAddr(unsigned short addr){
	PC = addr;
}

void nextInstruction(){
	PC += 0x2;
}

void skipNextInstruction(){
	PC += 0x4;
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

unsigned char getLastByte(unsigned short s){
	// overflow should leave only what we want
	unsigned char c = s;
	return c;
}