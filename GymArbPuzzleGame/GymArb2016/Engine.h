#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

class Engine
{
public:
	//Width and height of the game window
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 720;


	//the dimensions of the level
	const int LEVEL_WIDTH = 1200;
	const int LEVEL_HEIGHT = 720;

	static const int TOTAL_TILES = 240;
	static const int TOTAL_TILE_SPRITES = 7;

	//tile constants
	const int TILE_WIDTH = 60;
	const int TILE_HEIGHT = 60;

	//the different tile sprites
	const int TILE_NONE = 0;
	const int TILE_BRIDGE = 1;
	const int TILE_BOX = 2;
	const int TILE_GRASS = 3;
	const int TILE_GRASS_CENTER = 4;
	const int TILE_GRASS_CLIFF_LEFT = 5;
	const int TILE_GRASS_CLIFF_RIGHT = 6;

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

