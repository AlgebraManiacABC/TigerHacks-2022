#define MENU_BG "imgs/menu.jpg"
#define SURVIVAL_BG "imgs/survival.png"
#define ENEMY "imgs/enemy.png"
#define DEBRIS_1 "imgs/steel_beam.png"
#define PLAYER "imgs/rocket_player.png"

#ifndef DEBRIS
#define DEBRIS

static const char * debrisFiles[]=
{
	"imgs/steel_beam.png",
	"imgs/asteroid_1.png",
	"imgs/asteroid_2.png",
	"imgs/asteroid_3.png"
};
#define NUM_DEBRIS_TYPES 4

#endif

#define WARP_START_BASE "imgs/warp_animation/Warp_Start_"
#define NUM_WARP_START 21
#define WARP_TUNNEL_BASE "imgs/warp_animation/Warp_Tunnel_"
#define NUM_WARP_TUNNEL 16
#define WARP_END_BASE "imgs/warp_animation/Warp_End_"
#define NUM_WARP_END 18

#define SHIP_ID_BASE "imgs/ship_"
#define NUM_SHIP_TYPES 1
#define DEFAULT_SHIP_ID 1

#define MENUFONT "fonts/Dink/Dink-vKXE.ttf"
#define ROBOTO "fonts/Roboto/Roboto.ttf"