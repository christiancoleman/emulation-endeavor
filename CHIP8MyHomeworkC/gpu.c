#pragma once

#include "memory.c"
#include "SDL2/SDL.h"

// graphics in this system are either black or white
// we can symbolize this via a 1 or 2

//#define SCREEN_WIDTH 256
//#define SCREEN_HEIGHT 128
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define IMAGE_TO_LOAD "hello_world.bmp"

unsigned char gfx[SCREEN_WIDTH * SCREEN_HEIGHT]; // 64*32 = 2,048

unsigned char chip8_fontset[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// The window we'll be rendering to
SDL_Window* window = NULL;

// The surface contained by the window
SDL_Surface* screenSurface = NULL;

SDL_Surface* helloWorld = NULL;

//////////////////////////////////////
// declarations
//////////////////////////////////////

void initGraphics();
void loadFontSet();
void startSDL();
bool init();
bool loadMedia();
void closeSDL();
void clearScreen();
void draw();

//////////////////////////////////////
//implementations
//////////////////////////////////////

void initGraphics(){
	printf("\n");
	printf("######################################################\n");
	printf("############### INITIALIZING GRAPHICS ################\n");
	printf("######################################################\n\n");

	loadFontSet();

	startSDL();
}

void loadFontSet(){
	int i = 0;
	for(; i < 80; ++i) {
		memory[i] = chip8_fontset[i];
	}
	printf("Loaded font set...\n");
}

void startSDL(){
	if(!init()){
		printf("Failed to initialize SDL\n");
	}
	else {
		if(!loadMedia()){
			printf("Failed to load the media!\n");
		}
		else {
			// Apply the image
			SDL_BlitSurface( helloWorld, NULL, screenSurface, NULL );

			// Update the surface
			SDL_UpdateWindowSurface( window );

			// Wait two seconds
			SDL_Delay(2000);
		}
	}
}

bool init(){
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		// Create window
		window = SDL_CreateWindow("CHIP8MyHomework",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
		);
		if( window == NULL ){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			// Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			/*// Fill the surface with color (white is 0xFF, 0xFF, 0xFF)
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

			// Update the surface
			// "An important thing to know about rendering is that just because you've drawn something to the screen surface doesn't mean you'll see it. After you've done all your drawing you need to update the window so it shows everything you drew. A call to SDL_UpdateWindowSurface will do this."
			SDL_UpdateWindowSurface(window);

			// Wait two seconds
			SDL_Delay( 2000 );*/
		}
	}

	return success;
}

bool loadMedia(){
	// Loading success flag
	bool success = true;

	// Load splash image
	helloWorld = SDL_LoadBMP( IMAGE_TO_LOAD );
	if(helloWorld == NULL){
		printf("Unable to load image %s! SDL_Error: %s\n", IMAGE_TO_LOAD, SDL_GetError());
		success = false;
	}

	return success;
}

void closeSDL(){
	// Deallocate surface
	SDL_FreeSurface(helloWorld);
	helloWorld = NULL;

	// Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

void clearScreen(){
	printf("\tClearing the screen...\n");
}

void draw(){
	printf("\t~~~ DRAWING ~~~\n");
}