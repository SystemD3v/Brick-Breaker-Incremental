#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "function.h"
#include "brick_patterns.h"
#include "playerData.h"


int numberOfPatterns = 2;

// Define dynamic 2d arrays
struct gameBrick** brickMap;
int** pattern1;
int** pattern2;
int** patternBlank;


// Allocate memory for arrays
void _pattern_allocateMemory() {

    brickMap = malloc(NB_BRICK_WIDTH * sizeof(struct gameBrick*));
    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        brickMap[i] = malloc(NB_BRICK_HEIGHT * sizeof(struct gameBrick));
    }


    pattern1 = malloc(NB_BRICK_WIDTH * sizeof(int*));
    pattern2 = malloc(NB_BRICK_WIDTH * sizeof(int*));
    patternBlank = malloc(NB_BRICK_WIDTH * sizeof(int*));

    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        pattern1[i] = malloc(NB_BRICK_HEIGHT * sizeof(int));
        pattern2[i] = malloc(NB_BRICK_HEIGHT * sizeof(int));
        patternBlank[i] = malloc(NB_BRICK_HEIGHT * sizeof(int));
    }
}


// Free the memory cached
void _pattern_freeMemory() {
    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        free(brickMap[i]);
        free(pattern1[i]);
        free(pattern2[i]);
        free(patternBlank[i]);
    }
    free(brickMap);
    free(pattern1);
    free(pattern2);
    free(patternBlank);
}


// Init pattern 1
void _pattern_initPattern1() {
    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        for (int j = 0; j < NB_BRICK_HEIGHT; j++) {
            if (i % 2 == 0) {
                pattern1[i][j] = 0;
            } else {
                pattern1[i][j] = 1;
            }
        }
    }
}

// Init pattern 2
void _pattern_initPattern2() {
    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        for (int j = 0; j < NB_BRICK_HEIGHT; j++) {
            if (i == 0 || i == NB_BRICK_WIDTH - 1 || j == 0 || j == NB_BRICK_HEIGHT - 1) {
                pattern2[i][j] = 1;
            } else if (i % 4 == 0 || j % 4 == 0) {
                pattern2[i][j] = 1;
            } else {
                pattern2[i][j] = 0;
            }
        }
    }
}

int destColor; // 0=R; 1=G; 2=B;
void brickColorHandler(struct gameBrick **dest, int i, int j) {
    switch (_data_bricksColorSelected) {
        case 4:
            switch(destColor) {
                case 0:
                    dest[i][j].colorR = (j * (255 / NB_BRICK_HEIGHT) + i * (255 / NB_BRICK_WIDTH))/2;
                    dest[i][j].colorG = 0;
                    dest[i][j].colorB = 0;
                    dest[i][j].colorBorderR = 255;
                    dest[i][j].colorBorderG = 0;
                    dest[i][j].colorBorderB = 0;
                    break;
                case 1:
                    dest[i][j].colorR = 0;
                    dest[i][j].colorG = (j * (255 / NB_BRICK_HEIGHT) + i * (255 / NB_BRICK_WIDTH))/2;
                    dest[i][j].colorB = 0;
                    dest[i][j].colorBorderR = 0;
                    dest[i][j].colorBorderG = 255;
                    dest[i][j].colorBorderB = 0;
                    break;
                case 2:
                    dest[i][j].colorR = 0;
                    dest[i][j].colorG = 0;
                    dest[i][j].colorB = (j * (255 / NB_BRICK_HEIGHT) + i * (255 / NB_BRICK_WIDTH))/2;
                    dest[i][j].colorBorderR = 0;
                    dest[i][j].colorBorderG = 0;
                    dest[i][j].colorBorderB = 255;
                    break;
            }
            break;
        case 3:
            switch(destColor) {
                case 0:
                    dest[i][j].colorR = j * (255 / NB_BRICK_HEIGHT);
                    dest[i][j].colorG = 0;
                    dest[i][j].colorB = 0;
                    dest[i][j].colorBorderR = 255;
                    dest[i][j].colorBorderG = 0;
                    dest[i][j].colorBorderB = 0;
                    break;
                case 1:
                    dest[i][j].colorR = 0;
                    dest[i][j].colorG = j * (255 / NB_BRICK_HEIGHT);
                    dest[i][j].colorB = 0;
                    dest[i][j].colorBorderR = 0;
                    dest[i][j].colorBorderG = 255;
                    dest[i][j].colorBorderB = 0;
                    break;
                case 2:
                    dest[i][j].colorR = 0;
                    dest[i][j].colorG = 0;
                    dest[i][j].colorB = j * (255 / NB_BRICK_HEIGHT);
                    dest[i][j].colorBorderR = 0;
                    dest[i][j].colorBorderG = 0;
                    dest[i][j].colorBorderB = 255;
                    break;
            }
            break;
        case 2:
            switch(destColor) {
                case 0:
                    dest[i][j].colorR = i * (255 / NB_BRICK_WIDTH);
                    dest[i][j].colorG = 0;
                    dest[i][j].colorB = 0;
                    dest[i][j].colorBorderR = 255;
                    dest[i][j].colorBorderG = 0;
                    dest[i][j].colorBorderB = 0;
                    break;
                case 1:
                    dest[i][j].colorR = 0;
                    dest[i][j].colorG = i * (255 / NB_BRICK_WIDTH);
                    dest[i][j].colorB = 0;
                    dest[i][j].colorBorderR = 0;
                    dest[i][j].colorBorderG = 255;
                    dest[i][j].colorBorderB = 0;
                    break;
                case 2:
                    dest[i][j].colorR = 0;
                    dest[i][j].colorG = 0;
                    dest[i][j].colorB = i * (255 / NB_BRICK_WIDTH);
                    dest[i][j].colorBorderR = 0;
                    dest[i][j].colorBorderG = 0;
                    dest[i][j].colorBorderB = 255;
                    break;
            }
            break;
        case 1:
            dest[i][j].colorR = rand() % 255;
            dest[i][j].colorG = rand() % 255;
            dest[i][j].colorB = rand() % 255;
            dest[i][j].colorBorderR = rand() % 255;
            dest[i][j].colorBorderG = rand() % 255;
            dest[i][j].colorBorderB = rand() % 255;
            break;
        case 0:
            dest[i][j].colorR = 255;
            dest[i][j].colorG = 0;
            dest[i][j].colorB = 0;
            dest[i][j].colorBorderR = 0;
            dest[i][j].colorBorderG = 0;
            dest[i][j].colorBorderB = 255;
            break;

    }
}

