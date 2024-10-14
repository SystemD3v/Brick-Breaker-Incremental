#pragma once

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>



#ifndef CONSTANTS_H
#define CONSTANTS_H


// Debug settings
#define DEBUG_MODE 0


// Window and renderer settings
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int window_width;
extern int window_height;
extern char* name;

// Defining base structures
struct gameBall {
    int size;
    int x;
    int y;
    int velocityX;
    int velocityY;
};
extern struct gameBall ball;

struct gameHitbar {
    int sizeX;
    int sizeY;
    int x;
    int y;
    int velocityX;
};
extern struct gameHitbar bar;


// Window and overall experience settings
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int FPS;


// Brick & patterns settings
extern int NB_BRICK_WIDTH;
extern int NB_BRICK_HEIGHT;
extern int MAX_BRICKS_WIDTH;
extern int MAX_BRICKS_HEIGHT;
void _constants_initBrickMaximums();


// Gameplay settings
extern float BALL_SPEED;
extern float BAR_SPEED_MULTIPLIER;


// Audio settings
#define MAX_AUDIO_CHANNELS 8
enum audioChannels {
    ENUM_audioChannels_BACKGROUND,
    ENUM_audioChannels_COLLISION_SOUND_EFFECT
};

// Audio files
enum audioFiles {
    ENUM_audioFiles_COLLISION_BALL_BRICK,
    ENUM_audioFiles_NUMBER_OF_FILES // To keep track of the... NUMBER_OF_FILES!!
};
extern char* audioFilepaths[ENUM_audioFiles_NUMBER_OF_FILES];


// Font settings
extern TTF_Font* gameFont_16;
extern TTF_Font* gameFont_18;
extern TTF_Font* gameFont_24;
extern TTF_Font* gameFont_28;
extern TTF_Font* gameFont_36;
extern TTF_Font* gameFont_52;
void _constants_loadFont();
extern SDL_Color textColor;


// Misc settings
#define DVD_MODE 0

#endif
