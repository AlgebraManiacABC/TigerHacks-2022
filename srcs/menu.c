#include "menu.h"
#include "main.h"
#include "assets.h"
#include "survival.h"

int mainMenu(SDL_Window *w, SDL_Renderer *r)
{
    int ww,wh;
    outerSpace space = malloc(sizeof(_outerSpace));
    initDebrisArray(space->junk);
    SDL_GetWindowSize(w, &ww, &wh);

    gIMG menu_bg = CreateImgFromFile(r,MENU_BG);
    if(!menu_bg.tx)
    {
        fprintf(stderr, "Error creating image! [%s]\n", SDL_GetError());
        return MAIN_MENU_ERROR;
    }

    TTF_Font *font72 = TTF_OpenFont(ROBOTO, 150);
    if(!font72)
    {
        fprintf(stderr,"Font creation failure! [%s]\n",SDL_GetError());
        return MAIN_MENU_ERROR;
    }

    SDL_Color color_select[] =
    {
        {255, 225, 25}, //  Yellow
        {255, 255, 255} //  White
    };

    SDL_Texture *** txt_menu = malloc(sizeof(SDL_Texture **) * MAIN_MENU_COUNT);

    char menu_strings[MAIN_MENU_COUNT][10] =
    {"NEW GAME", "CONTINUE", "OPTIONS", "QUIT"};

    for (int i = 0; i < MAIN_MENU_COUNT; i++)
    {
        txt_menu[i] = malloc(sizeof(SDL_Texture *) * 2);
        txt_menu[i][false] =
            CreateSolidTextureFromText(r, font72, menu_strings[i], color_select[false]);
        txt_menu[i][true]  =
            CreateSolidTextureFromText(r, font72, menu_strings[i], color_select[true]);
    }

    TTF_CloseFont(font72);

    //INSERT AUDIO CODE

    int selection = MAIN_MENU_ERROR;
    int state = STATE_MAIN;

    while (selection == MAIN_MENU_ERROR)
    {
        moveDebris(space->junk,ww,wh);

		//	One in 512 chance per frame to spawn X: if(!(rand()%512))
		//	One in 512 chance per second to spawn X: if(!(rand()%(512*FPS)))
		//	TODO: Change arbitrary chance to scale with difficulty
		if(!(rand()%((int)FPS*1)))	//	Chance per second to spawn something
		{
			int err;
			if(rand()%2)
				err = spawnDebris(r,space->junk,ww,wh,0,0,0);
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

		SDL_Delay(1000 / FPS);
        bool click_up = false;
        bool click_down = false;
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    selection = MAIN_MENU_QUIT;
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        selection = MAIN_MENU_QUIT;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT)
                        click_up = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT)
                        click_down = true;
                    break;
                default:
                    break;
            }
        }

        if (selection == MAIN_MENU_QUIT)
            break;

        Uint32 hover;
        selection = evaluateClicks(&hover, state, click_up, click_down, ww, wh);
        
        if (selection == START_NEW)
            state = STATE_START;

        SDL_RenderClear(r);
        gIMG_RenderCopy(r,&menu_bg);
        renderDebris(r,space->junk,ww,wh);
        if(state == STATE_MAIN)
            renderMainOptions(r,
                txt_menu[START_NEW][(bool)(hover&MASK_NEW)],
                txt_menu[START_CONTINUE][(bool)(hover&MASK_CONTINUE)],
                txt_menu[MAIN_MENU_OPTIONS][(bool)(hover&MASK_OPTIONS)],
                txt_menu[MAIN_MENU_QUIT][(bool)(hover&MASK_QUIT)], ww, wh);

        SDL_RenderPresent (r);        
    }

    for (int i = 0; i < MAIN_MENU_COUNT; i++)
    {
        SDL_DestroyTexture(txt_menu[i][false]);
        SDL_DestroyTexture(txt_menu[i][true]);
        free(txt_menu[i]);
    }
    free(txt_menu);
    //Free audio
    free(space);

    return selection;
}

