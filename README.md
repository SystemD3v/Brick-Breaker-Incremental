REQUIREMENTS:
SDL2_mixer (sudo apt-get install libsdl2-mixer-dev)

PROGRAM COMPILE COMMAND:
gcc main.c constants.c function.c custom_functions.c brick_patterns.c collisions_handler.c -o "main.out" -lSDL2main -lSDL2 -lSDL2_mixer
