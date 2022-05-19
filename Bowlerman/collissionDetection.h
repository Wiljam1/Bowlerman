#ifndef collissionDetection_h
#define collissionDetection_h
#include <SDL2/SDL.h>
#define PUBLIC /* empty */
#define PRIVATE static
#include "game.h"

//void collisionDetect(Game theGame, Sounds *sounds);

void playerStandingOnBomb(Game theGame);                                            //kollar om spelare står på en bomb

int testCollisionExplosionWithWalls(Game theGame, int k);                           //testar explosion med oförstörbara väggar så att ingen explosion hamnar fel
int testCollisionWithDestroyableWalls(Game theGame, int k, int j);                  //testar om explosioner träffar förstörbar vägg och förstör den
int testPossibilityToExplode(Game theGame, int playerID, int i);                    //testar explosioner åt sidorna om man kan lägga en xplosion där
int testPossibilityToExplodeDestroyableWalls(Game theGame, int playerID, int i);    //kollar collision mellan explosioner och förstörbara väggar
void testPossibilityToExplodeWithBombs(Game theGame, int j);                        //kollar collision med andra bomber



#endif