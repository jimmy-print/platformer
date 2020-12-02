libs = -lglfw3 -lcglm -lSDL2 -lGLEW -lGL -lGLU -lm
game: main.o fileio.o shader.o rect.o
	g++ -o game main.o fileio.o shader.o rect.o $(libs)

main.o: src/main.cpp
	g++ -c src/main.cpp -I./include -Wall

fileio.o: src/fileio.cpp
	g++ -c src/fileio.cpp -Wall

shader.o: src/shader.cpp
	g++ -c src/shader.cpp -Wall

rect.o: src/rect.cpp
	g++ -c src/rect.cpp -Wall

clean:
	rm *.o
