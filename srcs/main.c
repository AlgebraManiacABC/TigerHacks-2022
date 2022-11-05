#include "main.h"
#include "survival.h"

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
	//gSDL_MaximizeWindow(&w,&r,"Space TigerHacks 2022");

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