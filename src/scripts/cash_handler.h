#pragma once

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"
#include "playerData.h"

void _cash_brickIncome(int income);

void _cash_drawCash(int posX, int posY, TTF_Font* font);

void _cash_drawInfinity(int posX, int posY, TTF_Font* font, int displayIfNone, int displayPercentIfNone);
