CC= gcc
FLAGS= -g -Wall -std=c99 -pedantic
LIBS= -lm -lSDL2 -lSDL2_image -lSDL2_ttf
SRC= src/*.c src/Engine/*.c
EXEC= game

make:
	$(CC) $(FLAGS) $(LIBS) $(SRC) -o $(EXEC)
