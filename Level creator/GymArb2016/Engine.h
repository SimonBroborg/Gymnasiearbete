#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <string>

class Engine
{
public:
	//Width and height of the game window
	const int SCREEN_WIDTH = 1260;
	const int SCREEN_HEIGHT = 720;


	//the dimensions of the level
	const int LEVEL_WIDTH = 1260;
	const int LEVEL_HEIGHT = 720;

	static const int TOTAL_TILES = 252;
	static const int TOTAL_TILE_SPRITES = 19;

	//tile constants
	static const int TILE_WIDTH = 60;
	static const int TILE_HEIGHT = 60;

	//the different tile sprites
	const int TILE_NONE = 0;
	const int TILE_BRIDGE = 1;
	const int TILE_BOX = 2;
	const int TILE_GRASS_ROUNDED = 3;
	const int TILE_DIRT_CENTER = 4;
	const int TILE_GRASS_CLIFF_LEFT = 5;
	const int TILE_GRASS_CLIFF_RIGHT = 6;
	const int TILE_GRASS_LEFT_ROUNDED = 7;
	const int TILE_GRASS_CENTER = 8;
	const int TILE_GRASS_RIGHT_ROUNDED = 9;
	const int TILE_PORTAL = 10;
	const int TILE_MOVING_PLATFORM = 11;
	const int TILE_MOVING_PLATFORM_STOP = 12;
	const int TILE_SPIKES = 13;

	const int TILE_ICE_WHOLE = 14;
	const int TILE_ICE_BROKEN_1 = 15;
	const int TILE_ICE_BROKEN_2 = 16;
	const int TILE_ICE_BROKEN_3 = 17;

	const int TILE_SAW_1 = 18;
	

	bool bIsRunning;

	//The game renderer
	SDL_Renderer *renderer;

	bool checkCollision(SDL_Rect a, SDL_Rect b);

	bool checkCircularCollision(SDL_Rect player, SDL_Rect circle, float velX, float velY);

	//Initializing the systems
	void initSystems();

	//The game window
	SDL_Window *window;

	double distanceSquared(int x1, int y1, int x2, int y2);

	Engine();
	~Engine();

	void Print(std::string text);

	void keepInsideBorder(SDL_Rect posRect);

	bool circularCollision(SDL_Rect circle, SDL_Rect rect);

};

