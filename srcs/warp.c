#include "warp.h"

int warp(SDL_Renderer *r, int ww, int wh)
{
	//	Play startup animation
	//	Show options
	//	Save and Quit -> Write save and quit game
	//	Map -> View map
	//	Continue -> Finish warp animation (with starry eyes)
	//	|
	//	`-> Pit stop or survival

	playWarpAnim_1(r,ww,wh);
	int choice = tunnelLoop(r,ww,wh);	//	Loops through the tunnel animation until they select an option
	switch(choice)
	{
		case WARP_SAVE_AND_QUIT:
		case WARP_MAP:
		case WARP_CONTINUE:
			playWarpAnim_2(r,ww,wh);
			break;
		default:
			fprintf(stderr,"Erroneous selection during warp animation!\n");
			break;
	}
	return choice;
}

//	Render functions

void playWarpAnim_1(SDL_Renderer *r, int ww, int wh)
{
	SDL_Rect screen;
	screen.x = 0;
	screen.y = 0;
	screen.w = ww;
	screen.h = wh;
	gIMG anim;
	char file[64]={0};
	snprintf(file,64,"%s%02d.png",WARP_START_BASE,0);
	int len = strnlen(WARP_START_BASE,sizeof(WARP_START_BASE));

	for(int i=01; i<=NUM_WARP_START; i++)
	{
		file[len] = '0'+(i/10);
		file[len+1] = '0'+(i%10);
		anim = CreateImgFromFile(r,file);
		if(!anim.tx)
		{
			fprintf(stderr,"Image error in animation! [%s]\n",SDL_GetError());
			return;
		}
		SDL_RenderClear(r);
		SDL_RenderCopy(r,anim.tx,NULL,&screen);
		SDL_RenderPresent(r);
		SDL_DestroyTexture(anim.tx);
		SDL_Delay(2000 / FPS);
	}
}

int tunnelLoop(SDL_Renderer *r, int ww, int wh)
{
	return WARP_CONTINUE;
}

void playWarpAnim_2(SDL_Renderer *r, int ww, int wh)
{
	SDL_Rect screen;
	screen.x = 0;
	screen.y = 0;
	screen.w = ww;
	screen.h = wh;
	gIMG anim;
	char file[64]={0};
	snprintf(file,64,"%s%02d.png",WARP_END_BASE,0);
	int len = strnlen(WARP_END_BASE,sizeof(WARP_END_BASE));

	for(int i=01; i<=NUM_WARP_END; i++)
	{
		file[len] = '0'+(i/10);
		file[len+1] = '0'+(i%10);
		anim = CreateImgFromFile(r,file);
		if(!anim.tx)
		{
			fprintf(stderr,"Image error in animation! [%s]\n",SDL_GetError());
			return;
		}
		SDL_RenderClear(r);
		SDL_RenderCopy(r,anim.tx,NULL,&screen);
		SDL_RenderPresent(r);
		SDL_DestroyTexture(anim.tx);
		SDL_Delay(2000 / FPS);
	}
}