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

		// Special zero case #1
		// Clears the screen.
		// type: Display
		// disp_clear();
		if(opcode == 0x00E0) {
			printf("Found 0x00E0 at: %x\n", PC);
			clearScreen();
		} 
		
		// Special zero case #2
		// return from subroutine
		// type: Flow
		// return;
		else if(opcode == 0x00EE) {
			printf("Found 0x00EE at: %x\n", PC);
		} 
		
		// Generic zero case catch
		// Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
		// type: Call
		// call NNN;
		else if( (0x0000 <= opcode) && (opcode <= 0x0FFF) ){
			printf("Found 0x0NNN at: %x with value of: %x\n", PC, opcode);
		} 
		
		// All 0x1*** cases are the same
		// Jumps to address NNN.
		// type: Flow
		// goto NNN;
		else if( (0x1000 <= opcode) && (opcode <= 0x1FFF) ){
			printf("Found 0x1NNN at: %x with value of: %x\n", PC, opcode);
		} 
		
		// All 0x2*** cases are the same
		// Calls subroutine at NNN.
		// type: Flow
		// *(0xNNN)()
		else if( (0x2000 <= opcode) && (opcode <= 0x2FFF) ){
			printf("Found 0x2NNN at: %x with value of: %x\n", PC, opcode);
		}
		
		// All 0x3*** cases are the same
		// 	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// if(Vx!=NN)
		else if( (0x3000 <= opcode) && (opcode <= 0x3FFF) ){
			printf("Found 0x3XNN at: %x with value of: %x\n", PC, opcode);
		}

		// All 0x4*** cases are the same
		// Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// if(Vx==NN)
		else if( (0x4000 <= opcode) && (opcode <= 0x4FFF) ){
			printf("Found 0x4XNN at: %x with value of: %x\n", PC, opcode);	
		} 

		// All 0x5*** cases are the same
		// Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// if(Vx!=Vy)
		else if( (0x5000 <= opcode) && (opcode <= 0x5FFF) ){
			printf("Found 0x5XY0 at: %x with value of: %x\n", PC, opcode);
		} 

		// All 0x6*** cases are the same
		// Sets VX to NN.
		// type: Constant
		// Vx = NN	
		else if( (0x6000 <= opcode) && (opcode <= 0x6FFF) ){
			printf("Found 0x6XNN at: %x with value of: %x\n", PC, opcode);
		} 

		// All 0x7*** cases are the same
		// Adds NN to VX. (Carry flag is not changed)
		// type: Constant
		// Vx += NN
		else if( (0x7000 <= opcode) && (opcode <= 0x7FFF) ){
			printf("Found 0x7XNN at: %x with value of: %x\n", PC, opcode);
		}

		// 8 ////////////////////////////////////////////////

		// Sets VX to the value of VY.
		// type: Assign
		// Vx=Vy
		/*case 0x8XY0:
			printf("Found 0x8XY0 at: %x\n", PC);
			break;*/

		// Sets VX to VX or VY. (Bitwise OR operation)
		// type: BitOp
		// Vx=Vx|Vy
		/*case 0x8XY1:
			printf("Found 0x8XY1 at: %x\n", PC);
			break;*/

		// Sets VX to VX and VY. (Bitwise AND operation)
		// type: BitOp
		// Vx=Vx&Vy
		/*case 0x8XY2:
			printf("Found 0x8XY2 at: %x\n", PC);
			break;*/

		// Sets VX to VX xor VY.
		// type: BitOp
		// Vx=Vx^Vy
		/*case 0x8XY3:
			printf("Found 0x8XY3 at: %x\n", PC);
			break;*/

		// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
		// type: Math
		// Vx += Vy
		/*case 0x8XY4:
			printf("Found 0x8XY4 at: %x\n", PC);
			break;*/

		// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
		// type: Math
		// Vx -= Vy
		/*case 0x8XY5:
			printf("Found 0x8XY5 at: %x\n", PC);
			break;*/

		// Stores the least significant bit of VX in VF and then shifts VX to the right by 1
		// type: BitOp
		// Vx>>=1
		/*case 0x8XY6:
			printf("Found 0x8XY6 at: %x\n", PC);
			break;*/

		// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
		// type: Math
		// Vx=Vy-Vx
		/*case 0x8XY7:
			printf("Found 0x8XY7 at: %x\n", PC);
			break;*/

		// Stores the most significant bit of VX in VF and then shifts VX to the left by 1
		// type: BitOp
		// Vx<<=1
		/*case 0x8XYE:
			printf("Found 0x8XYE at: %x\n", PC);
			break;*/

		// All 0x9*** cases are the same
		// Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
		// type: Condition
		// if(Vx==Vy)
		else if( (0x9000 <= opcode) && (opcode <= 0x9FFF) ){
			printf("Found 0x9XY0 at: %x with value of: %x\n", PC, opcode);
		}

		// All 0x0A*** cases are the same
		// Sets I to the address NNN.
		// type: Memory
		// I = NNN
		else if( (0xA000 <= opcode) && (opcode <= 0xAFFF) ){
			printf("Found 0xANNN at: %x with value of: %x\n", PC, opcode);
		}

		// All 0xB*** cases are the same
		// Jumps to the address NNN plus V0.
		// type: Flow
		// PC=V0+NNN
		else if( (0xB000 <= opcode) && (opcode <= 0xBFFF) ){
			printf("Found 0xBNNN at: %x with value of: %x\n", PC, opcode);
		}

		// All 0xC*** cases are the same
		// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		// type: Random
		// Vx=rand()&NN
		else if( (0xC000 <= opcode) && (opcode <= 0xCFFF) ){
			printf("Found 0xCXNN at: %x with value of: %x\n", PC, opcode);
		}

		// All 0xD*** cases are the same
		// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
		// type: Display
		// draw(Vx,Vy,N)
		else if( (0xD000 <= opcode) && (opcode <= 0xDFFF) ){
			printf("Found 0xDXYN at: %x with value of: %x\n", PC, opcode);
		}

		// E ////////////////////////////////////////////////

		// Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
		// type: KeyOp
		// if(key()==Vx)
		/*case 0xEX9E:
			printf("Found 0xEX9E at: %x with value of: %x\n", PC, opcode);
			break;*/

		// 	Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
		// type: KeyOp
		// if(key()!=Vx)
		/*case 0xEXA1:
			printf("Found 0xEXA1 at: %x with value of: %x\n", PC, opcode);
			break;*/

		// F ////////////////////////////////////////////////

		// Sets VX to the value of the delay timer.
		// type: Timer
		// Vx = get_delay()
		/*case 0xFX07:
			printf("Found 0xFX07 at: %x with value of: %x\n", PC, opcode);
			break;*/

		// A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
		// type: KeyOp
		// Vx = get_key()
		/*case 0xFX0A:
			printf("Found 0xFX0A at: %x with value of: %x\n", PC, opcode);
			break;*/

		// Sets the delay timer to VX.
		// type: Timer
		// delay_timer(Vx)
		/*case 0xFX15:
			printf("Found 0xFX15 at: %x with value of: %x\n", PC, opcode);
			break;*/

		// Sets the sound timer to VX.
		// type: Sound
		// sound_timer(Vx)
		/*case 0xFX18:
			printf("Found 0xFX18 at: %x with value of: %x\n", PC, opcode);
			break;*/

		// Adds VX to I
		// type: Memory
		// I +=Vx
		/*case 0xFX1E:
			printf("Found 0xFX1E at: %x with value of: %x\n", PC, opcode);
			break;*/

		// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
		// type: Memory
		// I=sprite_addr[Vx]
		/*case 0xFX29:
			printf("Found 0xFX29 at: %x with value of: %x\n", PC, opcode);
			break;*/

		// Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
		// type: BCD
		// set_BCD(Vx);
		// *(I+0)=BCD(3);
		// *(I+1)=BCD(2);
		// *(I+2)=BCD(1);
		/*case 0xFX33:
			printf("Found 0xFX33 at: %x with value of: %x\n", PC, opcode);
			break;*/

		// Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
		// type: Memory
		// reg_dump(Vx,&I)
		/*case 0xFX55:
			printf("Found 0xFX55 at: %x with value of: %x\n", PC, opcode);
			break;*/

		// Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
		// type: Memory
		// reg_load(Vx,&I)
		/*case 0xFX65:
			printf("Found 0xFX65 at: %x with value of: %x\n", PC, opcode);
			break;*/

		else {
			printf("\tInstruction not found at: %x with value of: %x\n", PC, opcode);
		}

		PC += 0x2; // opcodes are 2 bytes long, so we read them in that increment
	}

}