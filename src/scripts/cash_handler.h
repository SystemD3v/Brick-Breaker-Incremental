#pragma once

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"
#include "playerData.h"

void _cash_income(int income);

void _cash_drawCash(int posX, int posY, TTF_Font* font);
