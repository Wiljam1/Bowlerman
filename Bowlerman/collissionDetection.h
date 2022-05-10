#ifndef collissionDetection_h
#define collissionDetection_h
#include <SDL2/SDL.h>
#define PUBLIC /* empty */
#define PRIVATE static

void collisionDetect(Game theGame);

void testCollosionWithBombs(Game theGame);
void testCollosionWithExplosion(Game theGame);
void playerStandingOnBomb(Game theGame);
void testCollisionWithWalls(Game theGame);
int testCollisionExplosionWithWalls(Game theGame, int k);
int testCollisionWithDestroyableWalls(Game theGame, int k, int j);
int testPossibilityToExplode(Game theGame, int playerID, int i);
int testPossibilityToExplodeDestroyableWalls(Game theGame, int playerID, int i);
//void testPossibilityToExplodeWithBombs(Game theGame);
void playerCollisionWithPowerup(Game theGame);

#endif