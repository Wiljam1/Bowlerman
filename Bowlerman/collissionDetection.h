#ifndef collissionDetection_h
#define collissionDetection_h
#include <SDL2/SDL.h>
#define PUBLIC /* empty */
#define PRIVATE static

void collisionDetect(Game theGame);

void testCollosionWithBombs(Game theGame);
void testCollosionWithExplosion(Game theGame);
void playerStandingOnBomb(Game theGame);

#endif