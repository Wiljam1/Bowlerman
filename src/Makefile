all: main.o application.o 
	gcc -o main main.o application.o -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
main.o:
	gcc -c main.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
application.o:
	gcc -c application.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
clean:
	rm *.o