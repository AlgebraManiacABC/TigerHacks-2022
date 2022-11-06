#include "survival.h"

//	Need: Difficulty, Countdown time
//	Difficulty determines how dangerous hazards like asteroids and enemies are

int survivalLoop(SDL_Window *w, SDL_Renderer *r, spaceShip ship)
{
	time_t timeRemaining = COOLDOWN_TIME;
	time_t time0 = time(NULL);

	TTF_Font *font = TTF_OpenFont(ROBOTO,72);

	outerSpace space;
	initDebrisArray(space.junk);
	initEnemyArray(space.enemies);

	int ww,wh;
	SDL_GetWindowSize(w,&ww,&wh);
	gIMG bg = CreateImgFromFile(r,SURVIVAL_BG);
	if(!bg.tx)
	{
		fprintf(stderr,"Error creating image! [%s]\n",SDL_GetError());
		return EXIT_FAILURE;
	}
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
						case SDL_SCANCODE_ESCAPE:
							survive = false;
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

		moveDebris(space.junk,ww,wh);
		moveEnemies(space.enemies,ww,wh);

		//	One in 512 chance per frame to spawn X: if(!(rand()%512))
		//	One in 512 chance per second to spawn X: if(!(rand()%(512*FPS)))
		//	TODO: Change arbitrary chance to scale with difficulty
		if(!(rand()%((int)FPS*1)))	//	Chance per second to spawn something
		{
			int err;
			if(rand()%2)
				err = spawnDebris(r,space.junk,ww,wh,ship.x,ship.y,0);
			else
				err = spawnEnemy(r,space.enemies,ww,wh,0);
			switch(err)
			{
				case SPAWN_FAIL_MEM:
					fprintf(stderr,"Memory error!\n");
					return EXIT_FAILURE;
				case SPAWN_FAIL_IMG:
					fprintf(stderr,"Error creating image! [%s]\n",SDL_GetError());
					return EXIT_FAILURE;
				case SPAWN_SUCCESS:
				case SPAWN_FAIL_PROX:
				case SPAWN_FAIL_FULL:
				//	Normal behavior
					break;
			}
		}

		SDL_RenderClear(r);
		gIMG_RenderCopy(r,&bg);
		renderShip(r,ww,wh,ship);
		renderDebris(r,space.junk,ww,wh);
		renderCooldown(r, ww, wh, timeRemaining, font);
		SDL_RenderPresent(r);

		SDL_Delay(1000 / FPS);
	}
	return EXIT_FAILURE;
}

void initDebrisArray(debris * junk[MAX_DEBRIS])
{
	for(int i=0; i<MAX_DEBRIS; i++)
	{
		junk[i] = NULL;
	}
}

void initEnemyArray(enemy * enemies[MAX_ENEMIES])
{
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		enemies[i] = NULL;
	}
}

int spawnDebris(SDL_Renderer *r, debris * junk[MAX_DEBRIS], int ww, int wh, int shipx, int shipy, int difficulty)
{
	int i;
	for(i=0; i<MAX_DEBRIS && junk[i]; i++);
	if(i<MAX_DEBRIS)
	{
		junk[i] = malloc(sizeof(debris));
		if(!junk[i])
		{
			fprintf(stderr,"Memory error!\n");
			return SPAWN_FAIL_MEM;
		}
		junk[i]->rotation = rand()%360;
		junk[i]->size = (rand()%2)+1;//1 or 2, for Small/Large
		junk[i]->type = rand()%NUM_DEBRIS_TYPES;
		junk[i]->vx = (rand()%(DEBRIS_V_MAX*2))-DEBRIS_V_MAX;
		junk[i]->vy = (rand()%(DEBRIS_V_MAX*2))-DEBRIS_V_MAX;
		if(junk[i]->vx > junk[i]->vy)	//	Start at left/right border if vx>vy
		{
			//	Place *just* offscreen
			if(junk[i]->size==SMALL_DEBRIS_ID)
				junk[i]->x = SMALL_DEBRIS_RADIUS;
			else
				junk[i]->x = LARGE_DEBRIS_RADIUS;

			//	If going right, set offset to negative x, instead of positive, which it is currently
			if(junk[i]->vx > 0)
				junk[i]->x *= -1;
			else
				junk[i]->x += ww;

			//	Place on top half of border if vy < 10, lower half if vy > 10, middle if between
			if(junk[i]->vy < 10)
				junk[i]->y = (rand()%(wh/2))+1+(wh/2);
			else if(junk[i]->vy > 10)
				junk[i]->y = (rand()%(wh/2))+1;
			else
				junk[i]->y = wh/2;
		}
		else	//	Else, start at top/bottom border
		{
			if(junk[i]->size==SMALL_DEBRIS_ID)
				junk[i]->y = SMALL_DEBRIS_RADIUS;
			else
				junk[i]->y = LARGE_DEBRIS_RADIUS;

			if(junk[i]->vy > 0)
				junk[i]->y *= -1;
			else
				junk[i]->y += wh;

			//	Place on right half of border if vx < 10, left half if vx > 10, middle if between
			if(junk[i]->vx < 10)
				junk[i]->x = (rand()%(ww/2))+1+(ww/2);
			else if(junk[i]->vx > 10)
				junk[i]->x = (rand()%(ww/2))+1;
			else
				junk[i]->y = wh/2;
		}
		if(sqrt(pow(shipx-junk[i]->x,2)+pow(shipy-junk[i]->y,2)) < MIN_SHIP_DIST)
		{
			free(junk[i]);
			return SPAWN_FAIL_PROX;
			//	Too close to the ship
		}

		junk[i]->img = CreateImgFromFile(r,debrisFiles[junk[i]->type]);
		if(!(junk[i]->img.tx))
		{
			return SPAWN_FAIL_IMG;
		}
		if(junk[i]->size == SMALL_DEBRIS_ID)
			gIMG_Resize(&(junk[i]->img),SMALL_DEBRIS_RADIUS,SMALL_DEBRIS_RADIUS);
		else
			gIMG_Resize(&(junk[i]->img),LARGE_DEBRIS_RADIUS,LARGE_DEBRIS_RADIUS);
		junk[i]->img.rect.x = junk[i]->x;
		junk[i]->img.rect.y = junk[i]->y;

		struct timeval now;
		gettimeofday(&now,NULL);
		junk[i]->spawntime = now.tv_usec + (1000*1000*now.tv_sec);
		//	Time it would take for the entire width of the rect to be within the window
		//	i.e., time for x pos to move right 2*radius
		//	v = d/t -> t = d/v
		if(junk[i]->vx)
			junk[i]->incoming_x = fabs(2*1000*1000*(((double)junk[i]->img.rect.w)/(double)(junk[i]->vx)));
		else
			junk[i]->incoming_x = 0;

		if(junk[i]->vy)
			junk[i]->incoming_y = fabs(2*1000*1000*(((double)junk[i]->img.rect.h)/(double)(junk[i]->vy)));
		else
			junk[i]->incoming_y = 0;
	}
	else
		return SPAWN_FAIL_FULL;

	//printf("%s (of size %d [%d,%d]) successfully spawned at (%.0f, %.0f) with velocity (%d, %d) and times (%.2f, %.2f)\n",
	//	debrisFiles[junk[i]->type],junk[i]->size,junk[i]->img.rect.w,junk[i]->img.rect.h,
	//	junk[i]->x,junk[i]->y,junk[i]->vx,junk[i]->vy,
	//	(double)junk[i]->incoming_x/(1000*1000),(double)junk[i]->incoming_y/(1000*1000));
	return SPAWN_SUCCESS;
}

