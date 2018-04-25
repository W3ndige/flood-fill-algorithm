OBJS = src/main.cpp
CC = g++
COMPILER_FLAGS = -Wall -Wextra
LINKER_FLAGS = -lSDL2
OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
