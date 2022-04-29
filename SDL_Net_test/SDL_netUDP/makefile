soC: udps.c udpc.c
	gcc -o server udps.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image
	gcc -o client udpc.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image

udp0: udpsMirror.c
	gcc -o udpsMirror udpsMirror.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image
udp1: udpsMirrorWithStruct.c
	gcc -o udpsMirrorWithStruct udpsMirrorWithStruct.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image

udp2: udpsMirrorWithStruct2.c
	gcc -o udpsMirrorWithStruct2 udpsMirrorWithStruct2.c -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image


clean:
	del /Q /F server.exe *.o
	del /Q /F client.exe *.o