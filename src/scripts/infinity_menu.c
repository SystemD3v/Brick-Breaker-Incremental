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


void _infinityMenu_playMusic() {
    if (_data_AudioBGSelected >= 3) {
        if (infinityMusicPlaying == 0) {
            infinityMusicPlaying = 1;
            _audio_loadAndPlayLoop("../assets/audios/infinityMusic.wav", ENUM_audioChannels_BACKGROUND, -1);
        }
    } else {
        _audio_stopOnChannel(ENUM_audioChannels_BACKGROUND);
    }
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

int currentFrame = 0;
void textAnimation(char* entryText, int posX, int posY, TTF_Font* font) {

    char* textToDraw = entryText;
    int textLength = strlen(entryText);
    int textPositionX = posX;
    int textPositionY = posY;
    int textSpacing = 20;
    int randomMovementMaximum = 4;

    for (int i = 0; i < textLength; i++) {
        char character[2] = { textToDraw[i], '\0' };
        _text_drawText(character, textPositionX + i * textSpacing + rand() % randomMovementMaximum, textPositionY + rand() % randomMovementMaximum, font);
    }

}



void _infinityMenu_displayMenu() {

    clear();

    drawBackgroundInf();

    _text_changeColor(255, 0, 255, 255);
    _text_drawText("Welcome to the infinity tree! Press I to exit.", 0, 0, gameFont_52);
    _text_drawText("*IP = \"infinity (points)\"", 0, 54, gameFont_18);

    // Upgrade 1
    if (_infinityMenu_checkDependencies("1")) {
        changeColor(55, 0, 55);
        drawRect(WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.1, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1);
        changeColor(185, 0, 185);
        drawRect(WINDOW_WIDTH * 0.46, WINDOW_HEIGHT * 0.11, WINDOW_WIDTH * 0.08, WINDOW_HEIGHT * 0.08);
        _text_changeColor(255, 0, 255, 255);
        _text_drawText("1", WINDOW_WIDTH * 0.47, WINDOW_HEIGHT * 0.12, gameFont_52);
    }

    // Upgrade 2-1
    if (_infinityMenu_checkDependencies("2-1")) {
        changeColor(55, 0, 55);
        drawRect(WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.25, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1);
        changeColor(185, 0, 185);
        drawRect(WINDOW_WIDTH * 0.36, WINDOW_HEIGHT * 0.26, WINDOW_WIDTH * 0.08, WINDOW_HEIGHT * 0.08);
        _text_changeColor(255, 0, 255, 255);
        _text_drawText("2-1", WINDOW_WIDTH * 0.37, WINDOW_HEIGHT * 0.27, gameFont_52);
    }

    // Upgrade 2-2
    if (_infinityMenu_checkDependencies("2-2")) {
        changeColor(55, 0, 55);
        drawRect(WINDOW_WIDTH * 0.55, WINDOW_HEIGHT * 0.25, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1);
        changeColor(185, 0, 185);
        drawRect(WINDOW_WIDTH * 0.56, WINDOW_HEIGHT * 0.26, WINDOW_WIDTH * 0.08, WINDOW_HEIGHT * 0.08);
        _text_changeColor(255, 0, 255, 255);
        _text_drawText("2-2", WINDOW_WIDTH * 0.57, WINDOW_HEIGHT * 0.27, gameFont_52);
    }

    // Upgrade 3
    if (_infinityMenu_checkDependencies("3")) {
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


    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);


    if (_infinityMenu_checkDependencies("1")) {
        if (xMouse > WINDOW_WIDTH * 0.45 &&
            xMouse < WINDOW_WIDTH * 0.55 &&
            yMouse > WINDOW_HEIGHT * 0.1 &&
            yMouse < WINDOW_HEIGHT * 0.2
        ) {
            changeColor(55, 0, 55);
            drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.75, WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.2);
            changeColor(185, 0, 185);
            drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.76, WINDOW_WIDTH * 0.78, WINDOW_HEIGHT * 0.18);
            _text_changeColor(255, 0, 255, 255);
            textAnimation("Upgrade 1: increase max brick value x2 multiplier. (22 -> 30)", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.77, gameFont_36);
            if (_data_upgradeBought[0] == 0) {
                textAnimation("Cost: 1 infinity point.", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.85, gameFont_36);
            } else {
                textAnimation("Bought for 1 infinity point.", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.85, gameFont_36);
            }
        }
    }

    if (_infinityMenu_checkDependencies("2-1")) {
        if (xMouse > WINDOW_WIDTH * 0.35 &&
            xMouse < WINDOW_WIDTH * 0.45 &&
            yMouse > WINDOW_HEIGHT * 0.25 &&
            yMouse < WINDOW_HEIGHT * 0.35
        ) {
            changeColor(55, 0, 55);
            drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.75, WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.2);
            changeColor(185, 0, 185);
            drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.76, WINDOW_WIDTH * 0.78, WINDOW_HEIGHT * 0.18);
            _text_changeColor(255, 0, 255, 255);
            textAnimation("Upgrade 2-1: increase max brick value x2 multiplier again. (+1)", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.77, gameFont_36);
            if (_data_upgradeBought[1] == 0) {
                textAnimation("Cost: 10 infinity point.", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.85, gameFont_36);
            } else {
                textAnimation("Bought for 10 infinity point.", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.85, gameFont_36);
            }
        }
    }

    if (_infinityMenu_checkDependencies("2-2")) {
        if (xMouse > WINDOW_WIDTH * 0.55 &&
            xMouse < WINDOW_WIDTH * 0.65 &&
            yMouse > WINDOW_HEIGHT * 0.25 &&
            yMouse < WINDOW_HEIGHT * 0.35
        ) {
            changeColor(55, 0, 55);
            drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.75, WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.2);
            changeColor(185, 0, 185);
            drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.76, WINDOW_WIDTH * 0.78, WINDOW_HEIGHT * 0.18);
            _text_changeColor(255, 0, 255, 255);
            textAnimation("Upgrade 2-2: increase max brick value x2 multiplier yet again. (+1)", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.77, gameFont_36);
            if (_data_upgradeBought[2] == 0) {
                textAnimation("Cost: 20 infinity point.", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.85, gameFont_36);
            } else {
                textAnimation("Bought for 20 infinity point.", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.85, gameFont_36);
            }
        }
    }

    if (_infinityMenu_checkDependencies("3")) {
        if (xMouse > WINDOW_WIDTH * 0.45 &&
            xMouse < WINDOW_WIDTH * 0.55 &&
            yMouse > WINDOW_HEIGHT * 0.40 &&
            yMouse < WINDOW_HEIGHT * 0.50
        ) {
            changeColor(55, 0, 55);
            drawRect(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.75, WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.2);
            changeColor(185, 0, 185);
            drawRect(WINDOW_WIDTH * 0.11, WINDOW_HEIGHT * 0.76, WINDOW_WIDTH * 0.78, WINDOW_HEIGHT * 0.18);
            _text_changeColor(255, 0, 255, 255);
            textAnimation("Upgrade 3: The END.", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.77, gameFont_36);
            if (_data_upgradeBought[3] == 0) {
                textAnimation("Cost: 555 infinity point.", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.85, gameFont_36);
            } else {
                textAnimation("Ended for 555 infinity point. Thank you for playing. ", WINDOW_WIDTH * 0.12, WINDOW_HEIGHT * 0.85, gameFont_36);
            }
        }
    }

    currentFrame++;

    actualize();
    usleep(1000000 / FPS);
}


