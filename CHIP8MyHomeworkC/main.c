#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.c"
#include "cpu.c"
#include "gpu.c"
#include "timers.c"
#include "rom.c"
#include "SDL2/SDL.h"

void start();

int main(int arg, char * argv[]){

	// memory.c
	initMemory();

	// gou.c
	initGraphics();

	// rom.c
	loadRom();

	// below
	start();

	return 0;
}

void start(){
	bool quit = false;

	// Event handler
	SDL_Event e;

	printf("\n");
	printf("######################################################\n");
	printf("################ STARTING EMULATION ##################\n");
	printf("######################################################\n");
	printf("\n");

	// While app is running
	while( !quit ){
		// Handle events on queue
		while(SDL_PollEvent( &e ) != 0){
			// User requests quit via X button
			if( e.type == SDL_QUIT ){
				quit = true;
			}
		}
		doCycle();
	}

	// Free resources and close SDL
	closeSDL();
}