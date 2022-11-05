#include "main.h"
#include "assets.h"

#define WARP_SAVE_AND_QUIT 1
#define WARP_MAP 2
#define WARP_CONTINUE 3

int warp(SDL_Renderer *r, int ww, int wh);

//	Render functions:

void playWarpAnim_1(SDL_Renderer *r, int ww, int wh);

int tunnelLoop(SDL_Renderer *r, int ww, int wh);

void playWarpAnim_2(SDL_Renderer *r, int ww, int wh);