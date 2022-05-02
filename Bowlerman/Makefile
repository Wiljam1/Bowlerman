windows: main.o game.o player.o sorter.o bomb.o wall.o collissionDetection.o
	gcc -o main main.o game.o player.o sorter.o bomb.o wall.o collissionDetection.o -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
main.o:
	gcc -c main.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
game.o:
	gcc -c game.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
player.o:
	gcc -c player.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
sorter.o:
	gcc -c sorter.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
bomb.o:
	gcc -c bomb.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
collissionDetection.o:
	gcc -c collissionDetection.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
wall.o:
	gcc -c wall.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
clean_windows:
	del /Q /F main.exe UDPserver.exe *.o

UDPserver:
	gcc -o UDPserver UDPserver.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net


	
mac: main.o game.o player.o sorter.o bomb.o wall.o collissionDetection.o
	gcc -o main main.o game.o player.o sorter.o bomb.o wall.o collissionDetection.o -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
main.o:
	gcc -c main.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
game.o:
	gcc -c game.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
player.o:
	gcc -c player.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
sorter.o:
	gcc -c sorter.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
bomb.o:
	gcc -c bomb.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
wall.o:
	gcc -c wall.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
collissionDetection.o:
	gcc -c collissionDetection.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image
clean_mac:
	rm *.o

UDPserver_mac:
	gcc -o UDPserver UDPserver.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net