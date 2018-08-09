#ifndef TEXT_H
#define TEXT_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

typedef struct TextRender {
	TTF_Font* font;
	char* txt;
	SDL_Color color;
	SDL_Texture* msg;
	SDL_Surface* surfaceMessage;
	SDL_Rect msgRect;
	
	int pixels;
	int index;
} Text;

Text* CreateText(SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color col, int widthPerChar, int index);
void SetText(SDL_Renderer* renderer, Text* text, char* newText);

void RenderText(SDL_Renderer* renderer, Text* text);

#endif