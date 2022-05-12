#ifndef collissionDetection_h
#define collissionDetection_h
#include <SDL2/SDL.h>
#define PUBLIC /* empty */
#define PRIVATE static

void collisionDetect(Game theGame, Sounds *sounds);



void playerStandingOnBomb(Game theGame);

int testCollisionExplosionWithWalls(Game theGame, int k);
int testCollisionWithDestroyableWalls(Game theGame, int k, int j);
int testPossibilityToExplode(Game theGame, int playerID, int i);
int testPossibilityToExplodeDestroyableWalls(Game theGame, int playerID, int i);
void testPossibilityToExplodeWithBombs(Game theGame, int j);



#endif