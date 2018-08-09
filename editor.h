#ifndef EDITOR_H
#define EDITOR_H

#include "SDL2/SDL.h"
#include <stdbool.h>

typedef struct TextEditor {
	char* buffer; // This only contains functionality for one line at the moment.
	unsigned int caret;
	bool uppercase;
} Editor;

Editor* EditorCreate();
void EditorUpdate(SDL_Renderer* renderer, Editor* editor, SDL_Event* event, const Uint8* state);
void RemoveWord(Editor* editor);

void RenderCaret(Editor* editor, SDL_Renderer* renderer, int widthOfChar, int heightOfChar);

#endif