#ifndef collissionDetection_h
#define collissionDetection_h
#include <SDL2/SDL.h>
#define PUBLIC /* empty */
#define PRIVATE static
#include "game.h"

typedef struct playerController *Player;
typedef struct wall *Wall;
//void collisionDetect(Game theGame, Sounds *sounds);
//typedef struct playerController *Player;

void playerStandingOnBomb(Game theGame, Player p[]);                                            //kollar om spelare står på en bomb
//int testCollisionWithDestroyableWalls(Game theGame, Player p, int k, int j);                  //testar om explosioner träffar förstörbar vägg och förstör den */
int testCollisionWithDestroyableWalls(Game theGame, Player player[], int k, int j, Wall *wall);
int testCollisionExplosionWithWalls(Game theGame, int k, Wall *wall);                           //testar explosion med oförstörbara väggar så att ingen explosion hamnar fel
int testPossibilityToExplode(Game theGame, int playerID, int i, Wall *wall);                    //testar explosioner åt sidorna om man kan lägga en xplosion där
int testPossibilityToExplodeDestroyableWalls(Game theGame, int playerID, int i, Wall *wall);    //kollar collision mellan explosioner och förstörbara väggar
void testPossibilityToExplodeWithBombs(Game theGame, int j);                        //kollar collision med andra bomber
void collisionDetect(Game theGame, Sounds *s, Player p[], Wall *wall);


#endif