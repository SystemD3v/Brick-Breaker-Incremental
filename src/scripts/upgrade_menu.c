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


int currentPage = 0;
int aestheticsUpgradesLayer = 0;
int xMouse, yMouse;

void drawBackgroundShop() {
    switch (_data_backgroundShopSelected) {
        case 4:
            sprite(0 ,0, "../assets/imgs/backgrounds/shop/tier4bg.bmp");
            break;
        case 3:
            sprite(0 ,0, "../assets/imgs/backgrounds/shop/tier3bg.bmp");
            break;
        case 2:
            sprite(0 ,0, "../assets/imgs/backgrounds/shop/tier2bg.bmp");
            break;
        case 1:
            sprite(0 ,0, "../assets/imgs/backgrounds/shop/tier1bg.bmp");
            break;
        default:
            break;
    }
}

int animationFrame = 0;
int maxWhiggleSize = 40;
int whiggleSpacing = 35;
int whiggleDirection = 0;
int textStartPosition = 0;
float whigglePosition = 0;
float whiggleYspacing = 10;
float speed = 0.12;
float amount = 0.5;
void whiggleText(char* text, int positionX, int positionY, TTF_Font* font) {
    int charLength = strlen(text);

    for (int letter = 0; letter < charLength; letter++) {
        char character[2] = { text[letter], '\0' };

        float angle = (animationFrame * speed) + (letter * amount);
        int yOffset = (int)(sin(angle) * maxWhiggleSize / 2);

        // Draw the character at the calculated position
        _text_drawText(character, positionX + whiggleSpacing * letter, positionY + yOffset, font);
    }

    animationFrame = (animationFrame + 1); //% maxWhiggleSize;
}