int spawnEnemy(SDL_Renderer *r, enemy * enemies[MAX_ENEMIES], int ww, int wh, int difficulty)
{
	return EXIT_FAILURE;
}

bool isCollision(spaceShip ship, outerSpace env)
{
	return false;
}

void moveDebris(debris * junk[MAX_DEBRIS], int ww, int wh)
{
	struct timeval now;
	gettimeofday(&now,NULL);
	for(int i=0; i<MAX_DEBRIS; i++)
	{
		if(junk[i])
		{
			if(junk[i]->incoming_x &&
					((now.tv_sec*1000*1000 + now.tv_usec) - (junk[i]->spawntime) > junk[i]->incoming_x))
				junk[i]->incoming_x = 0;
			if(junk[i]->incoming_y &&
					((now.tv_sec*1000*1000 + now.tv_usec) - (junk[i]->spawntime) > junk[i]->incoming_y))
				junk[i]->incoming_y = 0;
			junk[i]->rotation += (junk[i]->vx)/FPS;
			junk[i]->x += (junk[i]->vx)/FPS;
			junk[i]->y += (junk[i]->vy)/FPS;

			if(junk[i]->rotation > 360)
				junk[i]->rotation -= 360;
			else if(junk[i]->rotation < 0)
				junk[i]->rotation += 360;

			if(!(junk[i]->incoming_x))
			{
				if(junk[i]->x > ww)
				{
					junk[i]->x -= ww;
				}
				if(junk[i]->x < 0)
				{
					junk[i]->x += ww;
				}
			}
			if(!(junk[i]->incoming_y))
			{
				if(junk[i]->y > wh)
				{
					junk[i]->y -= wh;
				}
				if(junk[i]->y < 0)
				{
					junk[i]->y += wh;
				}
			}

			junk[i]->img.rect.x = junk[i]->x - (junk[i]->img.rect.w)/2;
			junk[i]->img.rect.y = junk[i]->y - (junk[i]->img.rect.h)/2;
		}
	}
}

void moveEnemies(enemy * enemies[MAX_ENEMIES], int ww, int wh)
{
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		if(enemies[i])
		{
			enemies[i]->x += enemies[i]->vx;
			enemies[i]->y += enemies[i]->vy;
		}
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

void renderDebris(SDL_Renderer *r, debris * junk[MAX_DEBRIS], int ww, int wh)
{
	for(int i=0; i<MAX_DEBRIS; i++)
	{
		if(junk[i])
		{
			SDL_Rect temp = junk[i]->img.rect;
			SDL_RenderCopyEx(r,junk[i]->img.tx,NULL,&(temp),junk[i]->rotation,NULL,SDL_FLIP_NONE);
			if((junk[i]->incoming_x)==0.0)
			{
				temp.x += ww;
				SDL_RenderCopyEx(r,junk[i]->img.tx,NULL,&(temp),junk[i]->rotation,NULL,SDL_FLIP_NONE);
				temp.x -= 2*ww;
				SDL_RenderCopyEx(r,junk[i]->img.tx,NULL,&(temp),junk[i]->rotation,NULL,SDL_FLIP_NONE);
				temp.x += ww;
			}
			if((junk[i]->incoming_y)==0.0)
			{
				temp.y += wh;
				SDL_RenderCopyEx(r,junk[i]->img.tx,NULL,&(temp),junk[i]->rotation,NULL,SDL_FLIP_NONE);
				temp.y -= 2*wh;
				SDL_RenderCopyEx(r,junk[i]->img.tx,NULL,&(temp),junk[i]->rotation,NULL,SDL_FLIP_NONE);
				temp.y += wh;
			}
		}
	}
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