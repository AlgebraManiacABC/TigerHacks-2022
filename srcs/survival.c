#include "survival.h"

//	Need: Difficulty, Countdown time
//	Difficulty determines how dangerous hazards like asteroids and enemies are

void survivalLoop(SDL_Window *w, SDL_Renderer *r, spaceShip ship)
{
	time_t timeRemaining = COOLDOWN_TIME;
	time_t time0 = time(NULL);

	TTF_Font *font = TTF_OpenFont(ROBOTO,72);
	SDL_Color colorWhite = {255,255,255,255};
	CreateSolidTextureFromText(r,font,"Test",colorWhite);

	int ww,wh;
	SDL_GetWindowSize(w,&ww,&wh);
	printf("Window size: [%d, %d]\n",ww,wh);
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
					return;
				case SDL_WINDOWEVENT:
					switch(event.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							ww = event.window.data1;
							wh = event.window.data2;
							printf("New windows size detected: [%d, %d]\n", ww, wh);
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

		SDL_RenderClear(r);
		renderShip(r,ww,wh,ship);
		renderCooldown(r, ww, wh, timeRemaining,font);
		SDL_RenderPresent(r);

		SDL_Delay(1000 / FPS);
	}
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
}