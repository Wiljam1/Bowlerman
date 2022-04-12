#ifndef game_h
#define game_h

typedef struct game_type *Game;
Game createGame();              // Struct for app initialization
int loadMedia(Game newGame, char fileLocation[]); // Load any image you want in the resources/ folder!
void gameUpdate(Game newGame);
void destroyGame(Game theGame); // Function for easily destroying the application.

#endif