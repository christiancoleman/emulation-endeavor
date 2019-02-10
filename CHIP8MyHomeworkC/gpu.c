#pragma once

#include "memory.c"
#include "SDL2/SDL.h"

#define SCALE 10
// 64x32 is the original screen size of the CHIP-8
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

unsigned char oldFrame[SCREEN_WIDTH][SCREEN_HEIGHT];
unsigned char newFrame[SCREEN_WIDTH][SCREEN_HEIGHT];

bool shouldDraw = false;

/*

****		11110000	0xF0
*  *		10010000	0x90
*  *		10010000	0x90
*  *		10010000	0x90
****		11110000	0xF0

****		11110000	0xF0
   *		00010000	0x10
****		11110000	0xF0
*   		10000000	0x80
****		11110000	0xF0

*/
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

// The window renderer
SDL_Renderer* renderer = NULL;

//////////////////////////////////////
// declarations
//////////////////////////////////////

void initGraphics();
void loadFontSet();
bool initSDL();
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

	initSDL();
}

void loadFontSet(){
	int i = 0;
	for(; i < 80; ++i) {
		memory[i] = chip8_fontset[i];
	}
	printf("Loaded font set...\n");
}

bool initSDL(){
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
			(SCREEN_WIDTH * SCALE),
			(SCREEN_HEIGHT * SCALE),
			SDL_WINDOW_SHOWN
		);

		if( window == NULL ){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			// Create renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
			if(renderer == NULL){
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				// NOTHING
			}
		}
	}

	return success;
}

void closeSDL(){
	// Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

void clearScreen(){
	for(int i = 0; i < SCREEN_WIDTH; i++){
		for(int j = 0; j < SCREEN_HEIGHT; j++){
			oldFrame[i][j] = 0x0;
			newFrame[i][j] = 0x0;
		}
	}
}

void draw(){
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.h = SCREEN_HEIGHT * SCALE;
	rect.w = SCREEN_WIDTH * SCALE;

	for(int i = 0; i < SCREEN_WIDTH; i++){
		for(int j = 0; j < SCREEN_HEIGHT; j++){
			if(oldFrame[i][j] ^ newFrame[i][j] == 1){
				rect.x = i * SCALE;
				rect.y = j * SCALE;
				rect.w = SCALE;
				rect.h = SCALE;
				if(newFrame[i][j] == 1){
					SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xF0, 0xF0 );
					SDL_RenderFillRect( renderer, &rect );
				}
				else if(newFrame[i][j] == 0){
					SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
					SDL_RenderFillRect( renderer, &rect );
				}
			}
		}
	}

	//Update screen
	SDL_RenderPresent( renderer );

	shouldDraw = false;
}