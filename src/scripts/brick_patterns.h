#pragma once

// Define bricks' structure
struct gameBrick {
    int x;
    int y;
    int sizeX;
    int sizeY;
    int hit;
};


extern struct gameBrick **brickMap;

extern int **patternBlank;
extern int **pattern1;
extern int **pattern2;


void _pattern_allocateMemory();
void _pattern_freeMemory();

void _pattern_initPattern1();
void _pattern_initPattern2();
void _pattern_initAll();

void _pattern_copyToMap(struct gameBrick **dest, int **src);
void _pattern_loadPattern(int patternNumber);

void _pattern_displayBricks();
