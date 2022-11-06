#include "loadGame.h"

void startNewGame(SDL_Window *w, SDL_Renderer *r)
{
	shipInfo info = malloc(sizeof(_shipInformation));
	info->accel = DEFAULT_ACCEL;
	info->shield = DEFAULT_SHIELD;
	info->toughness = DEFAULT_TOUGHNESS;
	info->vrotate = DEFAULT_VROTATE;
	char buffer[64]={0};
	snprintf(buffer,64,"%s%02d.png",SHIP_ID_BASE,DEFAULT_SHIP_ID);
	gIMG img = CreateImgFromFile(r,buffer);
	info->tx = img.tx;
	info->rect.x = 0;
	info->rect.y = 0;
	info->rect.w = img.rect.w / 5;
	info->rect.h = img.rect.h;
	float r1 = img.rect.w / SHIP_SIZE;
	float r2 = img.rect.h / SHIP_SIZE;
	if(r1 < r2)
	{
		info->rect.w = SHIP_SIZE;
		info->rect.h /= r1;
	}
	else
	{
		info->rect.h = SHIP_SIZE;
		info->rect.w /= r2;
	}

	spaceShip ship = buildShip(r,info);
	free(info);
	while(!survivalLoop(w,r,ship))
	{
		int ww,wh;
		SDL_GetWindowSize(w,&ww,&wh);
		warp(r,ww,wh);
	}
	SDL_DestroyTexture(img.tx);
}

spaceShip buildShip(SDL_Renderer *r, shipInfo info)
{
	spaceShip ship;
	ship.x = 0;
	ship.y = 0;
	ship.vx = 0;
	ship.vy = 0;

	ship.tx = info->tx;
	ship.rect = info->rect;
	
	ship.accel = info->accel;
	ship.shield = info->shield;
	ship.toughness = info->toughness;
	ship.vrotate = info->vrotate;

	return ship;
}

void loadGame(SDL_Window *w, SDL_Renderer *r)
{
	startNewGame(w,r);
	/*
	FILE *save = fopen(SAVE_LOCATION,"rb");
	if(!save)
	{
		startNewGame(w,r);
		return;
	}
	fclose(save);
	*/
}