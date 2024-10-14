#pragma once

void _text_initializeTtfLibrary();

void _text_changeFontSize(int fontSize);

void _text_changeColor(int colorR, int colorG, int colorB, int colorAlpha);

void _text_drawText(char* textToDraw, int destinationX, int destinationY, TTF_Font* font);
