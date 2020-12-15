libs = -lglfw3 -lGLEW -lGL -lGLU  -ldl -lpthread -lX11 -lm
all: test game
test: test.cpp fileio.o
	g++ -o test test.cpp fileio.o -I./include -Wall
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