int evaluateClicks(Uint32 * hover, int state, bool click_up, bool click_down, int w, int h)
{
    static Uint32 click_held = 0;

    int x,y;
    int selection = MAIN_MENU_ERROR;
    SDL_GetMouseState(&x,&y);
    
    if(state == STATE_MAIN)
    {
        if(mouseOverNew(x,y, w, h) == true)
        {
            *hover = MASK_NEW;
            if(click_down)
                click_held = MASK_NEW;
            if(click_up && (click_held&MASK_NEW))
                selection = START_NEW;
        }
        else if (mouseOverContinue(x,y, w, h))
        {
            *hover = MASK_CONTINUE;
            if(click_down)
                click_held = MASK_CONTINUE;
            if(click_up && (click_held&MASK_CONTINUE))
                selection = START_CONTINUE;
        }
        else if(mouseOverOptions(x,y, w, h))
        {
            *hover = MASK_OPTIONS;
            if(click_down)
                click_held = MASK_OPTIONS;
            if(click_up && (click_held&MASK_OPTIONS))
                selection = MAIN_MENU_ERROR;
        }
        else if(mouseOverQuit(x,y, w, h))
        {
            *hover = MASK_QUIT;
            if(click_down)
                click_held = MASK_QUIT;
            if(click_up && (click_held&MASK_QUIT))
                selection = MAIN_MENU_QUIT;
        }
        else
            *hover = 0;
    }

    if(click_up) click_held = 0;
    return selection;
}

bool mouseOverQuit(int x, int y, int w, int h)
{
    int start_w = 110;
    int start_h = 50;
    if( !((30 < x)&&(x < 30 + start_w)) )
        return false;
    if((h/4*3 + 120 < y)&&(y < h/4*3+120+start_h))
        return true;
    return false;
}

bool mouseOverOptions(int x, int y, int w, int h)
{
    int options_w = 205;
    int options_h = 50;
    if( !((30 < x)&&(x < 30 + options_w))) 
        return false;
    if((h/4*3 + 75< y)&&(y < h/4*3+75+options_h))
        return true;
    return false;
}

bool mouseOverNew(int x, int y, int w, int h)
{
    int quit_w = 385;
    //int quit_h = 75;
    if( !((30 < x)&&(x < 30 + quit_w))) 
        return false;
    if((h/4*3 - 40 < y)&&(y < h/4*3 + 25))
        return true;
    return false;
}

bool mouseOverContinue(int x, int y, int w, int h)
{
    int quit_w = 241;
    int quit_h = 50;
    if( !((30 < x)&&(x < 30 + quit_w)) )
        return false;
    if((h/4 * 3 +30 < y)&&(y < h/4*3 + 30 + quit_h))
        return true;
    return false;
}

void renderMainOptions
    (SDL_Renderer *r, SDL_Texture * start, SDL_Texture * cont, 
    SDL_Texture * options, SDL_Texture * quit, int w, int h)
{
    int start_x = 30;
    int mid_y = h / 4 * 3;
    SDL_Rect box_start = {start_x,mid_y-40};
    SDL_Rect box_options = {start_x,mid_y + 75};
    SDL_Rect box_quit = {start_x,mid_y + 120};
    SDL_Rect box_cont = {start_x, mid_y + 30};

    SDL_QueryTexture(start,NULL,NULL,&box_start.w,&box_start.h);
    SDL_QueryTexture(cont,NULL,NULL,&box_cont.w,&box_cont.h);
    SDL_QueryTexture(options,NULL,NULL,&box_options.w,&box_options.h);
    SDL_QueryTexture(quit,NULL,NULL,&box_quit.w,&box_quit.h);
    box_start.w /= 2;
    box_options.w /= 3;
    box_quit.w /= 3;
    box_start.h /= 2;
    box_options.h /= 3;
    box_quit.h /= 3;
    box_cont.w /= 3;
    box_cont.h /= 3;

    SDL_RenderCopy(r,start,NULL,&box_start);
    SDL_RenderCopy(r,options,NULL,&box_options);
    SDL_RenderCopy(r,quit,NULL,&box_quit);
    SDL_RenderCopy(r, cont, NULL, &box_cont);


    /*Test to find out clickable regions
    static int print = 0;
    if(!print)
    {
        printf("box_start: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_start.x,box_start.x+box_start.w,box_start.y,box_start.y+box_start.h,box_start.w,box_start.h);
        printf("box_options: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_options.x,box_options.x+box_options.w,box_options.y,box_options.y+box_options.h,box_options.w,box_options.h);
        printf("box_quit: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_quit.x,box_quit.x+box_quit.w,box_quit.y,box_quit.y+box_quit.h,box_quit.w,box_quit.h);
        printf("box_cont: [%d,%d] x [%d,%d] (%d x %d px)\n",
            box_cont.x,box_cont.x+box_cont.w,box_cont.y,box_cont.y+box_cont.h,box_cont.w,box_cont.h);
        print++;
    }*/
    
}