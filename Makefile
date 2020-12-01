libs = -lglfw3 -lcglm -lSDL2 -lGLEW -lGL -lGLU -lm
game: main.o fileio.o shader.o
	g++ -o game main.o fileio.o shader.o $(libs)

main.o: main.cpp
	g++ -c main.cpp -Wall

fileio.o: fileio.cpp
	g++ -c fileio.cpp -Wall

shader.o: shader.cpp
	g++ -c shader.cpp -Wall

clean:
	rm *.o
