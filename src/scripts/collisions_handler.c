#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "function.h"
#include "brick_patterns.h"
#include "collisions_handler.h"
#include "audio_functions.h"
#include "playerData.h"
#include "cash_handler.h"
#include "text_handler.h"



void checkCollisionsWithBricks() {

    int isAtLeastOnePresent = 0;

    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        for (int j = 0; j < NB_BRICK_HEIGHT; j++) {

            // If already destroyed then skip this check
            if (brickMap[i][j].x == -1 && brickMap[i][j].y == -1) {
                continue;
            }

            isAtLeastOnePresent = 1;

            // Get current brick boundaries
            int brickLeft = brickMap[i][j].x;
            int brickRight = brickMap[i][j].x + brickMap[i][j].sizeX;
            int brickTop = brickMap[i][j].y;
            int brickBottom = brickMap[i][j].y + brickMap[i][j].sizeY;

            // Get ball boundaries
            int ballLeft = ball.x;
            int ballRight = ball.x + ball.size;
            int ballTop = ball.y;
            int ballBottom = ball.y + ball.size;

            // Check if the ball's bounding box intersects with the brick's bounding box
            if (ballRight > brickLeft && ballLeft < brickRight &&
                ballBottom > brickTop && ballTop < brickBottom) {

                // Get center of elements
                int ballCenterX = (ballLeft + ballRight) / 2;
                int ballCenterY = (ballTop + ballBottom) / 2;

                int brickCenterX = (brickLeft + brickRight) / 2;
                int brickCenterY = (brickTop + brickBottom) / 2;

                // Calculate differences between centers
                int deltaX = ballCenterX - brickCenterX;
                int deltaY = ballCenterY - brickCenterY;

                // Determine if collision is horizontal or vertical
                if (abs(deltaX) > abs(deltaY)) {

                    // Horizontal
                    if (deltaX > 0) {
                        // Ball hit right
                        ball.velocityX = abs(ball.velocityX);  // Vel right
                    } else {
                        // Ball hit left
                        ball.velocityX = -abs(ball.velocityX);  // Vel left
                    }

                } else {
                    // Vertical
                    if (deltaY > 0) {
                        // Ball hit the bottom of the brick
                        ball.velocityY = abs(ball.velocityY);  // Vel down
                    } else {
                        // Ball hit the top of the brick
                        ball.velocityY = -abs(ball.velocityY);  // Vel up
                    }
                }

                // Mark the brick as destroyed
                brickMap[i][j].x = -1;
                brickMap[i][j].y = -1;
                brickMap[i][j].sizeX = 0;
                brickMap[i][j].sizeY = 0;

                _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_COLLISION_BALL_BRICK], ENUM_audioChannels_COLLISION_SOUND_EFFECT);
                _cash_brickIncome(1);

                return;
            }
        }
    }

    if (!(isAtLeastOnePresent)) {
        _pattern_loadPattern(1);
    }
}

void checkBallCollisionWithBar() {
    // Check if ball made contact with bar
    if ((ball.x + ball.size >= bar.x && ball.x <= bar.x + bar.sizeX) &&
        (ball.y + ball.size >= bar.y && ball.y <= bar.y + bar.sizeY)) {

        // Ball hits the top of the bar
        ball.velocityY = -abs(ball.velocityY);  // Ball goes up up

        // Get centers X
        int barCenterX = bar.x + bar.sizeX / 2;
        int ballCenterX = ball.x + ball.size / 2;

        // Check for leftSide
        if (ballCenterX < barCenterX) {
            // Hit --> adjust
            ball.velocityX = -abs(ball.velocityX);  // Ball goes left left
        }
        // Check for rightSide
        else if (ballCenterX > barCenterX) {
            // Hit --> adjust
            ball.velocityX = abs(ball.velocityX);  // Ball goes right right
        }
        // If near center, reduce velocity X
        else {
            ball.velocityX *= 0.5;  // Gives impression of "straight-er" shot
        }
    }
}


