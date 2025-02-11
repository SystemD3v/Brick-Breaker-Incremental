# Linux Debian/Ubuntu
## Requirements:
- SDL2 (sudo apt-get install libsdl2-dev)
- SDL2_mixer (sudo apt-get install libsdl2-mixer-dev)
- SDL2_ttf (sudo apt-get install libsdl2-ttf-dev)
- cJSON (sudo apt-get install libcjson-dev)
- jsmn (sudo apt-get install libjsmn-dev)

## How to compile and run:
- cd ./src/scripts/
- gcc main.c constants.c function.c audio_functions.c brick_patterns.c collisions_handler.c text_handler.c playerData.c cash_handler.c upgrade_menu.c infinity_menu.c jetpack_joyride.c shaders.c -o "main.out" -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lcjson -lm -lGLEW -lGL
- ./main.out
