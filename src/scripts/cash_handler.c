#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#include "constants.h"
#include "playerData.h"
#include "text_handler.h"

void _cash_brickIncome(int income) {
    if ((_data_cash + (income * pow(2, _data_upgradeBrickCostLevel - 1))) < 0 ||
    _data_cash + (income * pow(2, _data_upgradeBrickCostLevel - 1)) >= __INT_MAX__ ||
    _data_cash < 0) {

        _data_infinity += 1;
        _data_cash = _data_cash - __INT_MAX__ + (income * pow(2, _data_upgradeBrickCostLevel - 1));

    } else {
        _data_cash += income * pow(2, _data_upgradeBrickCostLevel - 1);
    }

}

void _cash_drawCash(int posX, int posY, TTF_Font* font) {
    char cashText[300];

    sprintf(cashText, "$%d", _data_cash);

    _text_drawText(cashText, posX, posY, font);
}

void _cash_drawInfinity(int posX, int posY, TTF_Font* font, int displayIfNone, int displayPercentIfNone) {
    if (_data_infinity == 0 && displayIfNone == 0 && displayPercentIfNone == 0) {
        return;
    }
    if (_data_infinity == 0 && displayPercentIfNone == 1) {
        char infinityText[300];

        float divResult = (float)_data_cash /(__INT_MAX__ - 1) * 100;

        sprintf(infinityText, "%f%%", divResult);
        _text_drawText(infinityText, posX, posY, font);
        return;
    }

    char infinityText[300];

    sprintf(infinityText, "Infinity: %d", _data_infinity);

    _text_drawText(infinityText, posX, posY, font);
}
