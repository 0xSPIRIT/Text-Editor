#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "editor.h"
#include "text.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int currentLine = 0;
Text** lines;
Editor** editors;
	
TTF_Font* cour;
SDL_Color White;

int pixelsPerChar = 20;
double caretTime = 0;

int main(int argc, char** argv) {
	bool running;
	
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("Unable to initialize SDL2.\n");
		return EXIT_FAILURE;
	}
	
	SDL_Window* window = SDL_CreateWindow("Typing Test",
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  WINDOW_WIDTH,
										  WINDOW_HEIGHT,
										  SDL_WINDOW_SHOWN);
										  
	if (!window) {
		printf("The window was unable to be initialized\n");
		return EXIT_FAILURE;
	}
	
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	if (!renderer) {
		printf("The renderer was unable to be initialized\n");
		return EXIT_FAILURE;
	}
	
	if (TTF_Init()) {
		printf("SDL_TTF was unable to be initialized\n");
		return EXIT_FAILURE;
	}
	
	running = true;
	
	SDL_Event event;

	editors = (Editor**) malloc(sizeof(Editor*) * 50);
	editors[0] = EditorCreate(0);
	White.r = 255;
	White.g = 255;
	White.b = 255;
	
	cour  = TTF_OpenFont("../res/kongtext.ttf", 12);

	lines = (Text**) malloc(sizeof(Text*) * 50);
	lines[currentLine++] = CreateText(renderer, cour, editors[0]->buffer, White, pixelsPerChar, currentLine - 1);
	
	// SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(cour, editor->buffer, White, WINDOW_WIDTH); 

	// SDL_Texture* msg = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	
	// SDL_Rect Message_rect; //create a rect
	// Message_rect.x = 0;  //controls the rect's x coordinate 
	// Message_rect.y = 0; // controls the rect's y coordinte
	// Message_rect.w = strlen(editor->buffer) * pixelsPerChar; // controls the width of the rect
	// Message_rect.h = pixelsPerChar * 1.8; // controls the height of the rect
	
	const Uint8* state;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;	
	
	while (running) {
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST)*1000 / SDL_GetPerformanceFrequency() );
   
		caretTime += deltaTime;
		
		state = SDL_GetKeyboardState(NULL);
		
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) running = false;
			
			EditorUpdate(renderer, editors[currentLine - 1], &event, state);
			SetText(renderer, lines[currentLine - 1], editors[currentLine - 1]->buffer);

			// surfaceMessage = TTF_RenderText_Blended_Wrapped(cour, editor->buffer, White, WINDOW_WIDTH); 

			// msg = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
			// Message_rect.w = strlen(editor->buffer) * pixelsPerChar; // controls the width of the rect
		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);
		
		// SDL_RenderCopy(renderer, msg, NULL, &Message_rect);
		for (int i = 0; i < currentLine; i++) {
			RenderText(renderer, lines[i]);
		}
		
		RenderCaret(editors[currentLine - 1], renderer, pixelsPerChar, lines[currentLine - 1]->msgRect.h);

        SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	free(cour);
	for (int i = 0; i < sizeof(editors) / sizeof(Editor); i++) {
		free(editors[i]);
	}
	for (int i = 0; i < sizeof(lines) / sizeof(Text); i++) {
		SDL_DestroyTexture(lines[i]->msg);
		SDL_FreeSurface(lines[i]->surfaceMessage);
		free(lines[i]);
	}

	TTF_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}