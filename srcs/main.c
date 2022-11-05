#include "main.h"
#include "survival.h"
#include "menu.h"

int main(int argc, char ** argv)
{
	SDL_Window *w;
	SDL_Renderer *r;
	if(gSDL_Init(SDL_INIT_EVERYTHING,&w,640,400,SDL_WINDOW_RESIZABLE,
		"Space TigerHacks 2022",&r))
	{
		fprintf(stderr,"Error initializing SDL: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}
	TTF_Init();
	printf("Initialization successful!\n");
	gSDL_MaximizeWindow(&w,&r,"Space TigerHacks 2022");

    SDL_RenderClear(r);
    SDL_RenderPresent(r);
    int next = summon_main_menu(w,r);
    switch(next)
    {
        case MAIN_MENU_ERROR:
            fprintf(stderr,"An unknown error occurred in the main menu! Quitting...\n");
			return -1;
            break;
        case START_NEW:
            break;
        case START_CONTINUE:
			break;
        case MAIN_MENU_OPTIONS:
        case MAIN_MENU_QUIT:
			return -1;
            break;
    }	

	spaceShip ship;
	gIMG shipIMG = CreateImgFromFile(r,PLAYER);
	gIMG_Resize(&shipIMG,100,100);
	ship.tx = shipIMG.tx;
	ship.rect = shipIMG.rect;
	ship.accel = 100;
	ship.vrotate = 100;
	survivalLoop(w,r,ship);
	SDL_DestroyTexture(shipIMG.tx);

	gSDL_Close(LEVEL_REND,r,w);
	TTF_Quit();
	return EXIT_SUCCESS;
}