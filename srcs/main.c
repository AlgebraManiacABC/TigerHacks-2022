#include "main.h"
#include "survival.h"
#include "warp.h"
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
	if(TTF_Init())
	{
		fprintf(stderr,"Error initializing TTF: %s\n",TTF_GetError());
		return EXIT_FAILURE;
	}
	printf("Initialization successful!\n");
	//gSDL_MaximizeWindow(&w,&r,"Space TigerHacks 2022");

	int ww,wh;
	SDL_GetWindowSize(w,&ww,&wh);
	warp(r,ww,wh);
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