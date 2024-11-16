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
extern int _data_currentPattern;

extern int UPGRADE_BACKGROUND_MAIN;
extern int _data_upgradeBackgroundMain[4];
extern int _data_backgroundMainSelected;
extern int UPGRADE_BACKGROUND_SHOP;
extern int _data_upgradeBackgroundShop[4];
extern int _data_backgroundShopSelected;
extern int UPGRADE_BACKGROUND_INF;
extern int _data_upgradeBackgroundInf[4];
extern int _data_backgroundInfSelected;

extern int UPGRADE_COUNT;
extern int _data_upgradeBought[10];

extern char* _data_saveFilePath;

void _data_updateConstantsWithData();

int _data_loadGame();
void _data_saveGame();
