#include "main.h"

int main(int argc, char ** argv)
{
	SDL_Window *w;
	SDL_Renderer *r;
	if(gSDL_Init(SDL_INIT_EVERYTHING,&w,640,400,SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED,
		"Space TigerHacks 2022",&r))
	{
		fprintf(stderr,"Error initializing SDL: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}
	printf("Initialization successful!\n");
	gSDL_MaximizeWindow(&w,&r);

	//playGame(r);

	gSDL_Close(LEVEL_REND,r,w);
	return EXIT_SUCCESS;
}