void displayPage0() {
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("Welcome to the upgrade shop! Press A to exit.", 0, 0, gameFont_52);


    // Brick cost upgrade. TLC:(WW*0.1; WH*0.1); SIZE:(WW*0.8; WH*0.15); BORDER:
    changeColor(55, 55, 55);
    drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1, WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.15);
    changeColor(185, 185, 185);
    drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.11, WINDOW_WIDTH * 0.78, WINDOW_HEIGHT * 0.13);
    _text_changeColor(255, 255, 255, 255);
    char BVcost[300];
    // Function to compute the cost of the upgrade: (x-1)^2 + 10x^2 + 1
    // (pow((_data_upgradeBrickCostLevel - 1), 2) + pow((10 * _data_upgradeBrickCostLevel), 2)))
    if (!(maxBrickCostLevelValue == _data_upgradeBrickCostLevel)) {
        sprintf(BVcost, "Brick value multiplier x2: $%d", (int)(pow((_data_upgradeBrickCostLevel - 1), 2) + pow((10 * _data_upgradeBrickCostLevel), 2)));
        _text_drawText(BVcost, WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.115, gameFont_36);
    } else {
         _text_drawText("Brick value multiplier x2: MAXED OUT!", WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.115, gameFont_36);
    }
    int maxBarSize1 = WINDOW_WIDTH * 0.7;
    changeColor(55, 55, 55);
    drawRect(WINDOW_WIDTH * 0.15, WINDOW_HEIGHT * 0.175, maxBarSize1, WINDOW_HEIGHT * 0.05);
    changeColor(120, 120, 255);
    drawRect(WINDOW_WIDTH * 0.152, WINDOW_HEIGHT * 0.177, _data_upgradeBrickCostLevel * (maxBarSize1/maxBrickCostLevelValue) - WINDOW_WIDTH * 0.002, WINDOW_HEIGHT * 0.046);



    // Max brick width upgrade.
    changeColor(55, 55, 55);
    drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.3, WINDOW_WIDTH * 0.39, WINDOW_HEIGHT * 0.15);
    changeColor(185, 185, 185);
    drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.31, WINDOW_WIDTH * 0.37, WINDOW_HEIGHT * 0.13);
    _text_changeColor(255, 255, 255, 255);
    char BWcost[300];
    // Function to compute the cost of the upgrade: (1.16*_data_upgradeWidthLevel)^2 + 4.95*_data_upgradeWidthLevel + 16.6
    // (1.16*_data_upgradeWidthLevel)^2 + 4.95*_data_upgradeWidthLevel + 16.6
    sprintf(BWcost, "Add a width layer: $%d", (int)(pow((1.16*_data_upgradeWidthLevel), 2) + 4.95*_data_upgradeWidthLevel + 16.6));
    _text_drawText(BWcost, WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.315, gameFont_36);


    // Max brick height upgrade.
    changeColor(55, 55, 55);
    drawRect(WINDOW_WIDTH * 0.51, WINDOW_HEIGHT * 0.3, WINDOW_WIDTH * 0.39, WINDOW_HEIGHT * 0.15);
    changeColor(185, 185, 185);
    drawRect(WINDOW_WIDTH * 0.52, WINDOW_HEIGHT * 0.31, WINDOW_WIDTH * 0.37, WINDOW_HEIGHT * 0.13);
    _text_changeColor(255, 255, 255, 255);
    char BHcost[300];
    // Function to compute the cost of the upgrade: (5.93*_data_upgradeHeightLevel)^2 - 6.24*_data_upgradeHeightLevel + 45.6
    // (5.93*_data_upgradeHeightLevel)^2 - 6.24*_data_upgradeHeightLevel + 43.6
    sprintf(BWcost, "Add a height layer: $%d", (int)(pow((5.93*_data_upgradeHeightLevel), 2) - 6.24*_data_upgradeHeightLevel + 45.6));
    _text_drawText(BWcost, WINDOW_WIDTH * 0.525, WINDOW_HEIGHT * 0.315, gameFont_36);


    // Ball speed upgrade.
    changeColor(55, 55, 55);
    drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.5, WINDOW_WIDTH * 0.39, WINDOW_HEIGHT * 0.15);
    changeColor(185, 185, 185);
    drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.51, WINDOW_WIDTH * 0.37, WINDOW_HEIGHT * 0.13);
    _text_changeColor(255, 255, 255, 255);
    char BScost[300];
    // Function to compute the cost of the upgrade: -1
    // -1
    if (!(_data_gameSpeed == maxGameSpeedLevelValue)) {
        sprintf(BScost, "Make the ball faster (+1): $%d", -1);
        _text_drawText(BScost, WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.515, gameFont_36);
        char BSvalue[300];
        sprintf(BSvalue, "Current speed: %d", _data_gameSpeed);
        _text_drawText(BSvalue, WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.575, gameFont_36);
    } else {
        _text_drawText("Make the ball faster: MAX!", WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.515, gameFont_36);
        char BSvalue[300];
        sprintf(BSvalue, "Current speed: %d", _data_gameSpeed);
        _text_drawText(BSvalue, WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.575, gameFont_36);
    }


    // Loss upon death upgrade.
    changeColor(55, 55, 55);
    drawRect(WINDOW_WIDTH * 0.51, WINDOW_HEIGHT * 0.5, WINDOW_WIDTH * 0.39, WINDOW_HEIGHT * 0.15);
    changeColor(185, 185, 185);
    drawRect(WINDOW_WIDTH * 0.52, WINDOW_HEIGHT * 0.51, WINDOW_WIDTH * 0.37, WINDOW_HEIGHT * 0.13);
    _text_changeColor(255, 255, 255, 255);
    char BDcost[300];
    // Function to compute the cost of the upgrade: -1
    // -1
    if (!(_data_lossPreventionLevel == maxLossPreventionLevelValue)) {
        sprintf(BScost, "Loose less upon death (-1%): $%d", -1);
        _text_drawText(BScost, WINDOW_WIDTH * 0.525, WINDOW_HEIGHT * 0.515, gameFont_36);
        char BDvalue[300];
        sprintf(BDvalue, "Current loss: %d%%", maxLossPreventionLevelValue - _data_lossPreventionLevel);
        _text_drawText(BDvalue, WINDOW_WIDTH * 0.525, WINDOW_HEIGHT * 0.575, gameFont_36);
    } else {
        _text_drawText("Loose less upon death: MAX!", WINDOW_WIDTH * 0.525, WINDOW_HEIGHT * 0.515, gameFont_36);
        char BDvalue[300];
        sprintf(BDvalue, "Current loss: %d%%", maxLossPreventionLevelValue - _data_lossPreventionLevel);
        _text_drawText(BDvalue, WINDOW_WIDTH * 0.525, WINDOW_HEIGHT * 0.575, gameFont_36);
    }


    // R to reset upgrade.
    changeColor(55, 55, 55);
    drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.7, WINDOW_WIDTH * 0.39, WINDOW_HEIGHT * 0.15);
    changeColor(185, 185, 185);
    drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.71, WINDOW_WIDTH * 0.37, WINDOW_HEIGHT * 0.13);
    _text_changeColor(255, 255, 255, 255);
    char BRcost[300];
    // Function to compute the cost of the upgrade: -1
    // -1
    if (!(_data_rToReset)) {
        sprintf(BRcost, "R to reset pattern: $%d", -1);
        _text_drawText(BRcost, WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.715, gameFont_36);
    } else {
        _text_drawText("R to reset pattern: BOUGHT!", WINDOW_WIDTH * 0.115, WINDOW_HEIGHT * 0.715, gameFont_36);
    }


    // T to change pattern upgrade.
    changeColor(55, 55, 55);
    drawRect(WINDOW_WIDTH * 0.51, WINDOW_HEIGHT * 0.7, WINDOW_WIDTH * 0.39, WINDOW_HEIGHT * 0.15);
    changeColor(185, 185, 185);
    drawRect(WINDOW_WIDTH * 0.52, WINDOW_HEIGHT * 0.71, WINDOW_WIDTH * 0.37, WINDOW_HEIGHT * 0.13);
    _text_changeColor(255, 255, 255, 255);
    char BTcost[300];
    // Function to compute the cost of the upgrade: -1
    // -1
    if (!(_data_tToPattern)) {
        sprintf(BTcost, "T to change pattern: $%d", -1);
        _text_drawText(BTcost, WINDOW_WIDTH * 0.525, WINDOW_HEIGHT * 0.715, gameFont_36);
    } else {
        _text_drawText("T to change pattern: BOUGHT!", WINDOW_WIDTH * 0.525, WINDOW_HEIGHT * 0.715, gameFont_36);
    }

    _text_drawText("Next page -->", WINDOW_WIDTH * 0.89, WINDOW_HEIGHT * 0.975, gameFont_28);
}


