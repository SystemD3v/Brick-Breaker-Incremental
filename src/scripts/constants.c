#include <stdio.h>
#include <SDL2/SDL.h>

#include "constants.h"

// Define main structures
struct gameBall ball;
struct gameHitbar bar;

// Init window parameters
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
int FPS = 60;


// Init bricks/patterns parameters
int NB_BRICK_WIDTH = 20;
int NB_BRICK_HEIGHT = 10;
int MAX_BRICKS_WIDTH;
int MAX_BRICKS_HEIGHT;


// Init maximums
void _constants_initBrickMaximums() {
    MAX_BRICKS_WIDTH = WINDOW_WIDTH;
    MAX_BRICKS_HEIGHT = WINDOW_HEIGHT / 2;
    //printf("%d, %d.\n", WINDOW_HEIGHT, WINDOW_WIDTH);
}


// Init gamespeed
float BALL_SPEED = 5;
float BAR_SPEED_MULTIPLIER = 2;


// Audio filepaths
char* audioFilepaths[ENUM_audioFiles_NUMBER_OF_FILES] = {
    "../assets/audios/collision_between_ball_and_brick.wav"
};
