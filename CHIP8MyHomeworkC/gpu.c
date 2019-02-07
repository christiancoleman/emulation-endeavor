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

// The window renderer
SDL_Renderer* renderer = NULL;

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
			// Main loop flag
			bool quit = false;

			// Event handler
			SDL_Event e;

			// While app is running
			while( !quit ){
				//Handle events on queue
				while(SDL_PollEvent( &e ) != 0){
					//User requests quit
					if( e.type == SDL_QUIT ){
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );

				//Render red filled quad
				SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
				SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
				SDL_RenderFillRect( renderer, &fillRect );

				//Render green outlined quad
				SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
				SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );
				SDL_RenderDrawRect( renderer, &outlineRect );

				//Draw blue horizontal line
				SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0xFF );
				SDL_RenderDrawLine( renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

				//Draw vertical line of yellow dots
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF );
				for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
				{
					SDL_RenderDrawPoint( renderer, SCREEN_WIDTH / 2, i );
				}

				//Update screen
				SDL_RenderPresent( renderer );
			}
		}
	}

	// Free resources and close SDL
	closeSDL();
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
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) );{
			printf("Warning: Linear texture filtering not enabled!\n");
		}

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

bool loadMedia(){
	// Loading success flag
	bool success = true;

	// Nothing to load
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
	printf("\tClearing the screen...\n");
}

void draw(){
	printf("\t~~~ DRAWING ~~~\n");
}