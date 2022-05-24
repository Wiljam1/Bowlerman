windows: main.o game.o player.o bomb.o wall.o collisionDetection.o textures.o udpClient.o powerup.o gui.o sounds.o tcpClient.o menu.o
	gcc -o main main.o game.o player.o bomb.o wall.o collisionDetection.o textures.o udpClient.o powerup.o gui.o sounds.o tcpClient.o menu.o -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_mixer -lSDL2_ttf
main.o:
	gcc -c main.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL_ttf
game.o:
	gcc -c game.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL_ttf
player.o:
	gcc -c player.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
bomb.o:
	gcc -c bomb.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
collisionDetection.o:
	gcc -c collisionDetection.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
wall.o:
	gcc -c wall.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
textures.o:
	gcc -c textures.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL_ttf
udpClient.o:
	gcc -c udpClient.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
tcpClient.o:
	gcc -c tcpClient.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
powerup.o:
	gcc -c powerup.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
gui.o:
	gcc -c gui.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL_ttf
sounds.o:
	gcc -c sounds.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer
menu.o:
	gcc -c menu.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2
clean_windows:
	del /Q /F *.exe *.o

alludpServer: udpServer.o udpServerWithTCPclient.o
	gcc -o udpServer udpServer.o udpServerWithTCPclient.o -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
udpServer.o:
	gcc -c udpServer.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
udpServerWithTCPclient.o:
	gcc -c udpServerWithTCPclient.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net


tcpServer:
	gcc -o tcpServer tcpServer.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
	
