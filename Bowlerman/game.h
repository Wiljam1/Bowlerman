#ifndef game_h
#define game_h

typedef struct game_type *Game;
Game createGame();              // Struct for app initialization
void gameUpdate(Game newGame);
void destoryGame(Game theGame); // Function for easily destroying the application.
#endif