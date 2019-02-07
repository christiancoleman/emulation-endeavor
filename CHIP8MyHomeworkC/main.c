#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.c"
#include "cpu.c"
#include "gpu.c"
#include "timers.c"
#include "rom.c"
#include "SDL2/SDL.h"

int main(int arg, char * argv[]){

	// memory.c
	initMemory();

	// gou.c
	initGraphics();

	// rom.c
	loadRom();

	// cpu.c
	//startEmulation();

	// memory.c
	//DEBUG_printMemory();

	// http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
	/*
		setupGraphics
		setupInputs
		initialize cpu
		load the game
		for(;;){
			emulateACycle
			if(drawFlag){
				drawGraphics();
			}
			setKeys
		}
	*/
	/*
		Line 3-5: In this example we assume you will create a separate class to handle the opcodes.
		Line 10-11: Setup the graphics (window size, display mode, etc) and input system (bind callbacks)
		Line 14: Clear the memory, registers and screen
		Line 15: Copy the program into the memory
		Line 21: Emulate one cycle of the system
		Line 24: Because the system does not draw every cycle, we should set a draw flag when we need to update our screen. Only two opcodes should set this flag:
		0x00E0 – Clears the screen
		0xDXYN – Draws a sprite on the screen
		Line 28: If we press or release a key, we should store this state in the part that emulates the keypad
	*/

	return 0;
}