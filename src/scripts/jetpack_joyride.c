#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"
#include "function.h"
#include "text_handler.h"
#include "audio_functions.h"
#include "jetpack_joyride.h"

int spacebarPressed = 0;
int inEventinsider = 0;

int hit = 0;

void jjKeyPressed(int key) {
    switch (key) {
        case SDLK_ESCAPE:
            inEventinsider = 0;
            break;
        case SDLK_SPACE:
            spacebarPressed = 1;
            break;
        default:
            break;
    }
}
void jjKeyUnpressed(int key) {
    switch (key) {
        case SDLK_SPACE:
            spacebarPressed = 0;
            break;
        default:
            break;
    }
}
void jjonClick(int clickX, int clickY) {
    inEventinsider = 0;
}

int jjSpeed = 20;

int posBg1X = 0;
int sizeBgX = 3840;
int posBg2X = 3840;
int bgInFront = 1;


void drawBGs() {
    posBg1X -= jjSpeed;
    posBg2X -= jjSpeed;

    if (bgInFront == 1) {
        if (posBg1X + sizeBgX <= 0) {
            bgInFront = 2;
            posBg1X += sizeBgX * 2;

            sprite(posBg1X, 0, "../assets/imgs/jetpack_joyride/mainloop_bg.bmp");
            sprite(posBg2X, 0, "../assets/imgs/jetpack_joyride/mainloop_bg.bmp");
        } else {
            sprite(posBg1X, 0, "../assets/imgs/jetpack_joyride/mainloop_bg.bmp");
            sprite(posBg2X, 0, "../assets/imgs/jetpack_joyride/mainloop_bg.bmp");
        }
    } else {
        if (posBg2X + sizeBgX <= 0) {
            bgInFront = 1;
            posBg2X += sizeBgX * 2;

            sprite(posBg1X, 0, "../assets/imgs/jetpack_joyride/mainloop_bg.bmp");
            sprite(posBg2X, 0, "../assets/imgs/jetpack_joyride/mainloop_bg.bmp");
        } else {
            sprite(posBg1X, 0, "../assets/imgs/jetpack_joyride/mainloop_bg.bmp");
            sprite(posBg2X, 0, "../assets/imgs/jetpack_joyride/mainloop_bg.bmp");
        }
    }
}

int charPosX;
int charPosY;
int charSizeX = 126;
int charSizeY = 142;
int charVelX;
int charVelY;
int minPosY;
int maxPosY;
int maxVelocityY = 50;
int minVelocityY = 50;

int runAnimFrame = 0;
int flyAnimFrame = 0;
int state = 0; // 0 == walking, 1 == flying
void drawChar() {

    if (state == 0){
        flyAnimFrame = 0;
        if (runAnimFrame % 10 < 5) {
            sprite(WINDOW_WIDTH * 0.1, charPosY, "../assets/imgs/jetpack_joyride/barryRun1.bmp");
        } else {
            sprite(WINDOW_WIDTH * 0.1, charPosY, "../assets/imgs/jetpack_joyride/barryRun2.bmp");
        }
    } else if (state == 1) {
        runAnimFrame = 0;
    }

    flyAnimFrame++;
    runAnimFrame++;
}

void charMovement() {
    if (charPosY <= minPosY) {
        charVelY = 0;
    } else if (charPosY >= maxPosY) {
        charVelY = 0;
    }

    if (spacebarPressed) {
        charVelY -= 2;
    } else {
        charVelY += 2;
    }

    charPosY += charVelY;
    if (charPosY <= minPosY) {
        charPosY = minPosY;
    }
    if (charPosY >= maxPosY) {
        charPosY = maxPosY;
    }
}

