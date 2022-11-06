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
			break;
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
	TTF_Font * font = TTF_OpenFont(MENUFONT,144);
	if(!font)
	{
		fprintf(stderr,"Font error! [%s]\n",TTF_GetError());
		return WARP_SAVE_AND_QUIT;
	}

	SDL_Color selectColor[]=
	{
		{255,255,25,255},	//	Yellow
		{255,255,255,255}	//	White
	};

	SDL_Texture *** textArray = malloc(sizeof(SDL_Texture **) * 2);

	char warpOptions[][16]=
	{
		"CONTINUE",
		"MAP",
		"QUIT"
	};

	SDL_Rect rectArray[3]=
	{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};

	for(int i=0; i<3; i++)
	{
		textArray[i] = malloc(sizeof(SDL_Texture *));
		textArray[i][false] = CreateSolidTextureFromText(r,font,warpOptions[i],selectColor[false]);
		textArray[i][true] = CreateSolidTextureFromText(r,font,warpOptions[i],selectColor[true]);
		SDL_QueryTexture(textArray[i][false],NULL,NULL,&rectArray[i].w,&rectArray[i].h);
	}

	rectArray[0].x = 50;
	rectArray[0].y = 50;
	rectArray[2].x = 200;
	rectArray[2].y = 200;

	TTF_CloseFont(font);

	int selection = WARP_NONE;
	while(selection == WARP_NONE)
	{
		bool clickDown = false;
		bool clickUp = false;
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					selection = WARP_SAVE_AND_QUIT;
					break;
				default:
					break;
			}
		}

		Uint32 hover;
		selection = warpEvaluateClicks(&hover,ww,wh,rectArray);

		SDL_RenderClear(r);
		renderTunnelOptions(r,ww,wh);
		SDL_RenderPresent(r);
	}
	return selection;
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

void renderTunnelOptions(SDL_Renderer *r, int ww, int wh)
{
}

int warpEvaluateClicks(Uint32 *hover, int ww, int wh, SDL_Rect arr[3])
{
	static Uint32 clickHeld = 0;
	int selection = WARP_NONE;

	int x,y;
	static Uint32 button;
	Uint32 buttonOld = button;
	button = SDL_GetMouseState(&x,&y);
	bool leftClickUp;//	Left click was released since last call
	bool leftClickDown = false;//	Left click is currently held down
	if( (buttonOld & SDL_BUTTON(SDL_BUTTON_LEFT)) && !(button & SDL_BUTTON(SDL_BUTTON_LEFT)) )
		leftClickUp = true;
	leftClickDown = button & SDL_BUTTON(SDL_BUTTON_LEFT);

	if(isMouseOverRect(x,y,arr[0]))
	{
		*hover = WARP_MASK_CONTINUE;
		if(leftClickDown)
			clickHeld = WARP_MASK_CONTINUE;
		else if(leftClickUp && (clickHeld & WARP_MASK_CONTINUE))
			selection = WARP_CONTINUE;
	}
	else if(isMouseOverRect(x,y,arr[2]))
	{
		*hover = WARP_MASK_SAVEQUIT;
		if(leftClickDown)
			clickHeld = WARP_MASK_SAVEQUIT;
		else if(leftClickUp && (clickHeld & WARP_MASK_SAVEQUIT))
			selection = WARP_SAVE_AND_QUIT;
	}

	return selection;
}
