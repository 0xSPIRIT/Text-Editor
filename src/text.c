#include "text.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern int currentLine;

Text* CreateText(SDL_Renderer* renderer, TTF_Font* font, char* txt, SDL_Color col, int widthPerChar, int index) {
	Text* text = (Text*) malloc(sizeof(Text));
	text->font = font;
	text->color= col;

	text->txt  = (char*) malloc(strlen(txt) * sizeof(char));
	strcpy(text->txt, txt);
	
	text->pixels = widthPerChar;
	
	text->surfaceMessage = TTF_RenderText_Solid(text->font, text->txt, text->color); 

	text->msg = SDL_CreateTextureFromSurface(renderer, text->surfaceMessage);
	text->index = index;
}

void SetText(SDL_Renderer* renderer, Text* text, char* newText) {
	text->txt = newText;
	text->surfaceMessage = TTF_RenderText_Solid(text->font, text->txt, text->color); 

	text->msg = SDL_CreateTextureFromSurface(renderer, text->surfaceMessage);
}

void RenderText(SDL_Renderer* renderer, Text* text) {
	text->msgRect.h = text->pixels * 1.8; // controls the height of the rect
	text->msgRect.x = 0;  //controls the rect's x coordinate 
	text->msgRect.y = text->index * text->msgRect.h; // controls the rect's y coordinte
	text->msgRect.w = strlen(text->txt) * text->pixels; // controls the width of the rect
	
	SDL_RenderCopy(renderer, text->msg, NULL, &text->msgRect);
}