void page1upgrade0() {
    _text_changeColor(255, 255, 255, 255);
    // Backgrounds
    whiggleText("Backgrounds", WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.2, gameFont_52);


    _text_changeColor(0, 0, 255, 255);
    _text_drawText("Game background", WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.285, gameFont_36);

    changeColor(155, 155, 155);

    drawRect(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.024, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.002);

    drawSquare(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.32, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.2025, WINDOW_HEIGHT * 0.325, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("1", WINDOW_WIDTH * 0.205, WINDOW_HEIGHT * 0.325, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.32, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.3525, WINDOW_HEIGHT * 0.325, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("2", WINDOW_WIDTH * 0.355, WINDOW_HEIGHT * 0.325, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.50, WINDOW_HEIGHT * 0.32, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.5025, WINDOW_HEIGHT * 0.325, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("3", WINDOW_WIDTH * 0.505, WINDOW_HEIGHT * 0.325, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.32, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.6525, WINDOW_HEIGHT * 0.325, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("4", WINDOW_WIDTH * 0.655, WINDOW_HEIGHT * 0.325, gameFont_52);




    _text_changeColor(0, 0, 255, 255);
    _text_drawText("Shop background", WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.465, gameFont_36);

    changeColor(155, 155, 155);

    drawRect(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.024, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.002);

    drawSquare(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.50, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.2025, WINDOW_HEIGHT * 0.505, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("1", WINDOW_WIDTH * 0.205, WINDOW_HEIGHT * 0.505, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.50, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.3525, WINDOW_HEIGHT * 0.505, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("2", WINDOW_WIDTH * 0.355, WINDOW_HEIGHT * 0.505, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.50, WINDOW_HEIGHT * 0.50, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.5025, WINDOW_HEIGHT * 0.505, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("3", WINDOW_WIDTH * 0.505, WINDOW_HEIGHT * 0.505, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.50, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.6525, WINDOW_HEIGHT * 0.505, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("4", WINDOW_WIDTH * 0.655, WINDOW_HEIGHT * 0.505, gameFont_52);





    _text_changeColor(0, 0, 255, 255);
    _text_drawText("Infinity background", WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.645, gameFont_36);

    changeColor(155, 155, 155);

    drawRect(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.024, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.002);

    drawSquare(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.68, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.2025, WINDOW_HEIGHT * 0.685, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("1", WINDOW_WIDTH * 0.205, WINDOW_HEIGHT * 0.685, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.68, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.3525, WINDOW_HEIGHT * 0.685, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("2", WINDOW_WIDTH * 0.355, WINDOW_HEIGHT * 0.685, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.50, WINDOW_HEIGHT * 0.68, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.5025, WINDOW_HEIGHT * 0.685, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("3", WINDOW_WIDTH * 0.505, WINDOW_HEIGHT * 0.685, gameFont_52);

    changeColor(155, 155, 155);
    drawSquare(WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.68, WINDOW_WIDTH * 0.05);
    changeColor(255, 255, 255);
    drawSquare(WINDOW_WIDTH * 0.6525, WINDOW_HEIGHT * 0.685, WINDOW_WIDTH * 0.045);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText("4", WINDOW_WIDTH * 0.655, WINDOW_HEIGHT * 0.685, gameFont_52);




    if (xMouse >= WINDOW_WIDTH * 0.2 &&
        xMouse <= WINDOW_WIDTH * 0.25 &&
        yMouse >= WINDOW_HEIGHT * 0.32 &&
        yMouse <= WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 1 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundMain[0] == 0) {
            _text_drawText("Buy for $100", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else if (_data_backgroundMainSelected == 1) {
            _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
            _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
        } else {
            _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
            _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.35 &&
        xMouse <= WINDOW_WIDTH * 0.40 &&
        yMouse >= WINDOW_HEIGHT * 0.32 &&
        yMouse <= WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 2 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundMain[0] == 0) {
            _text_drawText("Required: tier 1!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundMain[1] == 0) {
                _text_drawText("Buy for $1000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundMainSelected == 2) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.50 &&
        xMouse <= WINDOW_WIDTH * 0.55 &&
        yMouse >= WINDOW_HEIGHT * 0.32 &&
        yMouse <= WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 3 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundMain[1] == 0) {
            _text_drawText("Required: tier 2!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundMain[2] == 0) {
                _text_drawText("Buy for $10000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundMainSelected == 3) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.65 &&
        xMouse <= WINDOW_WIDTH * 0.70 &&
        yMouse >= WINDOW_HEIGHT * 0.32 &&
        yMouse <= WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 4 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundMain[2] == 0) {
            _text_drawText("Required: tier 3!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundMain[3] == 0) {
                _text_drawText("Buy for $100000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundMainSelected == 4) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }





    if (xMouse >= WINDOW_WIDTH * 0.2 &&
        xMouse <= WINDOW_WIDTH * 0.25 &&
        yMouse >= WINDOW_HEIGHT * 0.50 &&
        yMouse <= WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 1 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundShop[0] == 0) {
            _text_drawText("Buy for $200", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else if (_data_backgroundShopSelected == 1) {
            _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
            _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
        } else {
            _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
            _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.35 &&
        xMouse <= WINDOW_WIDTH * 0.40 &&
        yMouse >= WINDOW_HEIGHT * 0.50 &&
        yMouse <= WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 2 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundShop[0] == 0) {
            _text_drawText("Required: tier 1!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundShop[1] == 0) {
                _text_drawText("Buy for $2000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundShopSelected == 2) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.50 &&
        xMouse <= WINDOW_WIDTH * 0.55 &&
        yMouse >= WINDOW_HEIGHT * 0.50 &&
        yMouse <= WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 3 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundShop[1] == 0) {
            _text_drawText("Required: tier 2!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundShop[2] == 0) {
                _text_drawText("Buy for $20000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundShopSelected == 3) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.65 &&
        xMouse <= WINDOW_WIDTH * 0.70 &&
        yMouse >= WINDOW_HEIGHT * 0.50 &&
        yMouse <= WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 4 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundShop[2] == 0) {
            _text_drawText("Required: tier 3!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundShop[3] == 0) {
                _text_drawText("Buy for $200000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundShopSelected == 4) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }





    if (xMouse >= WINDOW_WIDTH * 0.2 &&
        xMouse <= WINDOW_WIDTH * 0.25 &&
        yMouse >= WINDOW_HEIGHT * 0.68 &&
        yMouse <= WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 1 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundInf[0] == 0) {
            _text_drawText("Buy for $500", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else if (_data_backgroundInfSelected == 1) {
            _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
            _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
        } else {
            _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
            _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.35 &&
        xMouse <= WINDOW_WIDTH * 0.40 &&
        yMouse >= WINDOW_HEIGHT * 0.68 &&
        yMouse <= WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 2 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundInf[0] == 0) {
            _text_drawText("Required: tier 1!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundInf[1] == 0) {
                _text_drawText("Buy for $5000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundInfSelected == 2) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.50 &&
        xMouse <= WINDOW_WIDTH * 0.55 &&
        yMouse >= WINDOW_HEIGHT * 0.68 &&
        yMouse <= WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 3 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundInf[1] == 0) {
            _text_drawText("Required: tier 2!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundInf[2] == 0) {
                _text_drawText("Buy for $50000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundInfSelected == 3) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }


    if (xMouse >= WINDOW_WIDTH * 0.65 &&
        xMouse <= WINDOW_WIDTH * 0.70 &&
        yMouse >= WINDOW_HEIGHT * 0.68 &&
        yMouse <= WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.05
    ) {
        changeColor(120, 120, 120);
        drawRect(xMouse, yMouse, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1);
        changeColor(180, 180, 180);
        drawRect(xMouse + WINDOW_WIDTH * 0.005, yMouse + WINDOW_HEIGHT * 0.005, WINDOW_WIDTH * 0.19, WINDOW_HEIGHT * 0.09);
        _text_changeColor(255, 255, 255, 255);
        _text_drawText("Tier 4 background", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.01, gameFont_28);
        if (_data_upgradeBackgroundInf[2] == 0) {
            _text_drawText("Required: tier 3!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
        } else {
            if (_data_upgradeBackgroundInf[3] == 0) {
                _text_drawText("Buy for $500000", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.05, gameFont_28);
            } else if (_data_backgroundInfSelected == 4) {
                _text_drawText("Equipped!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to unequip.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            } else {
                _text_drawText("Not equipped.", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.04, gameFont_28);
                _text_drawText("Click to equip!", xMouse + WINDOW_WIDTH * 0.01, yMouse + WINDOW_HEIGHT * 0.065, gameFont_28);
            }
        }
    }

}


void displayPage1() {
    _text_changeColor(0, 0, 255, 255);
    _text_drawText("Gameplay upgrades! Graphics, SFX, and more!", 0, 0, gameFont_52);


    switch (aestheticsUpgradesLayer) {
        case 0:
            page1upgrade0();
            break;
        default:
            break;
    }


    _text_changeColor(255, 255, 255, 255);
    _text_drawText("<-- Previous page", 0, WINDOW_HEIGHT * 0.975, gameFont_28);
}


void clickPage0(int mousePosX, int mousePosY) {
    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_LONG_CLICK_SFX], ENUM_audioChannels_UPGRADE_MENU);


    // Check if mouse in first button
    if (mousePosX > WINDOW_WIDTH * 0.1 && mousePosX < WINDOW_WIDTH * 0.9 && mousePosY > WINDOW_HEIGHT * 0.1 && mousePosY < WINDOW_HEIGHT * 0.25) {
        if (maxBrickCostLevelValue == _data_upgradeBrickCostLevel) {
            return;
        }
        if (_data_cash >= (int)(pow((_data_upgradeBrickCostLevel - 1), 2) + pow((10 * _data_upgradeBrickCostLevel), 2))) {
            _data_cash -= (int)(pow((_data_upgradeBrickCostLevel - 1), 2) + pow((10 * _data_upgradeBrickCostLevel), 2));
            _data_upgradeBrickCostLevel += 1;
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
        } else {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
        }
        return;
    }


    // Check if mouse in second button
    if (mousePosX > WINDOW_WIDTH * 0.1 && mousePosX < WINDOW_WIDTH * 0.49 && mousePosY > WINDOW_HEIGHT * 0.3 && mousePosY < WINDOW_HEIGHT * 0.45) {
        if (_data_cash >= (int)(pow((1.16*_data_upgradeWidthLevel), 2) + 4.95*_data_upgradeWidthLevel + 16.6)) {
            _data_cash -= (int)(pow((1.16*_data_upgradeWidthLevel), 2) + 4.95*_data_upgradeWidthLevel + 16.6);
            _data_upgradeWidthLevel += 1;

            _pattern_freeMemory();

            _data_updateConstantsWithData();

            _pattern_allocateMemory();
            _pattern_initAll();
            _pattern_loadPattern(_data_currentPattern);
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
        } else {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
        }
    }


    // Check if mouse in third button
    if (mousePosX > WINDOW_WIDTH * 0.51 && mousePosX < WINDOW_WIDTH * 0.90 && mousePosY > WINDOW_HEIGHT * 0.3 && mousePosY < WINDOW_HEIGHT * 0.45) {
        if (_data_cash >= (int)(pow((5.93*_data_upgradeHeightLevel), 2) - 6.24*_data_upgradeHeightLevel + 45.6)) {
            _data_cash -= (int)(pow((5.93*_data_upgradeHeightLevel), 2) - 6.24*_data_upgradeHeightLevel + 45.6);
            _data_upgradeHeightLevel += 1;

            _pattern_freeMemory();

            _data_updateConstantsWithData();

            _pattern_allocateMemory();
            _pattern_initAll();
            _pattern_loadPattern(_data_currentPattern);
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
        } else {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
        }
    }

    // Check if mouse in fourth button
    if (mousePosX > WINDOW_WIDTH * 0.1 && mousePosX < WINDOW_WIDTH * 0.49 && mousePosY > WINDOW_HEIGHT * 0.5 && mousePosY < WINDOW_HEIGHT * 0.65) {
        if (_data_gameSpeed == maxGameSpeedLevelValue) {
            return;
        }
        if (_data_cash >= -1) {
            _data_cash -= -1;
            _data_gameSpeed += 1;

            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
        } else {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
        }
    }

    // Check if mouse in fifth button
    if (mousePosX > WINDOW_WIDTH * 0.51 && mousePosX < WINDOW_WIDTH * 0.90 && mousePosY > WINDOW_HEIGHT * 0.5 && mousePosY < WINDOW_HEIGHT * 0.65) {
        if (_data_lossPreventionLevel == maxLossPreventionLevelValue) {
            return;
        }
        if (_data_cash >= -1) {
            _data_cash -= -1;
            _data_lossPreventionLevel += 1;

            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
        } else {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
        }
    }

    // Check if mouse in sixth button
    if (mousePosX > WINDOW_WIDTH * 0.1 && mousePosX < WINDOW_WIDTH * 0.49 && mousePosY > WINDOW_HEIGHT * 0.7 && mousePosY < WINDOW_HEIGHT * 0.85) {
        if (_data_rToReset) {
            return;
        }
        if (_data_cash >= -1) {
            _data_cash -= -1;
            _data_rToReset += 1;

            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
        } else {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
        }
    }

    // Check if mouse in seventh button
    if (mousePosX > WINDOW_WIDTH * 0.51 && mousePosX < WINDOW_WIDTH * 0.90 && mousePosY > WINDOW_HEIGHT * 0.7 && mousePosY < WINDOW_HEIGHT * 0.85) {
        if (_data_tToPattern) {
            return;
        }
        if (_data_cash >= -1) {
            _data_cash -= -1;
            _data_tToPattern += 1;

            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
        } else {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
        }
    }

    if (mousePosX > WINDOW_WIDTH * 0.89 && mousePosY > WINDOW_HEIGHT * 0.975) {
        currentPage += 1;
    }
}

void clickPage1_0(int mousePosX, int mousePosY) {
    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_LONG_CLICK_SFX], ENUM_audioChannels_UPGRADE_MENU);

    if (mousePosX >= WINDOW_WIDTH * 0.2 &&
        mousePosX <= WINDOW_WIDTH * 0.25 &&
        mousePosY >= WINDOW_HEIGHT * 0.32 &&
        mousePosY <= WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundMain[0] == 0) {
            if (_data_cash >= 1000) {
                _data_cash -= 1000;
                _data_upgradeBackgroundMain[0] = 1;
                _data_backgroundMainSelected = 1;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundMainSelected != 1) {
                _data_backgroundMainSelected = 1;
            } else {
                _data_backgroundMainSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.35 &&
        mousePosX <= WINDOW_WIDTH * 0.40 &&
        mousePosY >= WINDOW_HEIGHT * 0.32 &&
        mousePosY <= WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundMain[0] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundMain[1] == 0) {
            if (_data_cash >= 10000) {
                _data_cash -= 10000;
                _data_upgradeBackgroundMain[1] = 1;
                _data_backgroundMainSelected = 2;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundMainSelected != 2) {
                _data_backgroundMainSelected = 2;
            } else {
                _data_backgroundMainSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.50 &&
        mousePosX <= WINDOW_WIDTH * 0.55 &&
        mousePosY >= WINDOW_HEIGHT * 0.32 &&
        mousePosY <= WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundMain[1] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundMain[2] == 0) {
            if (_data_cash >= 100000) {
                _data_cash -= 100000;
                _data_upgradeBackgroundMain[2] = 1;
                _data_backgroundMainSelected = 3;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundMainSelected != 3) {
                _data_backgroundMainSelected = 3;
            } else {
                _data_backgroundMainSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.65 &&
        mousePosX <= WINDOW_WIDTH * 0.70 &&
        mousePosY >= WINDOW_HEIGHT * 0.32 &&
        mousePosY <= WINDOW_HEIGHT * 0.32 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundMain[2] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundMain[3] == 0) {
            if (_data_cash >= 1000000) {
                _data_cash -= 1000000;
                _data_upgradeBackgroundMain[3] = 1;
                _data_backgroundMainSelected = 4;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundMainSelected != 4) {
                _data_backgroundMainSelected = 4;
            } else {
                _data_backgroundMainSelected = 0;
            }
        }
    }





    if (mousePosX >= WINDOW_WIDTH * 0.2 &&
        mousePosX <= WINDOW_WIDTH * 0.25 &&
        mousePosY >= WINDOW_HEIGHT * 0.50 &&
        mousePosY <= WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundShop[0] == 0) {
            if (_data_cash >= 2000) {
                _data_cash -= 2000;
                _data_upgradeBackgroundShop[0] = 1;
                _data_backgroundShopSelected = 1;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundShopSelected != 1) {
                _data_backgroundShopSelected = 1;
            } else {
                _data_backgroundShopSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.35 &&
        mousePosX <= WINDOW_WIDTH * 0.40 &&
        mousePosY >= WINDOW_HEIGHT * 0.50 &&
        mousePosY <= WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundShop[0] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundShop[1] == 0) {
            if (_data_cash >= 20000) {
                _data_cash -= 20000;
                _data_upgradeBackgroundShop[1] = 1;
                _data_backgroundShopSelected = 2;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundShopSelected != 2) {
                _data_backgroundShopSelected = 2;
            } else {
                _data_backgroundShopSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.50 &&
        mousePosX <= WINDOW_WIDTH * 0.55 &&
        mousePosY >= WINDOW_HEIGHT * 0.50 &&
        mousePosY <= WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundShop[1] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundShop[2] == 0) {
            if (_data_cash >= 200000) {
                _data_cash -= 200000;
                _data_upgradeBackgroundShop[2] = 1;
                _data_backgroundShopSelected = 3;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundShopSelected != 3) {
                _data_backgroundShopSelected = 3;
            } else {
                _data_backgroundShopSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.65 &&
        mousePosX <= WINDOW_WIDTH * 0.70 &&
        mousePosY >= WINDOW_HEIGHT * 0.50 &&
        mousePosY <= WINDOW_HEIGHT * 0.50 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundShop[2] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundShop[3] == 0) {
            if (_data_cash >= 2000000) {
                _data_cash -= 2000000;
                _data_upgradeBackgroundShop[3] = 1;
                _data_backgroundShopSelected = 4;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundShopSelected != 4) {
                _data_backgroundShopSelected = 4;
            } else {
                _data_backgroundShopSelected = 0;
            }
        }
    }





    if (mousePosX >= WINDOW_WIDTH * 0.2 &&
        mousePosX <= WINDOW_WIDTH * 0.25 &&
        mousePosY >= WINDOW_HEIGHT * 0.68 &&
        mousePosY <= WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundInf[0] == 0) {
            if (_data_cash >= 5000) {
                _data_cash -= 5000;
                _data_upgradeBackgroundInf[0] = 1;
                _data_backgroundInfSelected = 1;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundInfSelected != 1) {
                _data_backgroundInfSelected = 1;
            } else {
                _data_backgroundInfSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.35 &&
        mousePosX <= WINDOW_WIDTH * 0.40 &&
        mousePosY >= WINDOW_HEIGHT * 0.68 &&
        mousePosY <= WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundInf[0] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundInf[1] == 0) {
            if (_data_cash >= 50000) {
                _data_cash -= 50000;
                _data_upgradeBackgroundInf[1] = 1;
                _data_backgroundInfSelected = 2;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundInfSelected != 2) {
                _data_backgroundInfSelected = 2;
            } else {
                _data_backgroundInfSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.50 &&
        mousePosX <= WINDOW_WIDTH * 0.55 &&
        mousePosY >= WINDOW_HEIGHT * 0.68 &&
        mousePosY <= WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundInf[1] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundInf[2] == 0) {
            if (_data_cash >= 500000) {
                _data_cash -= 500000;
                _data_upgradeBackgroundInf[2] = 1;
                _data_backgroundInfSelected = 3;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundInfSelected != 3) {
                _data_backgroundInfSelected = 3;
            } else {
                _data_backgroundInfSelected = 0;
            }
        }
    }

    if (mousePosX >= WINDOW_WIDTH * 0.65 &&
        mousePosX <= WINDOW_WIDTH * 0.70 &&
        mousePosY >= WINDOW_HEIGHT * 0.68 &&
        mousePosY <= WINDOW_HEIGHT * 0.68 + WINDOW_WIDTH * 0.05
    ) {
        if (_data_upgradeBackgroundInf[2] == 0) {
            _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
            return;
        }
        if (_data_upgradeBackgroundInf[3] == 0) {
            if (_data_cash >= 5000000) {
                _data_cash -= 5000000;
                _data_upgradeBackgroundInf[3] = 1;
                _data_backgroundInfSelected = 4;
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            } else {
                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                return;
            }
        } else {
            if (_data_backgroundInfSelected != 4) {
                _data_backgroundInfSelected = 4;
            } else {
                _data_backgroundInfSelected = 0;
            }
        }
    }

    if (mousePosX < WINDOW_WIDTH * 0.15 && mousePosY > WINDOW_HEIGHT * 0.975) {
        currentPage -= 1;
    }
}

void _upgradeMenu_mouseHandler() {
    SDL_GetMouseState(&xMouse, &yMouse);
}

void selectPage() {
    _upgradeMenu_mouseHandler();
    drawBackgroundShop();
    switch (currentPage) {
        case 0:
            displayPage0();
            return;
            break;
        case 1:
            displayPage1();
            return;
            break;
        default:
            break;
    }
}


void selectClickPage(int mousePosX, int mousePosY) {
    _upgradeMenu_mouseHandler();
    switch (currentPage) {
        case 0:
            clickPage0(mousePosX, mousePosY);
            return;
            break;
        case 1:
            switch (aestheticsUpgradesLayer) {
                case 0:
                clickPage1_0(mousePosX, mousePosY);
                return;
                break;
            }
            return;
            break;
        default:
            break;
    }
}


void _upgradeMenu_displayMenu() {

    clear();

    selectPage();

    actualize();
    usleep(1000000 / FPS);
}


void _upgradeMenu_clickHandler(int mousePosX, int mousePosY) {

    selectClickPage(mousePosX, mousePosY);

}