void _infinityMenu_clickHandler(int clickX, int clickY) {
    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_LONG_CLICK_SFX], ENUM_audioChannels_UPGRADE_MENU);


    if (_infinityMenu_checkDependencies("1")) {
        if (clickX > WINDOW_WIDTH * 0.45 &&
            clickX < WINDOW_WIDTH * 0.55 &&
            clickY > WINDOW_HEIGHT * 0.1 &&
            clickY < WINDOW_HEIGHT * 0.2
        ) {
            if (_data_upgradeBought[0] == 0) {
                if (_data_infinity >= 1) {
                    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                    _data_infinity--;
                    _data_upgradeBought[0] = 1;
                    _data_maxBrickCostLevelValue = 30;
                } else {
                    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                }
            }
        }
    }

    if (_infinityMenu_checkDependencies("2-1")) {
        if (clickX > WINDOW_WIDTH * 0.35 &&
            clickX < WINDOW_WIDTH * 0.45 &&
            clickY > WINDOW_HEIGHT * 0.25 &&
            clickY < WINDOW_HEIGHT * 0.35
        ) {
            if (_data_upgradeBought[1] == 0) {
                if (_data_infinity >= 10) {
                    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                    _data_infinity -= 10;
                    _data_upgradeBought[1] = 1;
                    _data_maxBrickCostLevelValue += 1;
                } else {
                    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                }
            }
        }
    }

    if (_infinityMenu_checkDependencies("2-2")) {
        if (clickX > WINDOW_WIDTH * 0.55 &&
            clickX < WINDOW_WIDTH * 0.65 &&
            clickY > WINDOW_HEIGHT * 0.25 &&
            clickY < WINDOW_HEIGHT * 0.35
        ) {
            if (_data_upgradeBought[2] == 0) {
                if (_data_infinity >= 20) {
                    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                    _data_infinity -= 20;
                    _data_upgradeBought[2] = 1;
                    _data_maxBrickCostLevelValue += 1;
                } else {
                    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                }
            }
        }
    }

    if (_infinityMenu_checkDependencies("3")) {
        if (clickX > WINDOW_WIDTH * 0.45 &&
            clickX < WINDOW_WIDTH * 0.55 &&
            clickY > WINDOW_HEIGHT * 0.40 &&
            clickY < WINDOW_HEIGHT * 0.50
        ) {
            if (_data_upgradeBought[3] == 0) {
                if (_data_infinity >= 555) {
                    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_SUCCESS_SFX], ENUM_audioChannels_UPGRADE_MENU);
                    _data_infinity -= 555;
                    _data_upgradeBought[3] = 1;
                } else {
                    _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_BUY_FAILURE_SFX], ENUM_audioChannels_UPGRADE_MENU);
                }
            }
        }
    }
}