int isLaserPresent = 0;
int laserX;
int laserY;
int laserSizeX;
int laserSizeY;
int laserEndSize = 128;
int laserEndOffset = 20;
int laserLaserSize = 20;
void drawLaser() {
    if (isLaserPresent) {
        laserX -= jjSpeed;
        if (laserX < 0 - laserSizeX) {
            isLaserPresent = 0;
        }

        sprite(laserX, laserY, "../assets/imgs/jetpack_joyride/laserTop.bmp");
        sprite(laserX, laserY + laserSizeY - laserEndSize, "../assets/imgs/jetpack_joyride/laserBottom.bmp");
        int laserLengthToDraw = laserSizeY + laserEndOffset - (2 * laserEndSize);
        for (int i = 0; i < laserLengthToDraw; i+=laserLaserSize) {
            sprite(laserX, laserY + laserEndSize + i - laserEndOffset, "../assets/imgs/jetpack_joyride/laserLaser.bmp");
        }
        return;
    }
    int laserChance = rand() % 100;
    if (!(laserChance <= 10)) {
        return;
    }

    laserY = rand() % WINDOW_HEIGHT * 0.6;
    laserSizeY = rand() % WINDOW_HEIGHT * 0.4 + WINDOW_HEIGHT * 0.2;
    laserX = (rand() % WINDOW_WIDTH * 0.5) + WINDOW_WIDTH;
    laserSizeX = WINDOW_WIDTH * 0.05;

    sprite(laserX, laserY, "../assets/imgs/jetpack_joyride/laserTop.bmp");
    sprite(laserX, laserY + laserSizeY - laserEndSize, "../assets/imgs/jetpack_joyride/laserBottom.bmp");
    int laserLengthToDraw = laserSizeY + laserEndOffset - (2 * laserEndSize);
    for (int i = 0; i < laserLengthToDraw; i+=laserLaserSize) {
        sprite(laserX, laserY + laserEndSize + i - laserEndOffset, "../assets/imgs/jetpack_joyride/laserLaser.bmp");
    }

    isLaserPresent = 1;

}

void collisionLaser() {
    if (charPosX + charSizeX > laserX &&
        charPosX < laserX + laserSizeX &&
        charPosY + charSizeY > laserY &&
        charPosY < laserY + laserSizeY)
    {
        hit = 1;
    }
}

int animationEnded = 0;
int animationDeathFrame = 0;
void charFallAnimation() {
    if (charVelY <= 0.5 && charPosY >= maxPosY && jjSpeed <= 0) {
        printf("inhere\n");
        animationEnded = 1;
        sprite(charPosX, charPosY, "../assets/imgs/jetpack_joyride/barryDeath.bmp");
        return;
    }
    if (charPosY <= minPosY) {
        charVelY = 1;
    }
    if (charPosY < maxPosY) {
        charVelY += 1;
        if (charVelY > maxVelocityY) {
            charVelY = maxVelocityY;
        }
    }
    if (charPosY >= maxPosY) {
        charVelY *= -0.75;
    }
    charPosY += charVelY;
    if (animationDeathFrame % 10 == 0) {
        jjSpeed--;
    }
    animationDeathFrame++;
    sprite(charPosX, charPosY, "../assets/imgs/jetpack_joyride/barryDeath.bmp");
}

int score = 0;
void drawScore() {
    score += jjSpeed;
    char scoreText[30];
    sprintf(scoreText, "%dcm", score);
    _text_changeColor(255, 0, 0, 255);
    _text_drawText(scoreText, WINDOW_WIDTH * 0.02, WINDOW_WIDTH * 0.02, gameFont_36);
}

void _event_JJloop() {

    posBg1X = 0;
    posBg2X = sizeBgX;

    minPosY = WINDOW_HEIGHT * 0.1;
    maxPosY = WINDOW_HEIGHT * 0.8;

    charPosX = WINDOW_WIDTH * 0.1;
    charPosY = maxPosY;
    charVelX = 0;
    charVelY = 0;

    jjSpeed = 20;
    int jjSpeedIncreaseFrame = 0;

    score = 0;

    hit = 0;
    animationEnded = 0;
    animationDeathFrame = 0;

    _audio_loadAndPlayLoop("../assets/audios/jetpack_joyride/jjThemeSong.wav", ENUM_audioChannels_BACKGROUND, -1);

    inEventinsider = 1;
    while (inEventinsider) {


        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    inEventinsider = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    jjonClick(event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    jjKeyPressed(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    jjKeyUnpressed(event.key.keysym.sym);
                    break;
                case SDL_JOYBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }

        clear();

        if (hit == 0) {
            drawBGs();
            drawLaser();
            charMovement();
            collisionLaser();
            drawChar();
            drawScore();
            jjSpeedIncreaseFrame++;
            if (jjSpeedIncreaseFrame % 60 == 0){
                jjSpeed++;
            }
        } else if (hit == 1) {
            drawBGs();
            drawScore();
            charFallAnimation();
            if (animationEnded) {
                _text_changeColor(255, 0, 0, 255);
                _text_drawText("YOU DIED!", WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1, gameFont_128);
                _text_drawText("Going back to the lobby in 3 seconds...", WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.7, gameFont_36);
                actualize();
                usleep(2000000);
                inEventinsider = 0;
            }
        }

        actualize();
        usleep(1000000/FPS);

    }

    _audio_stopOnChannel(ENUM_audioChannels_BACKGROUND);
}
