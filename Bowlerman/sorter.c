 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <stdbool.h>
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>

 #include "game.h"
 #include "player.h"
 #include "sorter.h"
 //bubble sorter:
 void arraySorter(Game theGame, int length, int *v)
 {
     //reset array
     int b[10];
     for(int i=0; i<length;  i++)
     {
        b[i]=theGame->player[i].yPos;
        v[i]=i;
     }
     //sort array
     int j,tmp;
     for(int i=0;i<length -1;i++)
     {
         for(j=0;j<length -1-i;j++)
         {
             if(b[j] > b[j+1])
             {
                 tmp=b[j];
                 b[j]=b[j+1];
                 b[j+1]=tmp;
              
                 tmp=v[j];
                 v[j]=v[j+1];
                 v[j+1]=tmp;
             }
         }
     }
 } 

