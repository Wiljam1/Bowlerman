#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "application.h"
//#include "ball.h"

#define PUBLIC /* empty */
#define PRIVATE static

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

struct Application_type{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Renderer *renderer;
    SDL_Event    window_event;
};

PRIVATE void update(Application theApp, double delta_time);
PRIVATE void draw(Application theApp);

PUBLIC Application createApplication(){
    Application s = malloc(sizeof(struct Application_type));
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
    }

    s->window= SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    s->renderer = SDL_CreateRenderer(s->window, -1, 0);

    s->window_surface = SDL_GetWindowSurface(s->window);

    return s;

}

PUBLIC void applicationUpdate(Application theApp){
    bool keep_window_open = true;
    while(keep_window_open)
    {
        while(SDL_PollEvent(&theApp->window_event) > 0)
        {
            SDL_Event test = theApp->window_event;
            
            switch(theApp->window_event.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
            }
        }
        
        //update(theApp, 10.0/60.0);
        draw(theApp);
    }
}

PRIVATE void update(Application theApp, double delta_time){
    //ballUpdate(theApp->ballTwo, delta_time);
}

PRIVATE void draw(Application theApp)
{
    SDL_SetRenderDrawColor(theApp->renderer, 0x0, 0x0, 0x0, 0xFF);
	SDL_RenderClear(theApp->renderer);

    SDL_SetRenderDrawColor(theApp->renderer, 0xFF, 0xFF, 0xFF, 0xFF);


    for (int y = 0; y < WINDOW_HEIGHT; ++y)
    {
	    if (y % 5)
	    {
		    SDL_RenderDrawPoint(theApp->renderer, WINDOW_WIDTH / 2, y);
	    }
    }

			//
			// Rendering will happen here
			//

			// Present the backbuffer
	SDL_RenderPresent(theApp->renderer);
}

PUBLIC void destoryApplication(Application theApp){
    SDL_DestroyRenderer(theApp->renderer);
    SDL_FreeSurface(theApp->window_surface);
    SDL_DestroyWindow(theApp->window);
    SDL_Quit();
}

