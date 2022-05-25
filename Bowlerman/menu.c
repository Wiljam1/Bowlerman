#include <stdio.h>
#include <SDL2/SDL.h>
#include <windows.h>
#include "menu.h"
#include "game.h"
#include "udpClient.h"
#include "tcpClient.h"

#define TEXTLENGTH 20

char inputIp[TEXTLENGTH];
int textIndex = 0;

void menu(Game theGame, bool *quitGame, UDPStruct *udpvalues){
    int currentMenu = 0;
    int x;
    int y;
    int buttonsPerMenu = 4;
    int buttonsInRow = 3;
    int listenForIp = 0;
    int textWidth = 0;
    char ip1[]={"127.0.0.1"};
    char ip2[]={"127.0.0.2"};
    

    SDL_Color color = {255, 255, 255, 0};
    SDL_Texture *ipText;
 
    SDL_Rect buttonLocation[buttonsPerMenu];
    SDL_Rect logo = {WIDTH / 2 - 100, 50, 200, 100};
    SDL_Rect text = {WIDTH / 2 - 90, HEIGHT / 3 + 15};
    for (int i = 0; i < buttonsInRow; i++){
        buttonLocation[i].x = WIDTH / 2 - 100;
        buttonLocation[i].y = HEIGHT / 3 + i * 100;
        buttonLocation[i].w = 200;
        buttonLocation[i].h = 50;
    }
    buttonLocation[3].x = WIDTH / 2 - 250;
    buttonLocation[3].y = HEIGHT / 3 - 50;
    buttonLocation[3].w = 100;
    buttonLocation[3].h = 50;

    updateMenu(theGame, 0, buttonLocation, logo);

    SDL_Thread* thread; //thread used for TCP-waiting (when joining game)
    TCPstruct tcpValues = createTCPstruct();     //returns a struct for tcp-init-struct.	
    ThreadStruct threadStruct;          //creates a struct for threads for TCP
	threadStruct.startflag=0;

    bool running = true;
    while(running){
        while(SDL_PollEvent(&theGame->window_event)){
            SDL_Event event = theGame->window_event;
            switch (event.type){
                case SDL_QUIT:
                    *quitGame = true;
                    running = false;
                    //destroyGame(theGame);
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    if(theGame->window)
                    {
                        *quitGame = true;
                        running = false;
                        theGame->window = NULL;
                        //destroyGame(theGame);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = event.motion.x;
                    y = event.motion.y;
                    for(int i = 0; i < buttonsPerMenu; i++){
                        if(x >= buttonLocation[i].x && x <= buttonLocation[i].x + buttonLocation[i].w && y >= buttonLocation[i].y && y <= buttonLocation[i].y + buttonLocation[i].h){
                        if (i == 2){
                        *quitGame = true;
                        running = false;
                        //destroyGame(theGame);
                        }
                        switch(currentMenu){
                            case 0:
                                if(i == 0) currentMenu = updateMenu(theGame, 1, buttonLocation, logo), listenForIp = 1;
                                if(i == 1) currentMenu = updateMenu(theGame, 2, buttonLocation, logo), listenForIp = 1;
                                break;
                            case 1:
                                printf("join: %d", currentMenu);
                                if(i == 0) listenForIp = 1;
                                if(i == 1){ // join server
                                    if(textIndex > 0){
                                        strcpy(udpvalues->serverIp, inputIp);
                                        initTCP(&tcpValues, inputIp);		//connectar till angiven Ip-adress
                                    } else {
                                        strcpy(udpvalues->serverIp, ip2);
                                        initTCP(&tcpValues, ip2);		//connectar till angiven local Ip-adress
                                    }
                                        threadStruct.sd=tcpValues.sd; //copy socket descriptor into thread-struct
                                        theGame->playerIDLocal = getPlayerIDviaTCP(&tcpValues); 
                        
                                        thread = SDL_CreateThread(receiveTCP, "test", (void *) &threadStruct); //creates a thread waiting to see if the game is starting or not.

                                        *quitGame = false;

                                    //running = 0;
                                }
                                if(i == 3){ // return to main menu
                                    listenForIp = 0;
                                    currentMenu = updateMenu(theGame, 0, buttonLocation, logo);
                                    if(textIndex > 0){
                                        clearString();
                                        textWidth = 0;
                                    } 
                                }
                                break;
                            case 2:
                                printf("host : %d", currentMenu);
                                if(i == 0) listenForIp = 1;
                                if(i == 1){ // host server
                                //ShellExecuteA(GetDesktopWindow(),"open","udpServer.exe",NULL,NULL,SW_SHOW);  //Start server file
                                //SDL_Delay(1000);

                                if(textIndex > 0){
                                    strcpy(udpvalues->serverIp, inputIp);
                                    initTCP(&tcpValues, inputIp);		//connectar till angiven Ip-adress
                                } else {
                                    strcpy(udpvalues->serverIp, ip1);
                                    initTCP(&tcpValues, ip1);		//connectar till angiven local Ip-adress
                                }
                                threadStruct.sd=tcpValues.sd;  //copy socket descriptor into thread-struct
                                theGame->playerIDLocal = getPlayerIDviaTCP(&tcpValues); 
                                        
                                startGameViaTCP(&tcpValues, &threadStruct); //starts the game, and sends the info out to all other clients
                                *quitGame = false;
                                } //start game
                            if(i == 3){ // return to main menu
                                listenForIp = 0;
                                currentMenu = updateMenu(theGame, 0, buttonLocation, logo);
                                if(textIndex > 0){
                                    clearString();
                                    textWidth = 0;
                                } 
                            }
                            break;
                            }
                        } 
                    }
                    break;
            }
            if(listenForIp){
                if(event.type == SDL_KEYDOWN){
                    char keyDown = event.key.keysym.scancode;
                    if (keyDown == SDL_SCANCODE_BACKSPACE && textIndex > 0){
                        textIndex--;
                        inputIp[textIndex] = '\0';
                        SDL_RenderCopy(theGame->renderer, theGame->menuButtons[6], NULL, &buttonLocation[0]);
                        if(textIndex > 0){
                            theGame->window_surface = TTF_RenderText_Blended(theGame->font, inputIp, color);
                            textWidth = theGame->window_surface->w;
                            text.w = theGame->window_surface->w; text.h = theGame->window_surface->h;
                            ipText = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
                            SDL_FreeSurface(theGame->window_surface);
                            SDL_RenderCopy(theGame->renderer, ipText, NULL, &text);
                        } 
                        SDL_RenderPresent(theGame->renderer);
                    } 
                    else if (keyDown == SDL_SCANCODE_RETURN) listenForIp = 0;
                }
                SDL_StartTextInput();
                if(event.type == SDL_TEXTINPUT && textIndex < TEXTLENGTH && textWidth < 180){
                    if(textIndex == 0) strcpy(inputIp, event.text.text);
                    else strcat(inputIp, event.text.text);
                    textIndex++;
                    theGame->window_surface = TTF_RenderText_Blended(theGame->font, inputIp, color);
                    textWidth = theGame->window_surface->w;
                    text.w = theGame->window_surface->w; text.h = theGame->window_surface->h;
                    ipText = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
                    SDL_FreeSurface(theGame->window_surface);
                    SDL_RenderCopy(theGame->renderer, ipText, NULL, &text);
                    SDL_RenderPresent(theGame->renderer);
                }
            }
        }
        if(threadStruct.startflag==1) //if game should start
		{
            printf("playerID: %d\n", theGame->playerIDLocal);
			printf("playerammount: %d\n", threadStruct.playerAmmount);
			printf("startflag: %d\n", threadStruct.startflag);
            theGame->playerAmount=threadStruct.playerAmmount; //set playerAmount in theGamestruct to the value returned from the thread listening to TCP
			//get playerammount & start game (exit breakLoop)
			//thread=NULL;  //closes thread
            running = false;
			break;
		}
        SDL_Delay(1000/60);
    }
}

int updateMenu(Game theGame, int menuNumber, SDL_Rect buttonLocation[], SDL_Rect logo){
    SDL_RenderCopy(theGame->renderer, theGame->menuBackground, NULL, NULL);
    SDL_RenderCopy(theGame->renderer, theGame->logo, NULL, &logo);
    SDL_RenderPresent(theGame->renderer);
    switch(menuNumber){
        case 0: // main menu
            for(int i = 0; i <= 1; i++){
            SDL_RenderCopy(theGame->renderer, theGame->menuButtons[i], NULL, &buttonLocation[i]);
            }
        break;
        case 1: // join menu
            SDL_RenderCopy(theGame->renderer, theGame->menuButtons[6], NULL, &buttonLocation[0]);
            SDL_RenderCopy(theGame->renderer, theGame->menuButtons[2], NULL, &buttonLocation[1]);
            SDL_RenderCopy(theGame->renderer, theGame->menuButtons[5], NULL, &buttonLocation[3]);
        break;
        case 2: // host menu
            SDL_RenderCopy(theGame->renderer, theGame->menuButtons[6], NULL, &buttonLocation[0]);
            SDL_RenderCopy(theGame->renderer, theGame->menuButtons[3], NULL, &buttonLocation[1]);
            SDL_RenderCopy(theGame->renderer, theGame->menuButtons[5], NULL, &buttonLocation[3]);
        break;
        }
    SDL_RenderCopy(theGame->renderer, theGame->menuButtons[4], NULL, &buttonLocation[2]);
    SDL_RenderPresent(theGame->renderer);
    return menuNumber;
}

void clearString(){
    for(int i = textIndex; i >= 0; i--){
        inputIp[i] = '\0';
    }
    textIndex = 0;
}
