#include "main.h"
#include "status.h"

int main(int argc, char *argv[])
{
    //Init
    SDL_Init(SDL_INIT_EVERYTHING);
    srand((int)time(NULL));
    TTF_Init(); //Init font system
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    GameState game;
    SDL_Window *window = SDL_CreateWindow("Game Window",            // Window title
                                        SDL_WINDOWPOS_UNDEFINED,        // x position
                                        SDL_WINDOWPOS_UNDEFINED,        // y pos
                                        WIDTH,                          // width in pixels
                                        HEIGHT,                         // height in pixels
                                        0);                             // flags
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    game.renderer = renderer;
    loadGame(&game);
    //----------------------------------------------------------------------------------
    //Event/GAME loop
    bool done = false;
    while(!done)
    {
        //Check for events
        done = processEvents(window, &game);

        process(&game);
        collisionDetect(&game);

        //Render display
        doRender(renderer, &game);

        //Frames
        SDL_Delay(10);
    }
    //----------------------------------------------------------------------------------
    //Clean up
    closeGame(&game, window);

    return EXIT_SUCCESS;
}

void closeGame(GameState *game, SDL_Window *window)
{
    //Shutdown game and unload all memory
    SDL_DestroyTexture(game->wall);
    SDL_DestroyTexture(game->manFrames[0]);
    SDL_DestroyTexture(game->manFrames[1]);
    SDL_DestroyTexture(game->manFrames[2]);
    SDL_DestroyTexture(game->manFrames[3]);
    if(game->label != NULL)
        TTF_CloseFont(game->font);

    //Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(game->renderer);

    //Mix_FreeMusic(&gameState.bgMusic);
    //Mix_FreeChunk(&gameState.dieSound);

    //Clean up
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}

void process(GameState *game) //Handle game logic (bomb explode??)
{
    //add time
    game->time++;

    if(game->time > 120){
        shutdownStatusLives(game);
        game->statusState = STATUS_STATE_GAME;
    }

    //Only do stuff here if game is on
    if(game->statusState == STATUS_STATE_GAME && !game->man.isDead){
        
    }
}

int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
    return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void collisionDetect(GameState *game)
{

    for(int i = 0; i < WALLCOUNT/4; i++){
        if(collide2d(game->man.x, game->man.y, game->bricks[i].x, game->bricks[i].y, MANSIZE, MANSIZE, WALLSIZE, WALLSIZE)){
            game->man.isDead = true;
            //Mix_HaltChannel(game->musicChannel);
            break;
        }
    }
    // //Check for collision with any walls or if went too far left or right (Lazyfoo Lesson 27)
    // for(int i = 0; i < WALLCOUNT; i++){
    //     if( (game->man.x < 0) || (game->man.x + MANSIZE > WIDTH) || (checkCollision(game->man.collisionBox, game->walls[i].collisionBox)) ){
    //         //Move back
    //         game->man.x -= SPEED;
    //         game->man.collisionBox.x = game->man.x;
    //     }
    // }

    //Check for collision with any walls
    for(int i = 0; i < WALLCOUNT; i++)
    {
        float mW = MANSIZE, mH = MANSIZE;
        float mX = game->man.x, mY = game->man.y;
        float wX = game->walls[i].x, wY = game->walls[i].y, 
              wW = game->walls[i].w, wH = game->walls[i].h;

        if(mY+mH > wY && mY < wY+wH){
            //Rubbing against right edge
            if(mX < wX+wW && mX+mW > wX+wW){
                //Correct x
                //game->man.x = wX+wW;
                if(game->man.isMovingDiagonal == true){
                    game->man.x +=DIAGSPEED;
                    game->man.y +=DIAGSPEED;
                }
                else
                    game->man.x +=SPEED;
                mX = wX+wW;
                printf("Right Edge\n");
            }
            //Rubbing against left edge
            else if(mX+mW > wX && mX < wX){
                //Correct x
                //game->man.x = wX-mW;
                if(game->man.isMovingDiagonal == true){
                    game->man.x -=DIAGSPEED;
                    game->man.y -=DIAGSPEED;
                }
                else
                    game->man.x -=SPEED;
                mX = wX-mW;
                printf("Left Edge\n");
            }
        }

        if(mX+mW > wX && mX < wX+wW){
            //Are we bumping our head?
            if(mY < wY+wH && mY > wY){
                //correct y
                //game->man.y = wY+wH;
                if(game->man.isMovingDiagonal == true){
                    game->man.x +=DIAGSPEED;
                    game->man.y +=DIAGSPEED;
                }
                else
                    game->man.y +=SPEED;
                printf("Bumping head\n");
            }
            //Are we standing on the wall?
            else if(mY+mH > wY && mY < wY){
                //correct y
                //game->man.y = wY-mH;
                if(game->man.isMovingDiagonal == true){
                    game->man.x -=DIAGSPEED;
                    game->man.y -=DIAGSPEED;
                }
                else
                    game->man.y -=SPEED;
                printf("Standing on wall\n");
            }
        }
    }
}

