windows: main.o application.o 
	gcc -o main main.o application.o -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
main.o:
	gcc -c main.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
application.o:
	gcc -c application.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
clean_windows:
	del /Q /F main.exe *.o


mac: main.o application.o 
	gcc -o main main.o application.o -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
main.o:
	gcc -c main.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
application.o:
	gcc -c application.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
clean_mac:
	rm *.o