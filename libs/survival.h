#include "main.h"
#include "assets.h"

#define FPS 30.0	//	Frames per second

#define COOLDOWN_TIME 120	//	The default cooldown time

#define MAX_ENEMIES 10	//	The maximum number of enemies that can be spawned
#define MAX_DEBRIS 10	//	The maximum number of space junk that can be spawned

#define V_MAX 200	//	The maximum velocity in pixels per second
#define DECCEL 500	//	The natural decceleration of the ship, in pixels per second, per second
//	(every second, the velocity of the ship is decreased by DECCEL pixels per second (if not accelerating))

//	Details about the ship, including x/y pos/vel
typedef struct
{
	float x;	//	x pixel position of the center of the ship
	float y;	//	y pixel position of the center of the ship
	float vx;	//	x velocity
	float vy;	//	y velocity
	float rotation;	//	clockwise rotation in degrees
	int toughness;	//	hull strength
	int vrotate;	//	rotation speed (maneuverability)
	int accel;	//	acceleration
	int cargo;	//	how much cargo the ship is currently holding
	int shield;	//	current shield level
	int damage;	//	damage taken
	SDL_Texture * tx;
	SDL_Rect rect;
	//weapons next
	//then cosmetics

}	spaceShip;

typedef struct
{
	int x;	//	x pixel position of the center of the debris
	int y;	//	y pixel position of the center of the debris
	int vx;	//	x velocity
	int vy;	//	y velocity
	int rotation;	//	clockwise rotation in degrees
	int size;	//	Size of the debris (big, small)
	int type;	//	Type of space debris

}	debris;

typedef struct
{
	int x;
	int y;
	int vx;
	int vy;
	int rotation;
	int strength;
	//	what it looks like

}	enemy;


typedef struct
{
	debris junk[MAX_DEBRIS];	//	An array of asteroids present on the screen
	enemy * enemies[MAX_ENEMIES];		//	An array of enemies on the screen
	int cooldown;	//	Time left (in seconds) on the cooldown

}	outerSpace;


int survivalLoop(SDL_Window *w, SDL_Renderer *r, spaceShip ship);

//	Could pass the difficulty to these functions to determine danger level
//	Try to increase danger of hazard and shorten wait between next hazard, but not too extreme of both

void spawnAsteroid(int difficulty);

void spawnEnemyShip(int difficulty);

void spawnStrandedShip();

//	Render functions

void renderShip(SDL_Renderer *r, int ww, int wh, spaceShip ship);

void renderDebris(debris * junk[MAX_DEBRIS]);

void renderEnemies(enemy * enemies[MAX_ENEMIES]);

void renderCooldown(SDL_Renderer *r, int ww, int wh, time_t timeRemaining, TTF_Font *font);