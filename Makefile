libs = -lglfw3 -lcglm -lSDL2 -lGLEW -lGL -lGLU -lm
game: main.o fileio.o shader.o
	g++ -o game main.o fileio.o shader.o $(libs)

main.o:
	g++ -c main.cpp -Wall

fileio.o:
	g++ -c fileio.cpp -Wall

shader.o:
	g++ -c shader.cpp -Wall

clean:
	rm *.o