void checkCollisionsWithBottom() {
    if (ball.y > WINDOW_HEIGHT * 0.985) {
        _audio_loadAndPlay(audioFilepaths[ENUM_audioFiles_DEATH_SFX], ENUM_audioChannels_COLLISION_SOUND_EFFECT);

        int inAnimation = 1;
        int animFramesLeft = 60 * 7;
        int initialFramecount = animFramesLeft;

        int wastedDelay = 120;
        int textFramesLeft = 255;

        int switchRepeatsLeft = 20;
        int colorStep = 0;
        int bgColorR = 0;
        int bgColorG = 0;
        int bgColorB = 0;
        int ballColorR = 0;
        int ballColorG = 0;
        int ballColorB = 0;

        int forceQuitDelay = 120;
        int forceQuitted = 0;


        while (inAnimation) {

            clear();

            if (switchRepeatsLeft > 0) {
                if (colorStep == 0) {

                    bgColorR = 255;
                    bgColorG = 255;
                    bgColorB = 255;
                    ballColorR = 0;
                    ballColorG = 0;
                    ballColorB = 0;

                    colorStep = 1;
                } else {

                    bgColorR = 0;
                    bgColorG = 0;
                    bgColorB = 0;
                    ballColorR = 255;
                    ballColorG = 255;
                    ballColorB = 255;

                    colorStep = 0;
                }
                switchRepeatsLeft -= 1;
            }

            changeColor(bgColorR, bgColorG, bgColorB);
            drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            changeColor(ballColorR, ballColorG, ballColorB);
            drawRect(ball.x, ball.y, ball.size, ball.size);

            if (initialFramecount - animFramesLeft > forceQuitDelay) {
                _text_changeColor(0, 0, 255, 255);
                _text_drawText("Press any key to skip.", 0, 0, gameFont_24);
            }


            if (animFramesLeft < (initialFramecount - wastedDelay) && textFramesLeft > 0) {
                _text_changeColor(255, 0, 0, 255 - textFramesLeft + 1);
                _text_drawText("WASTED", WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.2, gameFont_256);
                textFramesLeft -= 1;
            } else if (textFramesLeft <= 0) {
                _text_changeColor(255, 0, 0, 255);
                _text_drawText("WASTED", WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.2, gameFont_256);
            }

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        inAnimation = 0;
                        programLaunched = 0;
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_q:
                                keyDownQ = 1;
                                break;
                            case SDLK_d:
                                keyDownD = 1;
                                break;
                        }
                        if (initialFramecount - animFramesLeft > forceQuitDelay) {
                            inAnimation = 0;
                            forceQuitted = 1;
                        }
                        break;
                    case SDL_KEYUP:
                        switch (event.key.keysym.sym) {
                            case SDLK_q:
                                keyDownQ = 0;
                                break;
                            case SDLK_d:
                                keyDownD = 0;
                                break;
                        }
                    default:
                        break;
                }
            }

            if (forceQuitted) {
                ball.x = 100;
                ball.y = 100;
                float cashLeft = (float)_data_cash * (1 - (((float)maxLossPreventionLevelValue - (float)_data_lossPreventionLevel) / 100.0));
                _data_cash = cashLeft;
                return;
            }

            actualize();
            usleep(1000000/FPS);

            animFramesLeft -= 1;

            if (animFramesLeft <= 0) {
                inAnimation = 0;
            }
        }

        ball.x = 100;
        ball.y = 100;

        _text_changeColor(0, 255, 0, 255);

        float cashLeft = (float)_data_cash * (1 - (((float)maxLossPreventionLevelValue - (float)_data_lossPreventionLevel) / 100.0));

        char cashLostText[300];
        sprintf(cashLostText, "You also lost %d%% of your cash. ($%d)", maxLossPreventionLevelValue - _data_lossPreventionLevel, _data_cash - (int)cashLeft);
        _text_drawText(cashLostText, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.8, gameFont_36);

        _data_cash = cashLeft;

        actualize();
        usleep(2000000);
    }
}
