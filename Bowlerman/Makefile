windows: main.o game.o player.o bomb.o wall.o collissionDetection.o textures.o network.o powerup.o sounds.o
	gcc -o main main.o game.o player.o bomb.o wall.o collissionDetection.o textures.o network.o powerup.o sounds.o -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_mixer
main.o:
	gcc -c main.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
game.o:
	gcc -c game.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
player.o:
	gcc -c player.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
bomb.o:
	gcc -c bomb.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
collissionDetection.o:
	gcc -c collissionDetection.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
wall.o:
	gcc -c wall.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
textures.o:
	gcc -c textures.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
network.o:
	gcc -c network.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
powerup.o:
	gcc -c powerup.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
sounds.o:
	gcc -c sounds.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer
clean_windows:
	del /Q /F main.exe UDPserver.exe *.o

UDPserver:
	gcc -o UDPserver UDPserver.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net

	
