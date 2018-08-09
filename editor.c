#include "editor.h"

#include <stdlib.h> // For malloc(), calloc() and realloc()
#include <stdio.h> // For printf()
#include <ctype.h> // For toupper()

#include "text.h"

#define MAX_CHARS 300
#define CARET_INTERVAL 30

extern int currentLine;
extern Text** lines;
extern Editor** editors;
	
extern TTF_Font* cour;
extern SDL_Color White;

extern int pixelsPerChar;
extern double caretTime;

Editor* EditorCreate() {
	Editor* editor = (Editor*) malloc(sizeof(Editor));
	editor->buffer = (char  *) calloc(MAX_CHARS, sizeof(char));
	editor->caret  = 0;
	editor->uppercase = false;
	
	return editor;
}

void EditorUpdate(SDL_Renderer* renderer, Editor* editor, SDL_Event* event, const Uint8* state) {
	SDL_Keycode keycode = event->key.keysym.sym;
	switch (event->type) {
		case SDL_KEYDOWN: {
			char key = (char) keycode;
			
			if (keycode == SDLK_CAPSLOCK) {
				editor->uppercase = !editor->uppercase;
			} else if (key == 8) { // The backspace ASCII code.
				if (editor->caret <= 0) {
					if (currentLine > 1) {
						currentLine--;
					}
				} else if (!state[SDL_SCANCODE_LCTRL]) {
					if (editor->caret > 0) {
						editor->buffer[--editor->caret] = 0;
					}
				} else {
					RemoveWord(editor);
				}
				// printf("Buffer: %s\n", editor->buffer);
			} else if (key >= 32 && key <= 126 && key != SDLK_CAPSLOCK) { // These are the printable ASCII characters (https://theasciicode.com.ar/ascii-printable-characters/space-ascii-code-32.html)
				if (state[SDL_SCANCODE_LSHIFT]) {
					editor->uppercase = true;
				}
				editor->buffer[editor->caret++] = editor->uppercase ? toupper(key) : key;
				// printf("Buffer: %s\n", editor->buffer);
				if (state[SDL_SCANCODE_LSHIFT]) {
					editor->uppercase = false;
				}
			} else if (key == SDLK_RETURN) {
				lines[currentLine++] = CreateText(renderer, cour, "", White, pixelsPerChar, currentLine - 1);
				printf("Inserted a new line");
				editors[currentLine - 1] = EditorCreate();
			}
			
			break;
		}
	}
	
}

void RenderCaret(Editor* editor, SDL_Renderer* renderer, int widthOfChar, int heightOfChar) {
	if (caretTime > CARET_INTERVAL && caretTime < CARET_INTERVAL * 2) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, editor->caret * widthOfChar + 5, 3 + ((currentLine - 1) * heightOfChar), editor->caret * widthOfChar + 5, heightOfChar - 3 + ((currentLine - 1) * heightOfChar));
	} else if (caretTime > CARET_INTERVAL * 2) {
		caretTime = 0;
	}
}

void RemoveWord(Editor* editor) {
	bool reachedEnd = false;
	while (!reachedEnd) {
		if (editor->caret <= 0) {
			reachedEnd = true;
		} else if (editor->buffer[editor->caret - 1] != ' ') {
			editor->buffer[--editor->caret] = 0;
		} else {
			editor->buffer[--editor->caret] = 0;
			reachedEnd = true;
		}
	}
}



