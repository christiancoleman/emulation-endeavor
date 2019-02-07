#pragma once

#include "memory.c"
#include "SDL2/SDL.h"

#define SCALE 10
#define SCREEN_WIDTH 64*SCALE
#define SCREEN_HEIGHT 32*SCALE

unsigned char gfx[SCREEN_WIDTH * SCREEN_HEIGHT]; // 64*32 = 2,048

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
void draw(int, int, int);
void drawD();

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
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) );{
			printf("Warning: Linear texture filtering not enabled!\n");
		}

		// Create window
		window = SDL_CreateWindow("CHIP8MyHomework",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
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
				// Initialize rendered color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
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
	// Clear screen
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( renderer );

	// Update screen
	SDL_RenderPresent( renderer );
}

//void draw(int x1, int y1, int x2, int y2){
	//Clear screen
	//SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	//SDL_RenderClear( renderer );

	//Render red filled quad
	/*SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( renderer, &fillRect );*/

	//Render green outlined quad
	//SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
	//SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );
	//SDL_RenderDrawRect( renderer, &outlineRect );

	//Draw vertical line of yellow dots
	/*SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF );
	for( int i = 0; i < SCREEN_HEIGHT; i += 4 )	{
		SDL_RenderDrawPoint( renderer, SCREEN_WIDTH / 2, i );
	}
	*/

	//Update screen
	//SDL_RenderPresent( renderer );
//}

void draw(int x, int y, int height){
	//Clear screen
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( renderer );

	int x1 = x;
	int x2 = x + 8;
	int y1 = y;
	int y2 = y + 8;

	// draw sprite
	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xF0, 0xF0 );
	for(int i = 0; i < height; i++){
		SDL_RenderDrawLine(
			renderer,
			( x1+i ),
			( y1+i ),
			( x2+i ),
			( y2+i )
		);
	}

	//Update screen
	SDL_RenderPresent( renderer );
}