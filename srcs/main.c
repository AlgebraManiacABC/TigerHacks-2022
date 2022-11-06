#include "main.h"
#include "survival.h"
#include "warp.h"
#include "menu.h"
#include "loadGame.h"
#include "assets.h"

int main(int argc, char ** argv)
{
	srand(time(NULL));
	SDL_Window *w;
	SDL_Renderer *r;
	if(gSDL_Init(SDL_INIT_EVERYTHING,&w,640,400,SDL_WINDOW_RESIZABLE,
		"Space TigerHacks 2022",&r))
	{
		fprintf(stderr,"Error initializing SDL: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}
	if(TTF_Init())
	{
		fprintf(stderr,"Error initializing TTF: %s\n",TTF_GetError());
		return EXIT_FAILURE;
	}
	printf("Initialization successful!\n");
	gSDL_MaximizeWindow(&w,&r,"Space TigerHacks 2022");

	SDL_RenderClear(r);
    SDL_RenderPresent(r);
    int next = mainMenu(w,r);
    switch(next)
    {
        case MAIN_MENU_ERROR:
            fprintf(stderr,"An unknown error occurred in the main menu! Quitting...\n");
            break;
        case START_NEW:
			//	Show "lore," then warp (no options)
			startNewGame(w,r);
            break;
        case START_CONTINUE:
			//	Read save, then continue from warp
			loadGame(w,r);
            break;
        case MAIN_MENU_OPTIONS:
			//	Show options page
            break;
        case MAIN_MENU_QUIT:
			//	Quit
            break;
    }

	//warp(r,ww,wh);

	gSDL_Close(LEVEL_REND,r,w);
	TTF_Quit();
	return EXIT_SUCCESS;
}