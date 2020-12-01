libs = -lglfw3 -lcglm -lSDL2 -lGLEW -lGL -lGLU -lm
srcs = main.cpp fileio.cpp shader.cpp
game: $(srcs)
	g++ -o game $(srcs) $(libs) -Wall -g
