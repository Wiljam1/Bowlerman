windows: main.o status.o
	gcc -o main main.o status.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
main.o:
	gcc -c main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
status.o:
	gcc -c status.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
clean:
	del /Q /F main.exe *.o