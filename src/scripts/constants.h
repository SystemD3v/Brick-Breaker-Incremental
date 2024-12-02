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


// Shaders settings
extern int shaderSelected;


// Run settings
extern int programLaunched;
extern int inEvent;
extern int eventId;


// Keydown checks
extern int keyDownQ;
extern int keyDownD;


// Defining base structures
struct gameBall {
    int size;
    float x;
    float y;
    float velocityX;
    float velocityY;
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
extern float BAR_SPEED_MULTIPLIER;


// Music settings
extern int mainMusicPlaying;
extern int shopMusicPlaying;
extern int infinityMusicPlaying;

// Audio settings
#define MAX_AUDIO_CHANNELS 64
enum audioChannels {
    ENUM_audioChannels_CHANNEL0,
    ENUM_audioChannels_BACKGROUND,
    ENUM_audioChannels_COLLISION_SOUND_EFFECT,
    ENUM_audioChannels_UPGRADE_MENU
};

// Audio files
enum audioFiles {
    ENUM_audioFiles_COLLISION_BALL_BRICK,
    ENUM_audioFiles_BUY_SUCCESS_SFX,
    ENUM_audioFiles_BUY_FAILURE_SFX,
    ENUM_audioFiles_SMALL_CLICK_SFX,
    ENUM_audioFiles_LONG_CLICK_SFX,
    ENUM_audioFiles_DEATH_SFX,
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
extern TTF_Font* gameFont_128;
extern TTF_Font* gameFont_256;
void _constants_loadFont();
extern SDL_Color textColor;


// Upgrade menu settings
extern int IN_UPGRADE_MENU;
extern int IN_INFINITY_MENU;

extern int maxGameSpeedLevelValue;
extern int maxLossPreventionLevelValue;

// Misc settings
#define DVD_MODE 0

#endif
