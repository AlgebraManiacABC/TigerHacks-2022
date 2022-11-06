#include "main.h"
#include "assets.h"

#define SPAWN_FAIL_PROX 3
#define SPAWN_FAIL_IMG 2
#define SPAWN_FAIL_FULL 1
#define SPAWN_SUCCESS 0
#define SPAWN_FAIL_MEM -1

#define COOLDOWN_TIME 120	//	The default cooldown time

#define MAX_ENEMIES 10	//	The maximum number of enemies that can be spawned
#define MAX_DEBRIS 10	//	The maximum number of space junk that can be spawned

#define MIN_SHIP_DIST 350	//	The minimum pixels away from the ship an enemy or debris can spawn

#define DEBRIS_V_MAX 200
#define SMALL_DEBRIS_ID 1
#define LARGE_DEBRIS_ID 2
#define SMALL_DEBRIS_RADIUS 100	//	The radius of small debris, in pixels
#define LARGE_DEBRIS_RADIUS 200	//	The radius of large debris, in pixels

#define V_MAX 200	//	The maximum velocity in pixels per second
#define DECCEL 500	//	The natural decceleration of the ship, in pixels per second, per second
//	(every second, the velocity of the ship is decreased by DECCEL pixels per second (if not accelerating))

#ifndef SPACESHIP
#define SPACESHIP
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
	float x;	//	x pixel position of the center of the debris
	float y;	//	y pixel position of the center of the debris
	int vx;	//	x velocity
	int vy;	//	y velocity
	float rotation;	//	clockwise rotation in degrees
	int size;	//	Size of the debris (big, small)
	int type;	//	Type of space debris
	suseconds_t spawntime;	//	The time when this piece of debris was spawned
	suseconds_t incoming_x;	//	The allotted duration from spawntime to allowing horizontal border looping
	suseconds_t incoming_y;	//	The allotted duration from spawntime to allowing vertical border looping
	gIMG img;	//	The rect and texture of the debris

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
	debris * junk[MAX_DEBRIS];		//	An array of debris present on the screen
	enemy * enemies[MAX_ENEMIES];	//	An array of enemies on the screen
	int cooldown;	//	Time left (in seconds) on the cooldown

}	outerSpace;
#endif


int survivalLoop(SDL_Window *w, SDL_Renderer *r, spaceShip ship);

//	Could pass the difficulty to these functions to determine danger level
//	Try to increase danger of hazard and shorten wait between next hazard, but not too extreme of both

void initDebrisArray(debris * junk[MAX_ENEMIES]);
void initEnemyArray(enemy * enemies[MAX_ENEMIES]);

int spawnDebris(SDL_Renderer *r, debris * junk[MAX_DEBRIS], int ww, int wh, int shipx, int shipy, int difficulty);
int spawnEnemy(SDL_Renderer *r, enemy * enemies[MAX_ENEMIES], int ww, int wh, int difficulty);

void spawnStrandedShip();

bool isColliding(spaceShip ship, outerSpace env);

void moveDebris(debris * junk[MAX_DEBRIS], int ww, int wh);
void moveEnemies(enemy * enemies[MAX_ENEMIES], int ww, int wh);

//	Render functions

void renderShip(SDL_Renderer *r, int ww, int wh, spaceShip ship);

void renderDebris(SDL_Renderer *r, debris * junk[MAX_DEBRIS],int ww,int wh);
void renderEnemies(SDL_Renderer *r, enemy * enemies[MAX_ENEMIES],int ww,int wh);

void renderCooldown(SDL_Renderer *r, int ww, int wh, time_t timeRemaining, TTF_Font *font);