// Copy a pattern to the brickMap
void _pattern_copyToMap(struct gameBrick** dest, int** src) {
    destColor = rand() % 3;
    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        for (int j = 0; j < NB_BRICK_HEIGHT; j++) {

            if (src[i][j] == 1) {
                dest[i][j].x = i * MAX_BRICKS_WIDTH / NB_BRICK_WIDTH;
                dest[i][j].y = j * MAX_BRICKS_HEIGHT / NB_BRICK_HEIGHT;
                dest[i][j].sizeX = MAX_BRICKS_WIDTH / NB_BRICK_WIDTH;
                dest[i][j].sizeY = MAX_BRICKS_HEIGHT / NB_BRICK_HEIGHT;
                dest[i][j].hit = 0;
                brickColorHandler(dest, i, j);
            } else {
                dest[i][j].x = -1;
                dest[i][j].y = -1;
                dest[i][j].sizeX = 0;
                dest[i][j].sizeY = 0;
                dest[i][j].hit = 1;

            }
        }
    }
}

// Loads pattern in brickMap
void _pattern_loadPattern(int patternNumber) {
    switch (patternNumber) {
        case 1:
            _pattern_copyToMap(brickMap, pattern1);
            break;
        case 2:
            _pattern_copyToMap(brickMap, pattern2);
            break;
        default:
            break;
    }
}


// Init all patterns
void _pattern_initAll() {
    _pattern_initPattern1();
    _pattern_initPattern2();
}


// Display bricks on the board
void _pattern_displayBricks() {
    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        for (int j = 0; j < NB_BRICK_HEIGHT; j++) {
            //printf("%d, %d, %d, %d.\n", brickMap[i][j].x, brickMap[i][j].y, brickMap[i][j].sizeX, brickMap[i][j].sizeY);
            //printf("%d, %d, %d, %d, %d, %d\n", brickMap[i][j].colorBorderR, brickMap[i][j].colorBorderG, brickMap[i][j].colorBorderB,brickMap[i][j].colorR, brickMap[i][j].colorG, brickMap[i][j].colorB);
            changeColor(brickMap[i][j].colorBorderR, brickMap[i][j].colorBorderG, brickMap[i][j].colorBorderB);
            drawRect(brickMap[i][j].x, brickMap[i][j].y, brickMap[i][j].sizeX, brickMap[i][j].sizeY);
            changeColor(brickMap[i][j].colorR, brickMap[i][j].colorG, brickMap[i][j].colorB);
            drawRect(brickMap[i][j].x + 1, brickMap[i][j].y + 1, brickMap[i][j].sizeX - 2, brickMap[i][j].sizeY - 2);
        }
    }
}
