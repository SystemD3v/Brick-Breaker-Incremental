#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <string.h>

#include "constants.h"
#include "playerData.h"
#include "text_handler.h"
#include "cash_handler.h"
#include "function.h"
#include "audio_functions.h"
#include "brick_patterns.h"
#include "upgrade_menu.h"


void _infinityMenu_mouseHandler() {
    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);


}

void drawBackgroundInf() {
    switch (_data_backgroundInfSelected) {
        case 4:
            sprite(0 ,0, "../assets/imgs/backgrounds/infinity/tier4bg.bmp");
            break;
        case 3:
            sprite(0 ,0, "../assets/imgs/backgrounds/infinity/tier3bg.bmp");
            break;
        case 2:
            sprite(0 ,0, "../assets/imgs/backgrounds/infinity/tier2bg.bmp");
            break;
        case 1:
            sprite(0 ,0, "../assets/imgs/backgrounds/infinity/tier1bg.bmp");
            break;
        default:
            break;
    }
}

int _infinityMenu_checkDependencies(char* upgradeName) {

    char* upgradeNames[10] = {"1", "2-1", "2-2", "3"};
    char* upgradeDependencies[10][10] = {{}, {"1"}, {"1"}, {"2-1", "2-2"}};

    int index = -1;
    for (int i = 0; i < 10; i++) {
        if (upgradeNames[i] != NULL && strcmp(upgradeNames[i], upgradeName) == 0) {
            index = i;
            break;
        }
    }

    // Upgrade doesn't exist, check
    if (index == -1) {
        return 0;
    }

    for (int j = 0; upgradeDependencies[index][j] != NULL; j++) {
        char* dependency = upgradeDependencies[index][j];

        // Find dependency index
        int depIndex = -1;
        for (int k = 0; k < 10; k++) {
            if (strcmp(upgradeNames[k], dependency) == 0) {
                depIndex = k;
                break;
            }
        }

        // If not bought, return 0
        if (depIndex == -1 || _data_upgradeBought[depIndex] == 0) {
            return 0;
        }
    }

    // All dependencies are satisfied :)))))
    return 1;
}


void _infinityMenu_displayMenu() {

    clear();

    drawBackgroundInf();

    _text_changeColor(255, 0, 255, 255);
    _text_drawText("Welcome to the infinity tree! Press I to exit.", 0, 0, gameFont_52);
    _text_drawText("*IP = \"infinity (points)\"", 0, 54, gameFont_18);

    // Upgrade 1
    if (_infinityMenu_checkDependencies("1")) {;
        changeColor(55, 0, 55);
        drawRect(WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.1, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1);
        changeColor(185, 0, 185);
        drawRect(WINDOW_WIDTH * 0.46, WINDOW_HEIGHT * 0.11, WINDOW_WIDTH * 0.08, WINDOW_HEIGHT * 0.08);
        _text_changeColor(255, 0, 255, 255);
        _text_drawText("1", WINDOW_WIDTH * 0.47, WINDOW_HEIGHT * 0.12, gameFont_52);
    }

    // Upgrade 2-1
    if (_infinityMenu_checkDependencies("2-1")) {;
        changeColor(55, 0, 55);
        drawRect(WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.25, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1);
        changeColor(185, 0, 185);
        drawRect(WINDOW_WIDTH * 0.36, WINDOW_HEIGHT * 0.26, WINDOW_WIDTH * 0.08, WINDOW_HEIGHT * 0.08);
        _text_changeColor(255, 0, 255, 255);
        _text_drawText("2-1", WINDOW_WIDTH * 0.37, WINDOW_HEIGHT * 0.27, gameFont_52);
    }

    // Upgrade 2-2
    if (_infinityMenu_checkDependencies("2-2")) {;
        changeColor(55, 0, 55);
        drawRect(WINDOW_WIDTH * 0.55, WINDOW_HEIGHT * 0.25, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1);
        changeColor(185, 0, 185);
        drawRect(WINDOW_WIDTH * 0.56, WINDOW_HEIGHT * 0.26, WINDOW_WIDTH * 0.08, WINDOW_HEIGHT * 0.08);
        _text_changeColor(255, 0, 255, 255);
        _text_drawText("2-2", WINDOW_WIDTH * 0.57, WINDOW_HEIGHT * 0.27, gameFont_52);
    }

    // Upgrade 3
    if (_infinityMenu_checkDependencies("3")) {;
        changeColor(55, 0, 55);
        drawRect(WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.40, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1);
        changeColor(185, 0, 185);
        drawRect(WINDOW_WIDTH * 0.46, WINDOW_HEIGHT * 0.41, WINDOW_WIDTH * 0.08, WINDOW_HEIGHT * 0.08);
        _text_changeColor(255, 0, 255, 255);
        _text_drawText("3", WINDOW_WIDTH * 0.47, WINDOW_HEIGHT * 0.42, gameFont_52);
    }


    /*
    // Brick cost upgrade. TLC:(WW*0.1; WH*0.1); SIZE:(WW*0.8; WH*0.15); BORDER:
    changeColor(55, 0, 55);
    drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1, WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.15);
    changeColor(185, 0, 185);
    drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.11, WINDOW_WIDTH * 0.78, WINDOW_HEIGHT * 0.13);
    _text_changeColor(255, 0, 255, 255);
    char BVcost[300];
    // Function to compute the cost of the upgrade: (x-1)^2 + 10x^2 + 1
    // (pow((_data_upgradeBrickCostLevel - 1), 2) + pow((10 * _data_upgradeBrickCostLevel), 2)))
    sprintf(BVcost, "Even more money! Increase x2 max upgrade lvl: %d IP", 1);
    _text_drawText(BVcost, WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.115, gameFont_36);
    */

    _infinityMenu_mouseHandler();

    actualize();
    usleep(1000000 / FPS);
}
