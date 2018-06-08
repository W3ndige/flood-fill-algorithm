OBJS = src/main.cpp src/menu.cpp src/drawing.cpp src/gadgets.cpp src/files.cpp
CC = g++
# -isystem will surpress warnings from CImg, -O3 will produce code with more optimization
COMPILER_FLAGS = -Wall -Wextra -O3 -isystem $(INCLUDE)
INCLUDE = CImg
LINKER_FLAGS = -lSDL2 -lX11 -lpthread
OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(LIBS) -o $(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
