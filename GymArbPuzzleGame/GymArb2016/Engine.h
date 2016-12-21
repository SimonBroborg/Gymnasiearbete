#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

class Engine
{
public:
	//Width and height of the game window
	static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 720;


	//the dimensions of the level
	static const int LEVEL_WIDTH = 1800;
	static const int LEVEL_HEIGHT = 1260;

	static const int TOTAL_TILES = 630;
	static const int TOTAL_TILE_SPRITES = 14;

	//tile constants
	const int TILE_WIDTH = 60;
	const int TILE_HEIGHT = 60;

	//the different tile sprites
	const int TILE_NONE = 0;
	const int TILE_BRIDGE = 1;
	const int TILE_BOX = 2;
	const int TILE_GRASS_ROUNDED = 3;
	const int TILE_DIRT_CENTER  = 4;
	const int TILE_GRASS_CLIFF_LEFT = 5;
	const int TILE_GRASS_CLIFF_RIGHT = 6;
	const int TILE_GRASS_LEFT_ROUNDED = 7;
	const int TILE_GRASS_CENTER = 8;
	const int TILE_GRASS_RIGHT_ROUNDED = 9;
	const int TILE_PORTAL = 10;
	const int TILE_MOVING_PLATFORM = 11; 
	const int TILE_MOVING_PLATFORM_STOP = 12;


	bool bIsRunning;


	//The game renderer
	SDL_Renderer *renderer;

	bool checkCollision(SDL_Rect a, SDL_Rect b);

	//Initializing the systems
	void initSystems();

	//The game window
	SDL_Window *window;


	
	Engine();
	~Engine();

	
};

