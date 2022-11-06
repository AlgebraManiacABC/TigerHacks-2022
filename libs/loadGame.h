#include "main.h"
#include "survival.h"
#include "warp.h"

#define SAVE_LOCATION "ship.sav"

void startNewGame(SDL_Window *w, SDL_Renderer *r);

spaceShip buildShip(SDL_Renderer *r, shipInfo info);

void loadGame(SDL_Window *w, SDL_Renderer *r);