#ifndef collissionDetection_h
#define collissionDetection_h
#include <SDL2/SDL.h>
#include "game.h"
#include "player.h"

//void collisionDetect(Game theGame, Sounds *sounds);
//typedef struct playerController *Player;

PUBLIC void collisionDetect(Game theGame, Sounds s, Player p[]);
void playerStandingOnBomb(Game theGame, Player p[]);                                            //kollar om spelare står på en bomb
//int testCollisionWithDestroyableWalls(Game theGame, Player p, int k, int j);                  //testar om explosioner träffar förstörbar vägg och förstör den */
int testCollisionWithDestroyableWalls(Game theGame, Player player[], int k, int j);
int testCollisionExplosionWithWalls(Game theGame, int k);                           //testar explosion med oförstörbara väggar så att ingen explosion hamnar fel
int testPossibilityToExplode(Game theGame, int playerID, int i);                    //testar explosioner åt sidorna om man kan lägga en xplosion där
int testPossibilityToExplodeDestroyableWalls(Game theGame, int playerID, int i);    //kollar collision mellan explosioner och förstörbara väggar
void testPossibilityToExplodeWithBombs(Game theGame, int j);                        //kollar collision med andra bomber



#endif