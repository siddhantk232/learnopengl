INCLUDE=$(shell pkg-config --libs glfw3 gl)
LINK=$(shell pkg-config --cflags glfw3 gl)

main: glad.o ./src/main.cc
	g++ $(INCLUDE) $(LINK) -I./include/ glad.o src/main.cc -o main

glad.o: ./src/glad.c
	gcc -c $(INCLUDE) $(LINK) -I./include/ src/glad.c -o glad.o
