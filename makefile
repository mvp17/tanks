compile:
	g++ game.cpp -o game.o -lglut -lGLU -lGL -lm -ljpeg
all: 
	g++ game.cpp -o game.o -lglut -lGLU -lGL -lm -ljpeg
	./game.o
clean:
	rm game.o
mac:
	g++ -std=c++11 -o labyrinth game.cpp -framework OpenGL -framework GLUT -lm -ljpeg
	./labyrinth
cleanmac:
	rm labyrinth