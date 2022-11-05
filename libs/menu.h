#include <stdio.h>
#include <gSDL.h>
#include "assets.h"

#define MAIN_MENU_ERROR    -1
#define START_NEW           0
#define START_CONTINUE      1
#define MAIN_MENU_OPTIONS   2
#define MAIN_MENU_QUIT      3

#define STATE_MAIN      0
#define STATE_START     1

#define MASK_NEW        0b00000001
#define MASK_CONTINUE   0b00000010
#define MASK_OPTIONS    0b00000100
#define MASK_QUIT       0b00001000

#define MAIN_MENU_COUNT 4

/**
 * @brief Summons the main menu.
 * Options:
 * * New Game (launches game loop)
 * * Continue Game (goes to saves screen)
 * * Options (tbd)
 * * Quit (Closes game)
 * 
 * @param r the SDL_Renderer
 * @return a value corresponding to the chosen option
*/
int summon_main_menu(SDL_Window *w, SDL_Renderer *r);

/**
 * @brief determines what main menu buttons are pressed
 * 
 * @param hover a pointer to a bitmask that will be updated
 * to show what is being hovered over
 * @return an int relating to the selection of buttons
 */
int evaluateClicks(Uint32 * hover, int state, bool click_up, bool click_down, int w, int h);

/**
 * @brief Functions that tell if the mouse is hovering over the button
 * 
 * @param x mouse x
 * @param y mouse y
 * @return true if it's hovering over the button, false if not
 */
bool mouse_over_new(int x, int y, int w, int h);
bool mouse_over_continue(int x, int y, int w, int h);
bool mouse_over_options(int x, int y, int w, int h);
bool mouse_over_quit(int x, int y, int w, int h);

/**
 * @brief Places the main menu option text on the screen.
 * 
 * @param r the SDL_Renderer
 * @param start the START text texture (selected or deselected)
 * @param options the OPTIONS text texture (selected or deselected)
 * @param quit the QUIT text texture (selected or deselected)
 *
 */
void present_main_options(SDL_Renderer *r, SDL_Texture * start, SDL_Texture * cont,
    SDL_Texture * options, SDL_Texture * quit, int w, int h);
