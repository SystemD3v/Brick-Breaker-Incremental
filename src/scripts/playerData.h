#pragma once

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


extern int _data_cash;
extern int _data_infinity;

extern int _data_upgradeBrickCostLevel;
extern int _data_upgradeWidthLevel;
extern int _data_upgradeHeightLevel;
extern int _data_gameSpeed;
extern int _data_lossPreventionLevel;
extern int _data_rToReset;
extern int _data_tToPattern;

extern int UPGRADE_COUNT;
extern int _data_upgradeBought[10];

extern char* _data_saveFilePath;

void _data_updateConstantsWithData();

int _data_loadGame();
void _data_saveGame();