SDL_Texture *loadImage(GameState *game, SDL_Surface *surface, char imgLocation[STRSIZE])
{
    //Load images and create rendering textures from them
    surface = IMG_Load(imgLocation);
    if(surface == NULL){
        printf("Error loading surface image!\n SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    return SDL_CreateTextureFromSurface(game->renderer, surface);
}

void loadGame(GameState *game)
{
    SDL_Surface *surface = NULL;
    // //Load images and create rendering textures from them
    // surface = IMG_Load("images/wall.png");
    // if(surface == NULL){
    //     printf("Error loading surface image!\n");
    //     SDL_Quit();
    //     exit(1);
    // }
    // game->wall = SDL_CreateTextureFromSurface(game->renderer, surface);
    // SDL_FreeSurface(surface);
    game->wall = loadImage(game, surface,"images/wall.png");
    game->alley = loadImage(game, surface, "images/alley.png");
    game->blood = loadImage(game, surface, "images/blood.png");
    game->brick = loadImage(game, surface, "images/brick.png");
    game->pin = loadImage(game, surface, "images/pin.png");
    game->manFrames[0] = loadImage(game, surface, "images/man-stand.png");
    game->manFrames[1] = loadImage(game, surface, "images/man-run1.png");
    game->manFrames[2] = loadImage(game, surface, "images/man-run2.png");
    game->manFrames[3] = loadImage(game, surface, "images/man-run3.png");
    SDL_FreeSurface(surface);

    game->font = TTF_OpenFont("fonts/Restoe Iboe.ttf", 48);
    if(!game->font){
        printf("Error loading font!\n");
        SDL_Quit();
        exit(1);
    }

    //Load struct start values
    game->label = NULL;
    game->man.isDead = false;
    game->man.x = 500;
    game->man.y = 300;
    game->man.isMoving = false;
    game->man.isMovingLeft = false;
    game->man.isMovingRight = false;
    game->statusState = STATUS_STATE_INTRO;

    initStatusLives(game);

    //Init walls
    for (int i = 0; i < WALLCOUNT; i++){
        game->walls[i].w = WALLSIZE;
        game->walls[i].h = WALLSIZE;

        if(i < 20){
            game->walls[i].x = i*WALLSIZE;
            game->walls[i].y = 0;
        }
        else if(i < 40){
            game->walls[i].x = i*WALLSIZE-WIDTH;
            game->walls[i].y = HEIGHT-WALLSIZE;
        }
        else if(i < 60){
            game->walls[i].x = 0;
            game->walls[i].y = i*WALLSIZE-HEIGHT*4;
        }
        else if(i < 80){
            game->walls[i].x = WIDTH-WALLSIZE;
            game->walls[i].y = i*WALLSIZE-HEIGHT*6;
        }
        else{
            game->walls[i].x = rand() % WIDTH - 128;
            game->walls[i].y = rand() % HEIGHT - 128;
        }
    }
    //Init pins
    for(int i = 0; i < WALLCOUNT/10; i++){
        game->bricks[i].w = WALLSIZE;
        game->bricks[i].h = WALLSIZE;
        game->bricks[i].x = rand() % WIDTH - 128;
        game->bricks[i].y = rand() % HEIGHT - 128;
    }
    
}

bool processEvents(SDL_Window *window, GameState *game)
{
    // The window is open: enter game loop (SDL_PollEvent)
    bool done = false;

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT_CLOSE:
                if(window){
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        done = 1;
                    break;
                    case SDLK_r:                          //RESURRECT
                        game->man.isDead = false;
                    break;
                }
            break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                    case SDLK_a: case SDLK_d: case SDLK_w: case SDLK_s:
                        game->man.isMoving = false;
                    break;
                }
            break;
            case SDL_QUIT:
                //Quit out of the game
                done = 1;
            break;
        }
    }

    //OLD MOVEMENT UNDER HERE
    // const Uint8 *state = SDL_GetKeyboardState(NULL);
    // if(state[SDL_SCANCODE_LEFT] && game->man.canMoveLeft == true){
    //     game->man.x -= SPEED;
    //     game->man.isMoving = true;
    //     game->man.isMovingLeft = true;
    //     game->man.isMovingRight = false;
    // }
    // if(state[SDL_SCANCODE_RIGHT] && game->man.canMoveRight == true){
    //     game->man.x += SPEED;
    //     game->man.isMoving = true;
    //     game->man.isMovingLeft = false;
    //     game->man.isMovingRight = true;
    // }
    // if(state[SDL_SCANCODE_UP] && game->man.canMoveUp == true){
    //     game->man.y -= SPEED;
    //     game->man.isMoving = true;
    // }
    // if(state[SDL_SCANCODE_DOWN] && game->man.canMoveDown == true){
    //     game->man.y += SPEED;
    //     game->man.isMoving = true;
    // }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_D])
    {
        game->man.x -= DIAGSPEED;
        game->man.y -= DIAGSPEED;
        game->man.isMoving = true;
        game->man.isMovingLeft = true;
        game->man.isMovingRight = false;
        game->man.isMovingDiagonal = true;
    }
    else if (state[SDL_SCANCODE_D] && state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_A])
    {
        game->man.x += DIAGSPEED;
        game->man.y -= DIAGSPEED;
        game->man.isMoving = true;
        game->man.isMovingLeft = false;
        game->man.isMovingRight = true;
        game->man.isMovingDiagonal = true;
    }
    else if (state[SDL_SCANCODE_D] && state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_A])
    {
        game->man.x += DIAGSPEED;
        game->man.y += DIAGSPEED;
        game->man.isMoving = true;
        game->man.isMovingLeft = false;
        game->man.isMovingRight = true;
        game->man.isMovingDiagonal = true;
    }
    else if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_D])
    {
        game->man.x -= DIAGSPEED;
        game->man.y += DIAGSPEED;
        game->man.isMoving = true;
        game->man.isMovingLeft = true;
        game->man.isMovingRight = false;
        game->man.isMovingDiagonal = true;
    }
    else
    {
        if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D])
        {
            game->man.x -= SPEED;
            game->man.isMoving = true;
            game->man.isMovingLeft = true;
            game->man.isMovingRight = false;
            game->man.isMovingDiagonal = false;
        }
        if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
        {
            game->man.y -= SPEED;
            game->man.isMoving = true;
            game->man.isMovingDiagonal = false;
        }
        if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_W])
        {
            game->man.y += SPEED;
            game->man.isMoving = true;
            game->man.isMovingDiagonal = false;
        }
        if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A])
        {
            game->man.x += SPEED;
            game->man.isMoving = true;
            game->man.isMovingLeft = false;
            game->man.isMovingRight = true;
            game->man.isMovingDiagonal = false;
        }
    }
    return done;
}

