#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"


// Window & renderer settings
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int window_width = 0;
int window_height = 0;
char* name = "SDL app";


// Run settings
int programLaunched = 1;


// Keydown checks
int keyDownQ;
int keyDownD;


// Define main structures
struct gameBall ball;
struct gameHitbar bar;

// Init window parameters
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
int FPS = 60;


// Init bricks/patterns parameters
int NB_BRICK_WIDTH = 20;  // Default: 20
int NB_BRICK_HEIGHT = 10; // Default: 10
int MAX_BRICKS_WIDTH;
int MAX_BRICKS_HEIGHT;


// Init maximums
void _constants_initBrickMaximums() {
    MAX_BRICKS_WIDTH = WINDOW_WIDTH;
    MAX_BRICKS_HEIGHT = WINDOW_HEIGHT / 2;
    //printf("%d, %d.\n", WINDOW_HEIGHT, WINDOW_WIDTH);
}


// Init gamespeed
float BAR_SPEED_MULTIPLIER = 2;


// Audio filepaths
char* audioFilepaths[ENUM_audioFiles_NUMBER_OF_FILES] = {
    "../assets/audios/collision_between_ball_and_brick.wav",
    "../assets/audios/buy_success.wav",
    "../assets/audios/buy_failure.wav",
    "../assets/audios/small_click.wav",
    "../assets/audios/long_click.wav",
    "../assets/audios/death.wav",
};


// Font settings
TTF_Font* gameFont_16;
TTF_Font* gameFont_18;
TTF_Font* gameFont_24;
TTF_Font* gameFont_28;
TTF_Font* gameFont_36;
TTF_Font* gameFont_52;
TTF_Font* gameFont_128;
TTF_Font* gameFont_256;
void _constants_loadFont() {
    gameFont_16 = TTF_OpenFont("../assets/fonts/main_font.ttf", 16);
    gameFont_18 = TTF_OpenFont("../assets/fonts/main_font.ttf", 18);
    gameFont_24 = TTF_OpenFont("../assets/fonts/main_font.ttf", 24);
    gameFont_28 = TTF_OpenFont("../assets/fonts/main_font.ttf", 28);
    gameFont_36 = TTF_OpenFont("../assets/fonts/main_font.ttf", 36);
    gameFont_52 = TTF_OpenFont("../assets/fonts/main_font.ttf", 52);
    gameFont_128 = TTF_OpenFont("../assets/fonts/main_font.ttf", 128);
    gameFont_256 = TTF_OpenFont("../assets/fonts/main_font.ttf", 256);
}
SDL_Color textColor = {255, 0, 0, 255};


// Upgrade menu settings
int IN_UPGRADE_MENU = 0;
int IN_INFINITY_MENU = 0;

int maxBrickCostLevelValue = 22;
int maxGameSpeedLevelValue = 15;
int maxLossPreventionLevelValue = 11;


// Misc settings
