#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.c"
#include "cpu.c"
#include "gpu.c"
#include "rom.c"
#include "tests.c"
#include "SDL2/SDL.h"

void start();
bool runTests();

int main(int argc, char * argv[]){

	printf("%s\n", argv[0]);
	printf("%s\n", argv[1]);
	if(argc > 1 && strcmp(argv[1], "-runtests") == 0){
		runTests();
		return 0;
	}
	printf("made it here at least");

	// memory.c
	initMemory();

	// gpu.c
	initGraphics();

	// rom.c
	loadRom();

	start();

	return 0;
}

void start(){
	bool quit = false;

	// Event handler
	SDL_Event e;

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

bool runTests(){
	if(testOpcodes()){
		printf("Tests have passed!\n\n");
	} else {
		printf("Tests have failed!\n\n");
	}
}