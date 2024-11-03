#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <jsmn.h> // JSON Handling
#include <math.h>


#include "constants.h"
#include "function.h"
#include "audio_functions.h"
#include "brick_patterns.h"
#include "collisions_handler.h"
#include "text_handler.h"
#include "playerData.h"
#include "cash_handler.h"
#include "upgrade_menu.h"
#include "infinity_menu.h"


void init_game() {
    _data_loadGame();
    _data_updateConstantsWithData();

    if (DVD_MODE) {
        ball.size = 50;
    } else {
        ball.size = 10;
    }
    ball.x = 100;
    ball.y = 100;
    ball.velocityX = _data_gameSpeed;
    ball.velocityY = _data_gameSpeed;

    bar.sizeX = WINDOW_WIDTH/20;
    bar.sizeY = WINDOW_HEIGHT/50;
    bar.x = WINDOW_WIDTH/2 - WINDOW_WIDTH/20/2;
    bar.y = 9 * WINDOW_HEIGHT/10;
    bar.velocityX = 0;


    _constants_initBrickMaximums();

    _pattern_allocateMemory();
    _pattern_initAll();
    _pattern_loadPattern(1);

    _audio_initializeMixer();

    _text_initializeTtfLibrary();
    _constants_loadFont();
}



void updateBallPosition() {
    if (ball.x <= 0 || ball.x >= WINDOW_WIDTH - ball.size) {
        ball.velocityX = ball.velocityX * -1;
    } else if (ball.y <= 0 || ball.y >= WINDOW_HEIGHT - ball.size) {
        ball.velocityY = ball.velocityY * -1;
    }
    if (ball.velocityX > 0) {
        ball.velocityX = _data_gameSpeed;
    } else {
        ball.velocityX = -_data_gameSpeed;
    }
    if (ball.velocityY > 0) {
        ball.velocityY = _data_gameSpeed;
    } else {
        ball.velocityY = -_data_gameSpeed;
    }

    ball.x = ball.x + ball.velocityX;
    ball.y = ball.y + ball.velocityY;
}

void updateBarPosition() {
    bar.x = bar.x + bar.velocityX;
    if (bar.x < 0) {
        bar.x = 0;
    } else if (bar.x + bar.sizeX > WINDOW_WIDTH) {
        bar.x = WINDOW_WIDTH - bar.sizeX;
    }
    drawRect(bar.x, bar.y, bar.sizeX, bar.sizeY);
}


void debugPrint() {
    printf("DVD MODE: %d\n", DVD_MODE);
    printf("%f, %f, %f, %f\n", ball.x, ball.y, ball.velocityX, ball.velocityY);
}


void checkKeydowns() {
    if (keyDownQ) {
        bar.x -= _data_gameSpeed * BAR_SPEED_MULTIPLIER;
    }
    if (keyDownD) {
        bar.x += _data_gameSpeed * BAR_SPEED_MULTIPLIER;
    }
}

void drawGame(){
    /* Ici je dessine mon jeu
     * exemple position x, y modifiés dans KeyPressed() et utilisés pour
     * pouvoir deplacer la figure à chaque boucle de gameLoop()
     */
    clear();

    // Barckground stuff here


    // Drawing bricks
    _pattern_displayBricks();

    changeColor(255, 255, 255);

    // Some extras &| debug
    if (DVD_MODE) {
        sprite(ball.x, ball.y, "../assets/imgs/_dvd_logo.bmp");
    } else {
        drawSquare(ball.x, ball.y, ball.size);
    }
    if (DEBUG_MODE) {
        debugPrint();
    }

    // Check keyDowns
    checkKeydowns();

    // Perform moves
    updateBarPosition();
    updateBallPosition();

    // Perform checks
    checkBallCollisionWithBar();
    checkCollisionsWithBricks();
    checkCollisionsWithBottom();

    // Draw texts
    _text_changeColor(0, 255, 0, 255);
    _cash_drawCash(4, WINDOW_HEIGHT - 40, gameFont_36);
    _text_changeColor(255, 0, 255, 255);
    _cash_drawInfinity(4, WINDOW_HEIGHT - 80, gameFont_36, 0, 1);
    _text_changeColor(0, 0, 255, 255);
    _text_drawText("Press A to open the shop.", WINDOW_WIDTH * 0.73, WINDOW_HEIGHT - 40, gameFont_36);
    if (_data_infinity > 0) {
        _text_changeColor(255, 0, 255, 255);
        _text_drawText("Press I to open the infinity tree.", WINDOW_WIDTH * 0.63, WINDOW_HEIGHT - 80, gameFont_36);
    }

    // And here we go again
    actualize();
    usleep(1000000 / FPS); // 60 images par seconde | 1000000 = 1 seconde
}


