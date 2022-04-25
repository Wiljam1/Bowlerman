windows: main.o status.o
	gcc -o main main.o status.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
main.o:
	gcc -c main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
status.o:
	gcc -c status.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
clean:
	del /Q /F main.exe *.o