CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -g -I header_files
SDL_INCLUDE = -I ./include
SDL_LIB = -L lib -lmingw32 -lSDL2main -lSDL2

SRC = $(wildcard source_files/*.c) main.c
OBJ = game.exe

all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(SRC) $(SDL_INCLUDE) $(CFLAGS) $(SDL_LIB) -o $(OBJ)
	@echo "Compilation sucessful, run ./$(OBJ) to play."

clean:
	del $(OBJ)