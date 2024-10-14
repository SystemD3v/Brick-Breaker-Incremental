#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"
#include "playerData.h"
#include "text_handler.h"

void _cash_income(int income) {
    _data_cash += income;
}

void _cash_drawCash(int posX, int posY, TTF_Font* font) {
    char cashText[300];

    sprintf(cashText, "$%d", _data_cash);

    _text_drawText(cashText, posX, posY, font);
}