void KeyPressed(SDL_Keycode touche){
    /** @brief event.key.keysym.sym renvoi la touche appuyé
     *
     */
    switch (touche) {
        // Voir doc SDL_Keycode pour plus de touches https://wiki.libsdl.org/SDL_Keycode
        case SDLK_ESCAPE:
            programLaunched = 0;
            break;
        case SDLK_q:
            keyDownQ = 1;
            break;
        case SDLK_d:
            keyDownD = 1;
            break;
        case SDLK_a:
            switch (IN_UPGRADE_MENU) {
                case 0:
                    IN_UPGRADE_MENU = 1;
                    IN_INFINITY_MENU = 0;
                    break;
                case 1:
                    IN_UPGRADE_MENU = 0;
                    break;
            }
            break;
        case SDLK_i:
            if (_data_infinity <= 0) {
                break;
            }
            switch (IN_INFINITY_MENU) {
                case 0:
                    IN_INFINITY_MENU = 1;
                    IN_UPGRADE_MENU = 0;
                    break;
                case 1:
                    IN_INFINITY_MENU = 0;
                    break;
            }
            break;
        default:
            break;
    }
}

void KeyUnpressed(SDL_KeyCode key) {
    switch (key) {
        case SDLK_q:
            keyDownQ = 0;
            break;
        case SDLK_d:
            keyDownD = 0;
            break;
        default:
            break;
    }
}

void click(int mousePosX, int mousePosY) {

}

int onClick(int mousePosX, int mousePosY) {
    //_audio_loadAndPlay("../assets/audios/beep_beep.wav", 3);
    switch (IN_UPGRADE_MENU) {
        case 0:
            click(mousePosX, mousePosY);
            break;
        case 1:
            _upgradeMenu_clickHandler(mousePosX, mousePosY);
            break;
    }
    return 0;
}

void joyButtonPressed(){
}



void GAME_MASTERORDER() {
    if (IN_UPGRADE_MENU == 1) {
        _upgradeMenu_displayMenu();
    } else if (IN_INFINITY_MENU == 1) {
        _infinityMenu_displayMenu();
    } else {
        drawGame();
    }
}




void gameLoop() {
    while (programLaunched == 1) {
        // Boucle pour garder le programme ouvert
        // lorsque programLaunched est different de 1
        // on sort de la boucle, donc de la fonction
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // boucle pour la gestion d'évenement
            switch (event.type) {
                /** event.type renvoi le type d'evenement qui se passe
                 * (si on appuie sur une touche, clique de souris...)
                 * en fonction du type d'évènement on à alors
                 * différentes information accessibles
                 * voir doc pour plus d'event https://wiki.libsdl.org/SDL_EventType
                 */
                case SDL_QUIT:
                    // quand on clique sur fermer la fénêtre en haut à droite
                    programLaunched = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    /* clique de la souris
                     * event.motion.y | event.motion.x pour les positions de la souris
                     */
                    printf("position de la souris x : %d , y : %d\n", event.motion.x, event.motion.y);
                    onClick(event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    KeyUnpressed(event.key.keysym.sym);
                    break;
                case SDL_JOYBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }
        GAME_MASTERORDER();
    }
}

int main(){
    /** @description 3 fonctions dans le main qui permettent de créer l'application et la maintenir ouverte :
     *  init(...) : initialiser la SDL/ fenêtre
     *  gameLoop() : boucle de jeu dans laquelle l'application reste ouverte
     *  freeAndTerminate() : quitte le programme proprement
     */

    init(WINDOW_WIDTH, WINDOW_HEIGHT);
    init_game();
    gameLoop();

    _data_saveGame();

    printf("Fin du programme\n");
    freeAndTerminate();
}
