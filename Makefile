CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -g -I header_files
CFLAGS += -fsanitize=address
SDL_INCLUDE = -I ./include
SDL_LIB = -L C:/SDL2/lib -lmingw32 -lSDL2main -lSDL2

SRC = $(wildcard source_files/*.c)
OBJ = game.exe

all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(SRC) $(SDL_INCLUDE) $(CFLAGS) $(SDL_LIB) -o $(OBJ)
	@echo "Build completo! Execute ./$(OBJ) para jogar."

clean:
	del $(OBJ)
