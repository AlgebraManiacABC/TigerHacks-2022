#include "main.h"
#include "assets.h"

#define WARP_NONE 0
#define WARP_SAVE_AND_QUIT 1
#define WARP_MAP 2
#define WARP_CONTINUE 3

#define WARP_MASK_NONE		0b000
#define WARP_MASK_CONTINUE	0b001
#define WARP_MASK_MAP 		0b010
#define WARP_MASK_SAVEQUIT	0b100

int warp(SDL_Renderer *r, int ww, int wh);

//	Render functions:

void playWarpAnim_1(SDL_Renderer *r, int ww, int wh);

int tunnelLoop(SDL_Renderer *r, int ww, int wh);

void playWarpAnim_2(SDL_Renderer *r, int ww, int wh);

void renderTunnelOptions(SDL_Renderer *r, int ww, int wh);

int warpEvaluateClicks(Uint32 *hover, int ww, int wh, SDL_Rect arr[3]);

bool warpMouseOverContinue(int x, int y, int ww, int wh);
bool warpMouseOverMap(int x, int y, int ww, int wh);
bool warpMouseOverSaveQuit(int x, int y, int ww, int wh);