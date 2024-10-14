#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "brick_patterns.h"
#include "collisions_handler.h"
#include "audio_functions.h"
#include "playerData.h"
#include "cash_handler.h"



void checkCollisionsWithBricks() {
    for (int i = 0; i < NB_BRICK_WIDTH; i++) {
        for (int j = 0; j < NB_BRICK_HEIGHT; j++) {

            // If already destroyed then skip this check
            if (brickMap[i][j].x == -1 && brickMap[i][j].y == -1) {
                continue;
            }

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
                _cash_income(1);

                return;
            }
        }
    }
}

void checkBallCollisionWithBar() {
    // Check if ball made contact with bar
    if ((ball.x + ball.size >= bar.x && ball.x <= bar.x + bar.sizeX) &&
        (ball.y + ball.size >= bar.y && ball.y <= bar.y + bar.sizeY)) {

        // Ball hits the top of the bar
        BALL_SPEED += 0.1;
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
