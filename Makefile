CC=gcc
CFLAGS=""

debug:clean
	$(CC) $(CFLAGS) main.c constants.c function.c audio_functions.c brick_patterns.c collisions_handler.c text_handler.c playerData.c cash_handler.c upgrade_menu.c infinity_menu.c  -o "game.out" -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lcjson -lm
stable:clean
	$(CC) $(CFLAGS) main.c constants.c function.c audio_functions.c brick_patterns.c collisions_handler.c text_handler.c playerData.c cash_handler.c upgrade_menu.c infinity_menu.c  -o "game.out" -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lcjson -lm
clean:
	rm -vfr *~ game
