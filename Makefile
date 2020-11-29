libs = -lglfw3 -lcglm -lSDL2 -lGLEW -lGL -lGLU -lm
include = /usr/include/SDL2
srcs = main.cpp fileio.cpp shader.cpp
game: $(srcs)
	g++ -o game $(srcs) $(libs) -Wall -g
