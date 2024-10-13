#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>


#include "constants.h"
#include "function.h"
#include "custom_functions.h"
#include "brick_patterns.h"
#include "collisions_handler.h"


int keyDownQ;
int keyDownD;


void init_game() {
    if (DVD_MODE) {
        ball.size = 50;
    } else {
        ball.size = 10;
    }
    ball.x = 100;
    ball.y = 100;
    ball.velocityX = BALL_SPEED;
    ball.velocityY = BALL_SPEED;

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
}


void updateBallPosition() {
    if (ball.x <= 0 || ball.x >= WINDOW_WIDTH - ball.size) {
        ball.velocityX = ball.velocityX * -1;
    } else if (ball.y <= 0 || ball.y >= WINDOW_HEIGHT - ball.size) {
        ball.velocityY = ball.velocityY * -1;
    }
    if (ball.velocityX > 0) {
        ball.velocityX = BALL_SPEED;
    } else {
        ball.velocityX = -BALL_SPEED;
    }
    if (ball.velocityY > 0) {
        ball.velocityY = BALL_SPEED;
    } else {
        ball.velocityY = -BALL_SPEED;
    }

    ball.x = ball.x + ball.velocityX;
    ball.y = ball.y + ball.velocityY;
}
void updateBarPosition() {
    bar.x = bar.x + bar.velocityX;
    drawRect(bar.x, bar.y, bar.sizeX, bar.sizeY);
}

void debugPrint() {
    printf("DVD MODE: %d\n", DVD_MODE);
    printf("%d, %d, %d, %d\n", ball.x, ball.y, ball.velocityX, ball.velocityY);
}

void checkKeydowns() {
    if (keyDownQ) {
        bar.x -= BALL_SPEED * BAR_SPEED_MULTIPLIER;
    }
    if (keyDownD) {
        bar.x += BALL_SPEED * BAR_SPEED_MULTIPLIER;
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
            freeAndTerminate();
            break;
        case SDLK_q:
            keyDownQ = 1;
            break;
        case SDLK_d:
            keyDownD = 1;
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

int onClick() {
    _audio_loadAndPlay("../assets/audios/beep_beep.wav", 3);
    return 0;
}

void joyButtonPressed(){
}

void gameLoop() {
    int programLaunched = 1;
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
                    onClick();
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
        /* coder ici pour que le code s'execute après chaque évenement
         * exemple dessiner un carré avec position int x, int y qu'on peut
         * deplacer lorsqu'on appuie sur une touche
         */
        drawGame();
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
    printf("Fin du programme\n");
    freeAndTerminate();
}