void doRender(SDL_Renderer *renderer, GameState *game)
{
    if(game->statusState == STATUS_STATE_INTRO)
        drawStatusLives(game);
    else if(game->statusState == STATUS_STATE_GAME){

        // SDL_SetRenderDrawColor(renderer, 55,166,252,255);
        // SDL_RenderClear(renderer);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, game->alley, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);

        SDL_Rect rect = {game->man.x, game->man.y, MANSIZE, MANSIZE};

        //Draw blood when dead
        if(game->man.isDead){
            SDL_Rect rect = {game->man.x, game->man.y, 48, 48};
            SDL_RenderCopyEx(renderer, game->blood, NULL, &rect, 0, NULL, (game->time%40<10));
        }

        //Homemade scuffed animation
        static int animationTick = 0, frame = 0;
        static bool flip = false;
        SDL_RenderCopyEx(renderer, game->manFrames[frame], NULL, &rect, 0, NULL, (flip));
        animationTick++;
        if(animationTick > 15 && game->man.isMoving){
            if(game->man.isMovingLeft){
                flip = true;
            }
            else if(game->man.isMovingRight){
                flip = false;
            }
            frame++;
            animationTick = 0;
        }
        else if(game->man.isMoving == false) frame = 0; 
        if(frame >= 4) frame = 1;

        //Draw image
        for (int i = 0; i < WALLCOUNT; i++)
        {
            SDL_Rect wallRect = {game->walls[i].x, game->walls[i].y, WALLSIZE, WALLSIZE};
            SDL_RenderCopy(renderer, game->wall, NULL, &wallRect);
        }
        for(int i = 0; i<WALLCOUNT/4;i++){
            SDL_Rect brickRect = {game->bricks[i].x, game->bricks[i].y, 256/8, 800/8};
            SDL_RenderCopy(renderer, game->pin, NULL, &brickRect);
        }
    }
    //When done drawing, present what is drawn!
    SDL_RenderPresent(renderer);
}