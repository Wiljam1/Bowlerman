windows: main.o game.o player.o sorter.o bomb.o
	gcc -o main main.o game.o player.o sorter.o bomb.o -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
main.o:
	gcc -c main.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
game.o:
	gcc -c game.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
player.o:
	gcc -c player.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
sorter.o:
	gcc -c sorter.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
bomb.o:
	gcc -c bomb.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
clean_windows:
	del /Q /F main.exe *.o

	
mac: main.o game.o player.o sorter.o bomb.o
	gcc -o main main.o game.o player.o sorter.o bomb.o -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
main.o:
	gcc -c main.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
game.o:
	gcc -c game.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
player.o:
	gcc -c player.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
sorter.o:
	gcc -c sorter.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
bomb.o:
	gcc -c bomb.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
clean_mac:
	rm *.o