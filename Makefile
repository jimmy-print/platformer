libs = -lglfw3 -lSDL2 -lGLEW -lGL -lX11 -ldl -lpthread
include = /usr/include/SDL2
srcs = main.c fileio.c
game: $(srcs)
	gcc -o game $(srcs) $(libs) -Wall
