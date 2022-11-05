#include "survival.h"

//	Need: Difficulty, Countdown time
//	Difficulty determines how dangerous hazards like asteroids and enemies are

int survivalLoop(SDL_Window *w, SDL_Renderer *r, spaceShip ship)
{
	time_t timeRemaining = COOLDOWN_TIME;
	time_t time0 = time(NULL);

	TTF_Font *font = TTF_OpenFont(ROBOTO,72);

	int ww,wh;
	SDL_GetWindowSize(w,&ww,&wh);
	gIMG bg = CreateImgFromFile(r,SURVIVAL_BG);
	gIMG_Resize(&bg,ww,wh);

	ship.rotation = 270;
	ship.x = ww/2;
	ship.y = wh/2;
	ship.vx = 0;
	ship.vy = 0;

	bool survive = true;
	SDL_Event event;
	bool accelerating = false;
	bool rotating_left = false;
	bool rotating_right = false;
	while(survive)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					return EXIT_SUCCESS;
				case SDL_WINDOWEVENT:
					switch(event.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							ww = event.window.data1;
							wh = event.window.data2;
							gIMG_Resize(&bg,ww,wh);
							break;
					}
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode)
					{
						case SDL_SCANCODE_W:
							accelerating = true;
							break;
						case SDL_SCANCODE_A:
							rotating_left = true;
							break;
						case SDL_SCANCODE_D:
							rotating_right = true;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.scancode)
					{
						case SDL_SCANCODE_W:
							accelerating = false;
							break;
						case SDL_SCANCODE_A:
							rotating_left = false;
							break;
						case SDL_SCANCODE_D:
							rotating_right = false;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		if(accelerating)
		{
			float testvx = ship.vx + (ship.accel*cos((ship.rotation/180)*M_PI))/FPS;
			float testvy = ship.vy + (ship.accel*sin((ship.rotation/180)*M_PI))/FPS;
			if(sqrt(testvx*testvx+testvy*testvy)<V_MAX)
			{
				ship.vx = testvx;
				ship.vy = testvy;
			}
		}
		else if(!accelerating)
		{
			if(ship.vx > 0)
				ship.vx -= (sqrt(DECCEL/2))/FPS;
			if(ship.vx < 0)
				ship.vx += (sqrt(DECCEL/2))/FPS;
			if(ship.vy > 0)
				ship.vy -= (sqrt(DECCEL/2))/FPS;
			if(ship.vy < 0)
				ship.vy += (sqrt(DECCEL/2))/FPS;
		}

		if(rotating_left)
		{
			ship.rotation -= (ship.vrotate)/FPS;
			if(ship.rotation < 0)
				ship.rotation += 360;
		}
		else if(rotating_right)
		{
			ship.rotation += (ship.vrotate)/FPS;
			if(ship.rotation > 360)
				ship.rotation -= 360;
		}

		ship.x += (ship.vx)/FPS;
		ship.y += (ship.vy)/FPS;

		if(ship.x > ww)	ship.x -= ww;
		if(ship.x < 0) ship.x += ww;
		if(ship.y > wh) ship.y -= wh;
		if(ship.y < 0) ship.y += wh;

		ship.rect.x = ship.x - ship.rect.w/2;
		ship.rect.y = ship.y - ship.rect.h/2;

		timeRemaining = COOLDOWN_TIME - (time(NULL) - time0);
		if(timeRemaining == 0.0)
		{
			//jump to warp
			return EXIT_SUCCESS;
		}

		SDL_RenderClear(r);
		gIMG_RenderCopy(r,&bg);
		renderShip(r,ww,wh,ship);
		renderCooldown(r, ww, wh, timeRemaining, font);
		//printf("Remaining: %ld:%02ld\n",timeRemaining/60,timeRemaining%60);
		SDL_RenderPresent(r);

		SDL_Delay(1000 / FPS);
	}
	return EXIT_FAILURE;
}

//	Render functions

void renderShip(SDL_Renderer *r, int ww, int wh, spaceShip ship)
{
	SDL_Rect imgLeft = ship.rect;
	SDL_Rect imgRight = ship.rect;
	SDL_Rect imgUp = ship.rect;
	SDL_Rect imgDown = ship.rect;

	imgLeft.x = ship.rect.x - ww;
	imgLeft.y = ship.rect.y;
	imgRight.x = ship.rect.x + ww;
	imgRight.y = ship.rect.y;
	imgUp.x = ship.rect.x;
	imgUp.y = ship.rect.y - wh;
	imgDown.x = ship.rect.x;
	imgDown.y = ship.rect.y + wh;

	SDL_RenderCopyEx(r,ship.tx,NULL,&ship.rect,ship.rotation+90,NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(r,ship.tx,NULL,&imgLeft,ship.rotation+90,NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(r,ship.tx,NULL,&imgRight,ship.rotation+90,NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(r,ship.tx,NULL,&imgUp,ship.rotation+90,NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(r,ship.tx,NULL,&imgDown,ship.rotation+90,NULL,SDL_FLIP_NONE);
}

void renderCooldown(SDL_Renderer *r, int ww, int wh, time_t remaining, TTF_Font *font)
{
	SDL_Color white = {255,255,255,255};
	char buffer[32]={0};
	snprintf(buffer,32,"%01ld:%02ld",(long int)remaining/60,(long int)remaining%60);
	SDL_Texture *timer = CreateSolidTextureFromText(r,font,buffer,white);
	if(!timer)
	{
		fprintf(stderr,"Timer texture null! [%s]\n",SDL_GetError());
		return;
	}
	SDL_Rect rect = {0,0,0,0};
	SDL_QueryTexture(timer,NULL,NULL,&rect.w,&rect.h);
	rect.x = (ww/2) - (rect.w/2);
	rect.y = 10;
	SDL_RenderCopy(r,timer,NULL,&rect);
}