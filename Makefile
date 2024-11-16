# Compiler and flags
CC = gcc
CFLAGS = 
LIBS = -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lcjson -lm

# Source and object files
SRC_DIR = src/scripts
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/constants.c $(SRC_DIR)/function.c $(SRC_DIR)/audio_functions.c $(SRC_DIR)/brick_patterns.c $(SRC_DIR)/collisions_handler.c $(SRC_DIR)/text_handler.c $(SRC_DIR)/playerData.c $(SRC_DIR)/cash_handler.c $(SRC_DIR)/upgrade_menu.c $(SRC_DIR)/infinity_menu.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Output executable
OUT = main.out

# Targets
all: $(OUT)

$(OUT): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(OUT) $(LIBS)

# Compile .c files to .o object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ_FILES) $(OUT)

# Rebuild the project
rebuild: clean all

