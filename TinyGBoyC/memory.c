/*
$FFFF Interrupt Enable Flag

$FF80-$FFFE Zero Page - 127 bytes

$FF00-$FF7F Hardware I/O Registers

$FEA0-$FEFF Unusable Memory

$FE00-$FE9F OAM - Object Attribute Memory

$E000-$FDFF Echo RAM - Reserved, Do Not Use

$D000-$DFFF Internal RAM - Bank 1-7 (switchable - CGB only)

$C000-$CFFF Internal RAM - Bank 0 (fixed)

$A000-$BFFF Cartridge RAM (If Available)

$9C00-$9FFF BG Map Data 2

$9800-$9BFF BG Map Data 1

$8000-$97FF Character RAM

$4000-$7FFF Cartridge ROM - Switchable Banks 1-xx

$0150-$3FFF Cartridge ROM - Bank 0 (fixed)

$0100-$014F Cartridge Header Area

$0000-$00FF Restart and Interrupt Vectors
*/

#pragma once

// Should be size of 0xFFFF or 65,535+1(0x0000 included) bytes
// 11 total offsets but 12 sections
struct memory {
	char interruptEnableFlag; // $FFFF
	char zeroPage[0x7F]; // $FF80 - $FFFE
	char hwIORegions[0x80]; // $FF00-$FF7F
	char unusableMemory[0x60]; // $FEA0-$FEFF
	char objectAttributeMemory[0xA0]; // $FE00-$FE9F
	char echoRAM[0x1E00]; // $E000-$FDFF
	char internalRAM_1_7[0x1000]; // $D000-$DFFF
	char internalRAM_0[0x1000]; // $C000-$CFFF
	char cartridgeRAM[0x2000]; // $A000-$BFFF
	char bgMapData2[0x400]; // $9C00-$9FFF
	char bgMapData1[0x400]; // $9800-$9BFF
	char characterRAM[0x1800]; // $8000-$97FF
	char cartridgeROM_switchableBank_1_xx[0x4000]; // $4000-$7FFF
	char cartridgeROM_bank_0[0x3EB0]; // $0150-$3FFF
	char cartridgeHeaderArea[0x50]; // $0100-$014F
	char restartAndInterruptVectors[0x100]; // $0000 - $00FF
};

static char ram[0xFFFF];

static char readByte(int address) {
	return ram[address];
}

static char writeByte(int address, char value){
	ram[address] = value;
}