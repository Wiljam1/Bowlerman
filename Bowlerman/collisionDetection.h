#ifndef collissionDetection_h
#define collissionDetection_h
#include <SDL2/SDL.h>
#include "game.h"
#include "player.h"



//all collision detections in the game loop
PUBLIC void collisionDetect(Game theGame, Sounds s, Player p[]);                                            
//collision detection between explosion and destroyable wall
int testCollisionWithDestroyableWalls(Game theGame, Player player[], int k, int j);
//collision detection between explosion and walls so the explosion doesnt get placed wrong
int testCollisionExplosionWithWalls(Game theGame, int k);              
//Collision detection with undestroyable walls and if the explosion can be placed there             
int testPossibilityToExplode(Game theGame, int playerID, int i);  
//Collision detection with destroyable walls and if the explosion can be placed there                  
int testPossibilityToExplodeDestroyableWalls(Game theGame, int playerID, int i); 
//collision detection with explosions and bombs if it should create a chain reaction of explosions 
void testPossibilityToExplodeWithBombs(Game theGame, int j);                        



#endif